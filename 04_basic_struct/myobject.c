// SPDX-License-Identifier: GPL-2.0-only

#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_AUTHOR("Nazarii Kurylko <kurylko.n@gmail.com>");
MODULE_DESCRIPTION("Basic data structures module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

#define MAX_NODESS_IN_LIST (100)

static unsigned int nodes_in_list;

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
	list_for_each_entry_safe (node, node_temp, &list_head, list) {
		res = scnprintf(&buf[bytes], (PAGE_SIZE - bytes), "%s\n",
				node->str_ptr);
		if (res <= 0) {
			pr_debug("myobject: %s->scnprintf returned %d\n",
				 __func__, res);
			return -EIO;
		}
		bytes += res;
	}

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

	return count;
}

static struct kobj_attribute list_attribute =
	__ATTR(list, 0664, myobject_show, myobject_store);

static struct kobject *myobject_kobj;

static int myobject_init(void)
{
	int res = 0;

	myobject_kobj = kobject_create_and_add("MyObject", kernel_kobj);
	if (!myobject_kobj)
		return -ENOMEM;
	res = sysfs_create_file(myobject_kobj, &list_attribute.attr);
	if (res)
		kobject_put(myobject_kobj);
	return res;
}

static void myobject_exit(void)
{
	struct str_node *node = NULL;
	struct str_node *node_temp = NULL;

	kobject_put(myobject_kobj);

	//free resources
	list_for_each_entry_safe (node, node_temp, &list_head, list) {
		list_del(&node->list);
		kfree(node->str_ptr);
		kfree(node);
	}
}

module_init(myobject_init);
module_exit(myobject_exit);
