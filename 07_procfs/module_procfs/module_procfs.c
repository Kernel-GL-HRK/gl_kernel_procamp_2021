// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>

static char *my_dir = "my_dir";
static char *my_name = "my_name";
static char *my_show = "my_show";
static char *my_store = "my_store";
#define THIS_AUTHOR "Volodymyr Kharuk <jlabpih@ukr.net>"

struct message_list {
	struct list_head entry;
	char *buffer;
};

static struct list_head my_list;
static int show_number_cb;
static int store_number_cb;

static ssize_t list_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	struct message_list *pos;
	char *p = buf, *prehead = "Constent of list";
	ssize_t ret, size, size_left = PAGE_SIZE; // sysfs allocated for us whole page
	int i = 0;

	list_for_each_entry(pos, &my_list, entry) {
		// size of one list element with head message
		size = strlen(pos->buffer) + strlen(prehead) + 4;
		// copy into buffer formatted string
		ret = scnprintf(p, size_left, "%s %d: %s", prehead, i++, pos->buffer);
		if (ret != size)
			return -ENOMEM;

		size_left -= size;
		p += size;
	}

	show_number_cb++;

	return PAGE_SIZE - size_left;
}

static ssize_t list_store(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count)
{
	char *new_msg;
	struct message_list *new_entry;

	new_msg = kzalloc(count, GFP_KERNEL);
	if (!new_msg)
		return -ENOMEM;

	memcpy(new_msg, buf, count);

	new_entry = kzalloc(sizeof(*new_entry), GFP_KERNEL);
	if (!new_entry) {
		kfree(new_msg);
		return -ENOMEM;
	}

	new_entry->buffer = new_msg;
	list_add_tail(&new_entry->entry, &my_list);

	store_number_cb++;

	return count;
}

static struct kobj_attribute my_list_attribute = __ATTR_RW_MODE(list, 0664);

static ssize_t my_proc_read(struct file *file, char __user *pbuf, size_t count, loff_t *ppos)
{
	static char msg[PAGE_SIZE];
	ssize_t size, not_copied;
	char *name = file->f_path.dentry->d_iname;

	/* check end of file */
	if (*ppos != 0)
		return 0;

	/* check what file was used to read */
	if (!strncmp(name, my_store, strlen(my_store)))
		size = scnprintf(msg, PAGE_SIZE, "%d\n", store_number_cb);
	else if (!strncmp(name, my_show, strlen(my_show)))
		size = scnprintf(msg, PAGE_SIZE, "%d\n", show_number_cb);
	else if (!strncmp(name, my_name, strlen(my_name)))
		size = scnprintf(msg, PAGE_SIZE, "%s\n", THIS_AUTHOR);
	else
		size = 0; /* shouldn't be there */

	not_copied = copy_to_user(pbuf, msg, size);

	*ppos += size;

	return size;
}

static struct proc_ops my_proc_ops = {
	.proc_read = my_proc_read,
};

static struct kobject *my_object;
static struct proc_dir_entry *dir;
static struct proc_dir_entry *ent_name;
static struct proc_dir_entry *ent_store;
static struct proc_dir_entry *ent_show;

static int module_procfs_init(void)
{
	int ret;

	show_number_cb = 0;
	store_number_cb = 0;

	my_object = kobject_create_and_add("my_object", kernel_kobj);
	if (!my_object)
		return -ENOMEM;

	ret = sysfs_create_file(my_object, &my_list_attribute.attr);
	if (ret)
		kobject_put(my_object);

	INIT_LIST_HEAD(&my_list);

	dir = proc_mkdir(my_dir, NULL);
	if (dir == NULL) {
		pr_err("module_procfs: error to create my_dir");
		return -ENOMEM;
	}

	ent_name = proc_create(my_name, 0444, dir, &my_proc_ops);
	if (ent_name == NULL) {
		pr_err("module_procfs: error to create my_name");
		return -ENOMEM;
	}

	ent_store = proc_create(my_store, 0444, dir, &my_proc_ops);
	if (ent_store == NULL) {
		pr_err("module_procfs: error to create my_store");
		return -ENOMEM;
	}

	ent_show = proc_create(my_show, 0444, dir, &my_proc_ops);
	if (ent_show == NULL) {
		pr_err("module_procfs: error to create my_show");
		return -ENOMEM;
	}

	pr_info("module_procfs: init");

	return ret;
}

static void module_procfs_exit(void)
{
	struct message_list *pos, *tmp;

	proc_remove(ent_name);
	proc_remove(ent_show);
	proc_remove(ent_store);
	proc_remove(dir);

	list_for_each_entry_safe(pos, tmp, &my_list, entry) {
		list_del(&pos->entry);
		kfree(pos->buffer);
		kfree(pos);
	}
	kobject_put(my_object);
	pr_info("module_procfs: exit");
}

module_init(module_procfs_init);
module_exit(module_procfs_exit);

MODULE_AUTHOR(THIS_AUTHOR);
MODULE_LICENSE("GPL v2");
