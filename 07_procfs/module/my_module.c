#define DEBUG
#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/proc_fs.h>

#define AUTHOR		"Michael N."

struct my_object {
	struct list_head list;
	char str[128];
};

static struct kobject *my_kobj;
static struct list_head head;
static int show_count, store_count;

static ssize_t hello_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	struct list_head *p;
	struct my_object *entry;
	int len = 0;

	show_count++;

	list_for_each(p, &head) {
		entry = list_entry(p, struct my_object, list);

		pr_debug("entry: %p %s\n", entry, entry->str);

		len += sprintf(&buf[len], "[%s]->", entry->str);
	}
	len -= 2;
	buf[len++] = '\n';
	buf[len++] = '\0';

	return len;
}

static ssize_t hello_store(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count)
{
	struct my_object *myobj;

	store_count++;

	myobj = kzalloc(sizeof(struct my_object), GFP_KERNEL);
	if (!myobj) {
		pr_err("Can not create object\n");
		return -ENOMEM;
	}

	strlcpy(myobj->str, buf, sizeof(myobj->str));
	if (count < sizeof(myobj->str))
		/* remove \n character */
		myobj->str[count - 1] = '\0';

	list_add_tail(&myobj->list, &head);

	pr_debug("ADD entry: %p %s\n", myobj, myobj->str);

	return count;
}

static struct kobj_attribute list_attribute =
	__ATTR(list, 0664, hello_show, hello_store);


static ssize_t my_read(struct file *file, char __user *pbuf, size_t count, loff_t *ppos)
{
	char tmp[256];
	int ret;

	if (*ppos > 0)
		return 0;

	if (strcmp(file->f_path.dentry->d_iname, "store_count") == 0) {
		ret = sprintf(tmp, "%d\n", store_count);
	} else if (strcmp(file->f_path.dentry->d_iname, "show_count") == 0) {
		ret = sprintf(tmp, "%d\n", show_count);
	} else {
		/* author */
		ret = sprintf(tmp, AUTHOR"\n");
	}

	copy_to_user(pbuf, tmp, ret);
	*ppos = ret;

	return ret;
}

static const struct proc_ops my_proc_ops =
{
	.proc_read = my_read,
};


static struct proc_dir_entry *proc_dir;

static int my_module_init(void)
{
	int res;

	my_kobj = kobject_create_and_add("my_module", kernel_kobj);
	if (!my_kobj)
		return -ENOMEM;

	res = sysfs_create_file(my_kobj, &list_attribute.attr);
	if (res)
		kobject_put(my_kobj);

	INIT_LIST_HEAD(&head);


	proc_dir = proc_mkdir("my_module", NULL);
	if (proc_dir) {
		if (proc_create("author", 0444, proc_dir, &my_proc_ops) == NULL)
			goto _error;
		if (proc_create("store_count", 0444, proc_dir, &my_proc_ops) == NULL)
			goto _error;
		if (proc_create("show_count", 0444, proc_dir, &my_proc_ops) == NULL)
			goto _error;
	}

	return res;

_error:
	if (proc_dir)
		proc_remove(proc_dir);

	return -EPERM;
}

static void my_module_exit(void)
{
	struct list_head *p;
	struct list_head *n;
	struct my_object *entry;

	if (proc_dir)
		proc_remove(proc_dir);

	list_for_each_prev_safe(p, n, &head) {
		entry = list_entry(p, struct my_object, list);

		pr_debug("DEL entry: %p %s\n", entry, entry->str);

		list_del(&entry->list);
		kfree(entry);
	}

	kobject_put(my_kobj);

	pr_debug("module exited\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_AUTHOR(AUTHOR" <michael.n@gmail.com>");
MODULE_DESCRIPTION("Basic data structures module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
