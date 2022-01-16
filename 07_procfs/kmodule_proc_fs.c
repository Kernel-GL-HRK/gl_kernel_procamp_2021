// SPDX-License-Identifier: GPL-2.0-only
/*
 *Module implements the following task:
 *Update your existing sysfs kernel module with procfs API:
 *Create folder in procfs file system;
 *Create entry that returns module author name;
 *Create entry that returns amount of "store" callback calls;
 *Create entry that returns amount of "show" callback calls.
 *Build image for orange pi zero
 *Attach console output from your development board
 */

#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>

#define MODULEAUTHOR "Nazarii Kurylko <kurylko.n@gmail.com>"

MODULE_AUTHOR(MODULEAUTHOR);
MODULE_DESCRIPTION("Basic data structures module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

#define MODULE_NAME "MyObject"
#define FILE_AUTHOR "author"
#define FILE_SHOWCOUNT "showcount"
#define FILE_STORECOUNT "storecount"
#define MAX_NODESS_IN_LIST (100)

static unsigned int nodes_in_list;
static unsigned int showcount;
static unsigned int storecount;

LIST_HEAD(list_head);

struct str_node {
	struct list_head list;
	char *str_ptr;
};

static ssize_t myobject_show(struct kobject *kobj, struct kobj_attribute *attr,
			     char *buf)
{
	int bytes = 0;
	int res = 0;
	struct str_node *node = NULL;
	struct str_node *node_temp = NULL;

	// print all string to output
	list_for_each_entry_safe(node, node_temp, &list_head, list) {
		res = scnprintf(&buf[bytes], (PAGE_SIZE - bytes), "%s\n",
				node->str_ptr);
		if (res <= 0) {
			pr_debug("myobject: %s->scnprintf returned %d\n",
				 __func__, res);
			return -EIO;
		}
		bytes += res;
	}

	showcount++;
	return bytes + 1; //  +1 - for 0 termination
}

static ssize_t myobject_store(struct kobject *kobj, struct kobj_attribute *attr,
			      const char *buf, size_t count)
{
	//allocate memory for the struct.
	struct str_node *new = NULL;

	if (nodes_in_list > MAX_NODESS_IN_LIST) {
		pr_debug("myobject: maximum number of strings reached %u\n",
			 nodes_in_list);
		return -ECANCELED;
	}

	// if no string return;
	if (count < 1)
		return count;

	//alocate the link list node
	new = kmalloc(sizeof(struct str_node), GFP_KERNEL);
	if (new == NULL)
		return -ENOMEM;

	new->str_ptr = NULL; //initialize pointer

	//allocate the memory for string.
	new->str_ptr = kzalloc(count + 1, GFP_KERNEL);
	if (new->str_ptr == NULL) {
		kfree(new);
		pr_debug("myobject: kzalloc returned error\n");
		return -ENOMEM;
	}

	//copy string to allocated buffer
	memcpy(new->str_ptr, buf, count + 1);

	//next prev pointer initalisation
	INIT_LIST_HEAD(&new->list);

	//add new node between head and previous node
	list_add(&(new->list), &list_head);

	nodes_in_list += 1;

	storecount++;

	return count;
}

static struct kobj_attribute list_attribute =
	__ATTR(list, 0664, myobject_show, myobject_store);

static struct kobject *myobject_kobj;

static struct proc_dir_entry *proc_dir, *author_file, *showcount_file,
	*storecount_file;

static ssize_t read_author(struct file *file, char __user *pbuf, size_t count,
			   loff_t *ppos)
{
	int not_copied = copy_to_user(pbuf, MODULEAUTHOR, sizeof(MODULEAUTHOR));
	*ppos += sizeof(MODULEAUTHOR) - not_copied;

	if (*ppos > sizeof(MODULEAUTHOR))
		return 0;
	else
		return sizeof(MODULEAUTHOR);
}

static ssize_t read_showcount(struct file *file, char __user *pbuf,
			      size_t count, loff_t *ppos)
{
	char tmp_buff[255];
	int bytes = scnprintf(tmp_buff, sizeof(tmp_buff), "showcount = %u\n",
			      showcount);

	int not_copied = copy_to_user(pbuf, tmp_buff, bytes);
	*ppos += bytes - not_copied;

	if (*ppos > bytes)
		return 0;
	else
		return bytes;
}

static ssize_t read_storecount(struct file *file, char __user *pbuf,
			       size_t count, loff_t *ppos)
{
	char tmp_buff[255];
	int bytes = scnprintf(tmp_buff, sizeof(tmp_buff), "storecount = %u\n",
			      storecount);

	int not_copied = copy_to_user(pbuf, tmp_buff, bytes);
	*ppos += bytes - not_copied;

	if (*ppos > bytes)
		return 0;
	else
		return bytes;
}

static struct proc_ops proc_ops_author = {
	.proc_read = read_author,
};

static struct proc_ops proc_ops_showcount = {
	.proc_read = read_showcount,
};

static struct proc_ops proc_ops_storecount = {
	.proc_read = read_storecount,
};

static int myobject_init(void)
{
	int res = 0;

	myobject_kobj = kobject_create_and_add(MODULE_NAME, kernel_kobj);
	if (!myobject_kobj) {
		pr_debug("myobject: kobject_create_and_add returned error\n");
		return -ENOMEM;
	}
	res = sysfs_create_file(myobject_kobj, &list_attribute.attr);
	if (res) {
		pr_debug("myobject: sysfs_create_file returned error\n");
		kobject_put(myobject_kobj);
		return -ENOMEM;
	}

	/* create directory */
	proc_dir = proc_mkdir(MODULE_NAME, NULL);
	if (proc_dir == NULL) {
		pr_debug("myobject: proc_mkdir returned error\n");
		kobject_put(myobject_kobj);
		return -ENOMEM;
	}

	/* create author file */
	author_file =
		proc_create(FILE_AUTHOR, 0444, proc_dir, &proc_ops_author);
	if (author_file == NULL) {
		pr_debug("myobject: proc_create1 returned error\n");
		kobject_put(myobject_kobj);
		return -ENOMEM;
	}

	/* create showcount file */
	showcount_file = proc_create(FILE_SHOWCOUNT, 0444, proc_dir,
				     &proc_ops_showcount);
	if (showcount_file == NULL) {
		pr_debug("myobject: proc_create2 returned error\n");
		kobject_put(myobject_kobj);
		return -ENOMEM;
	}

	/* create storecount file */
	storecount_file = proc_create(FILE_STORECOUNT, 0444, proc_dir,
				      &proc_ops_storecount);
	if (storecount_file == NULL) {
		pr_debug("myobject: proc_create3 returned error\n");
		kobject_put(myobject_kobj);
		return -ENOMEM;
	}
	return res;
}

static void myobject_exit(void)
{
	struct str_node *node = NULL;
	struct str_node *node_temp = NULL;

	kobject_put(myobject_kobj);

	//free resources
	list_for_each_entry_safe(node, node_temp, &list_head, list) {
		list_del(&node->list);
		kfree(node->str_ptr);
		kfree(node);
	}

	proc_remove(proc_dir);
	proc_remove(author_file);
	proc_remove(showcount_file);
	proc_remove(storecount_file);
}

module_init(myobject_init);
module_exit(myobject_exit);
