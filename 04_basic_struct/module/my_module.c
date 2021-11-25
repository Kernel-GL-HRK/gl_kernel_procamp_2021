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

struct my_object {
	struct list_head list;
	char str[128];
};

static struct kobject *hello_kobj;
static struct list_head head;


static ssize_t hello_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	struct list_head *p;
	struct my_object *entry;
	int len = 0;

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
	struct my_object *myobj = kzalloc(sizeof(struct my_object), GFP_KERNEL);
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

static int my_module_init(void)
{
	int res;

	hello_kobj = kobject_create_and_add("my_module", kernel_kobj);
	if (!hello_kobj)
		return -ENOMEM;

	res = sysfs_create_file(hello_kobj, &list_attribute.attr);
	if (res)
		kobject_put(hello_kobj);

	INIT_LIST_HEAD(&head);

	return res;
}

static void my_module_exit(void)
{
	struct list_head *p;
	struct list_head *n;
	struct my_object *entry;

	list_for_each_prev_safe(p, n, &head) {
		entry = list_entry(p, struct my_object, list);

		pr_debug("DEL entry: %p %s\n", entry, entry->str);

		list_del(&entry->list);
		kfree(entry);
	}

	kobject_put(hello_kobj);

	pr_debug("module exited\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_AUTHOR("Michael N. <michael.n@gmail.com>");
MODULE_DESCRIPTION("Basic data structures module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
