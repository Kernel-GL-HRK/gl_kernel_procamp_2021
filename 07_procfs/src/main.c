/*
 *main.c: test module
 *
 *(C) Copyright 2021
 *Author: Vitaliy Zhyrko <vitaliyzh@gmail.com>
 *
 *This program is free software; you can redistribute it and/or
 *modify it under the terms of the GNU General Public License
 *as published by the Free Software Foundation; version 2
 *of the License.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/time.h>
#include <linux/proc_fs.h>

#define DRIVER_AUTHOR "Vitaliy Zhyrko <vitaliyzh@gmail.com>"

#define MODULE_TAG      "example_module "
#define PROC_DIRECTORY  "example"
#define PROC_FILENAME   "buffer"
#define PROC_FILENAME_1   "author"
#define PROC_FILENAME_2   "store"
#define PROC_FILENAME_3   "show"
#define BUFFER_SIZE     32
#define LEN_MSG 160

struct msg_item {
	struct list_head msg_list;
	char *message;
	size_t lenght;
};

LIST_HEAD(msg_list);


static char *proc_buffer;
static size_t proc_msg_length;
static size_t proc_msg_read_pos;

static int store_count;
static int show_count;

static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file, *proc_file_2, *proc_file_3;

static ssize_t example_read(struct file *file_p, char __user *buffer,
	size_t length, loff_t *offset);
static ssize_t example_write(struct file *file_p, const char __user *buffer,
	 size_t length, loff_t *offset);


#if KERNEL_VERSION(5, 10, 0) <= LINUX_VERSION_CODE
static struct proc_ops proc_fops = {
	.proc_read  = example_read,
	.proc_write = example_write,
};
#else
static const struct file_operations proc_fops = {
	.read  = example_read,
	.write = example_write,
};
#endif

void show_msgs(void)
{
	struct list_head *iter;
	struct msg_item *item;

	list_for_each(iter, &msg_list)
	{
	   item = list_entry(iter, struct msg_item, msg_list);
	   printk(KERN_INFO "Show: [MSG] %s, [LEN] %ld\n", item->message,
			  item->lenght);
	}
}

static void add_msg(const char *msg, const size_t len)
{
	struct msg_item *item;

	item = kmalloc(sizeof(*item), GFP_KERNEL);
	item->lenght = len;
	item->message = kmalloc(len, GFP_KERNEL);
	memcpy(item->message, (char *)msg, item->lenght);
	list_add(&(item->msg_list), &msg_list);
	printk(KERN_INFO "Add: [MSG] %s, [LEN] %ld\n", item->message,
		   item->lenght);
}

void free_lists(void)
{
	struct list_head *iter, *iter_safe;
	struct msg_item *item;

	list_for_each_safe(iter, iter_safe, &msg_list) {
		item = list_entry(iter, struct msg_item, msg_list);
		list_del(iter);
		kfree(item->message);
		kfree(item);
	}
}

#if KERNEL_VERSION(2, 6, 32) < LINUX_VERSION_CODE

#define IOFUNCS(name)                                                         \
static char buf_##name[LEN_MSG + 1] = "0\n";                                  \
static ssize_t SHOW_##name(struct class *class, struct class_attribute *attr, \
			    char *buf) \
{                                       \
	strcpy(buf, buf_##name);                                                  \
	printk(KERN_INFO "read %ld\n", (long)strlen(buf));                        \
	show_msgs();                                                              \
	show_count++;                                                             \
	return strlen(buf);                                                       \
}                                                                             \
static ssize_t STORE_##name(struct class *class, struct class_attribute *attr,\
			     const char *buf, size_t count) \
{                  \
	printk(KERN_INFO "write %ld\n", (long)count);                             \
	strncpy(buf_##name, buf, count);                                          \
	buf_##name[count] = '\0';                                                 \
	add_msg(buf, count);                                                      \
	store_count++;                                                            \
	return count;                                                             \
}

#else

#define IOFUNCS(name)                                                         \
static char buf_##name[LEN_MSG + 1] = "не инициализировано "#name"\n";        \
static ssize_t SHOW_##name(struct class *class, char *buf) \
{	\
	strcpy(buf, buf_##name);                                                  \
	printk(KERN_INFO "read %ld\n", (long)strlen(buf));                        \
	return strlen(buf);                                                       \
}                                                                             \
static ssize_t STORE_##name(struct class *class, const char *buf,             \
			     size_t count) \
{                                   \
	printk(KERN_INFO "write %ld\n", (long)count);                             \
	strncpy(buf_##name, buf, count);                                          \
	buf_##name[count] = '\0';                                                 \
	return count;                                                             \
}
#endif

IOFUNCS(list);
#define OWN_CLASS_ATTR(name) \
	struct class_attribute class_attr_##name = \
	__ATTR(name, (0770 /*| S_IRWXO*/), &SHOW_##name, &STORE_##name)

static OWN_CLASS_ATTR(list);

static struct kobject *my_kobj;

static struct class *x_class;

static int create_sysclass(void)
{
	int res;

	x_class = class_create(THIS_MODULE, "example-class");
	if (IS_ERR(x_class))
		printk(KERN_INFO "bad class create\n");
	res = class_create_file(x_class, &class_attr_list);

	return res;
}

void x_cleanup(void)
{
	if (x_class) {
		class_remove_file(x_class, &class_attr_list);
		class_destroy(x_class);
	}
}


static ssize_t example_read(struct file *file_p, char __user *buffer,
				size_t length, loff_t *offset)
{
	char bufff[128];
	size_t left = 0;

	printk(KERN_INFO MODULE_TAG "read %s, from %llu(%lu)",
		   file_p->f_path.dentry->d_name.name, *offset, length);

	if (strncmp(file_p->f_path.dentry->d_name.name, PROC_FILENAME_1,
				sizeof(PROC_FILENAME_1)) == 0) {
		proc_msg_length = snprintf(bufff, sizeof(bufff), "author: %s%c",
							   DRIVER_AUTHOR, '\n');
		bufff[proc_msg_length+1] = 0;
	} else if (strncmp(file_p->f_path.dentry->d_name.name, PROC_FILENAME_2,
					   sizeof(PROC_FILENAME_2)) == 0) {
		proc_msg_length = snprintf(bufff, sizeof(bufff), "store count: %d%c",
							   store_count, '\n');
		bufff[proc_msg_length+1] = 0;
	} else if (strncmp(file_p->f_path.dentry->d_name.name, PROC_FILENAME_3,
					   sizeof(PROC_FILENAME_3)) == 0) {
		proc_msg_length = snprintf(bufff, sizeof(bufff), "show count: %d%c",
							   show_count, '\n');
		bufff[proc_msg_length+1] = 0;
	}
	if (length > proc_msg_length-proc_msg_read_pos)
		length = proc_msg_length-proc_msg_read_pos;
	left = copy_to_user(buffer, &bufff[proc_msg_read_pos], length);
	proc_msg_read_pos += length - left;
	if (*offset >= proc_msg_length)
		proc_msg_read_pos = 0;
	*offset += proc_msg_length;
	return length - left;
}

static ssize_t example_write(struct file *file_p, const char __user *buffer,
				 size_t length, loff_t *offset)
{
	return 0;
}


static int create_proc_example(void)
{
	proc_dir = proc_mkdir(PROC_DIRECTORY, NULL);
	if (proc_dir == NULL)
		return -EFAULT;

	proc_file = proc_create(PROC_FILENAME_1, S_IFREG | 0666, proc_dir,
							&proc_fops);
	if (proc_file == NULL)
		return -EFAULT;

	proc_file_2 = proc_create(PROC_FILENAME_2, S_IFREG | 0666, proc_dir,
							  &proc_fops);
	if (proc_file_2 == NULL)
		return -EFAULT;

	proc_file_3 = proc_create(PROC_FILENAME_3, S_IFREG | 0666, proc_dir,
							  &proc_fops);
	if (proc_file_3 == NULL)
		return -EFAULT;

	return 0;
}

static void cleanup_proc_example(void)
{
	if (proc_file_3) {
		remove_proc_entry(PROC_FILENAME_3, proc_dir);
		proc_file_3 = NULL;
	}
	if (proc_file_2) {
		remove_proc_entry(PROC_FILENAME_2, proc_dir);
		proc_file_2 = NULL;
	}
	if (proc_file) {
		remove_proc_entry(PROC_FILENAME_1, proc_dir);
		proc_file = NULL;
	}
	if (proc_dir) {
		remove_proc_entry(PROC_DIRECTORY, NULL);
		proc_dir = NULL;
	}
}

static int create_buffer(void)
{
	int res = 0;

	proc_buffer = kmalloc(BUFFER_SIZE+1, GFP_KERNEL);
	if (proc_buffer == NULL)
		return -ENOMEM;
	proc_msg_length = 0;


	my_kobj = kobject_create_and_add("my_kobj", kernel_kobj);
	if (!my_kobj)
		return -ENOMEM;
	res = sysfs_create_file(my_kobj, &class_attr_list.attr);
	if (res)
		kobject_put(my_kobj);

	return res;
}

static void cleanup_buffer(void)
{
	if (proc_buffer) {
		kfree(proc_buffer);
		proc_buffer = NULL;
	}
	proc_msg_length = 0;

	free_lists();
	kobject_put(my_kobj);
}

static int __init example_init(void)
{
	int err;

	err = create_buffer();
	if (err)
		goto error;

	err = create_sysclass();
	if (err)
		goto error;

	err = create_proc_example();
	if (err)
		goto error;

	printk(KERN_NOTICE MODULE_TAG "loaded\n");
		return 0;

error:
	printk(KERN_ERR MODULE_TAG "failed to load\n");
	cleanup_proc_example();
	x_cleanup();
	cleanup_buffer();
	return err;
}

static void __exit example_exit(void)
{
	cleanup_proc_example();
	x_cleanup();
	cleanup_buffer();
	printk(KERN_NOTICE MODULE_TAG "exited\n");
}

module_init(example_init);
module_exit(example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("Example for sysfs show/store");
