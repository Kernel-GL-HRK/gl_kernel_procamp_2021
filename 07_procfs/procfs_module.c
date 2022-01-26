#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define MODULE_AUTHOR_STRING                                                   \
	"Valentyn Korniienko <valentyn.korniienko1@nure.ua>"


typedef struct string_list_item {
	struct list_head list;
	char *stored_string;
	size_t stored_string_length;
} string_list_item_t;

static struct kobject *list_obj;
static struct list_head string_list;

typedef struct proc_entries_data {
	ssize_t write_requests;
	ssize_t read_requests;
	const char *const module_author_info;
} proc_entries_data_t;

static proc_entries_data_t proc_entries_data_instance = {
	.write_requests = 0,
	.read_requests = 0,
	.module_author_info = MODULE_AUTHOR_STRING
};

static struct proc_dir_entry *proc_entry_module_root;

static int procfs_module_show_author(struct seq_file *m, void *v)
{
	seq_printf(m, "Module author: %s\n",
		   proc_entries_data_instance.module_author_info);
	return 0;
}

static int procfs_module_show_write_requests(struct seq_file *m, void *v)
{
	seq_printf(m, "Write requests: %ld\n",
		   proc_entries_data_instance.write_requests);
	return 0;
}

static int procfs_module_show_read_requests(struct seq_file *m, void *v)
{
	seq_printf(m, "Read requests: %ld\n",
		   proc_entries_data_instance.read_requests);
	return 0;
}

static ssize_t add_new_string_entity(const char *buffer)
{
	string_list_item_t *p_entity =
		kmalloc(sizeof(string_list_item_t), GFP_KERNEL);
	if (!p_entity)
		return -ENOMEM;

	const size_t string_length = strlen(buffer);
	const size_t string_buffer_mem_size = string_length + 1;
	char *string_buffer = kmalloc(string_buffer_mem_size, GFP_KERNEL);
	if (!string_buffer) {
		kfree(p_entity);
		return -ENOMEM;
	}
	p_entity->stored_string = string_buffer;

	strncpy(string_buffer, buffer, string_buffer_mem_size);
	p_entity->stored_string_length = string_length;

	list_add_tail(&p_entity->list, &string_list);

	return string_length;
}

static void string_item_dtor(string_list_item_t *string_list_item)
{
	if (!string_list_item)
		return;

	kfree(string_list_item->stored_string);
	string_list_item->stored_string = NULL;
	string_list_item->stored_string_length = 0;
	kfree(string_list_item);
}
static void cleanup_storage(void)
{
	string_list_item_t *string_it = NULL;
	struct list_head *pos, *q;
	list_for_each_safe (pos, q, &string_list) {
		string_it = list_entry(pos, string_list_item_t, list);
		list_del(pos);
		string_item_dtor(string_it);
	}
}
static ssize_t list_show(struct kobject *kobj, struct kobj_attribute *attr,
			 char *buf)
{
	++proc_entries_data_instance.read_requests;

	char *buffer_pointer = buf;
	ssize_t pointer_offset = 0;
	string_list_item_t *list_entity_it = NULL;
	list_for_each_entry (list_entity_it, &string_list, list) {
		size_t copied_data_size =
			list_entity_it->stored_string_length + 1;
		ssize_t copy_result =
			sprintf(buffer_pointer + pointer_offset, "%s",
				list_entity_it->stored_string);
		pointer_offset += copy_result;
	}
	pr_debug(
		"==BASIC_STRUCT==:Called list_show with copied pointer_offset:%ld\n",
		pointer_offset);

	return pointer_offset;
}

static ssize_t list_store(struct kobject *kobj, struct kobj_attribute *attr,
			  const char *buf, size_t count)
{
	++proc_entries_data_instance.write_requests;

	pr_debug("==BASIC_STRUCT==:Called list_store with count:%ld\n", count);
	ssize_t res = add_new_string_entity(buf);
	if (!res)
		return -EFAULT;
	return count;
}

struct kobj_attribute list_attribute =
	__ATTR(list, 0660, list_show, list_store);

static int __init simple_struct_module_init(void)
{
	int result = 0;
	list_obj = kobject_create_and_add("basic_struct", kernel_kobj);
	if (!list_obj) {
		pr_warn("%s:%d: kobject_create_and_add failed.\n", __func__,
			__LINE__);
		return -ENOMEM;
	}
	result = sysfs_create_file(list_obj, &list_attribute.attr);
	if (result) {
		pr_warn("%s:%d: sysfs_create_file failed.\n", __func__,
			__LINE__);
		goto cleanup_kobject;
	}
	INIT_LIST_HEAD(&string_list);

	proc_entry_module_root = proc_mkdir("task07_module_entries", NULL);
	if (!proc_entry_module_root) {
		pr_warn("task07_module_entries: unable to create /proc/proc_entry_module_root\n");
		goto cleanup_kobject;
	}
	if (!proc_create_single(
		    "module_author", S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH,
		    proc_entry_module_root, procfs_module_show_author)) {
		pr_warn("unable to register /proc/task07_module_entries/task07_module_author\n");
		goto cleanup_kobject_and_procfs;
	}
	if (!proc_create_single("write_requests",
				S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH,
				proc_entry_module_root,
				procfs_module_show_write_requests)) {
		pr_warn("unable to register /proc/task07_module_entries/write_requests\n");

		remove_proc_entry("module_author", proc_entry_module_root);
		goto cleanup_kobject_and_procfs;
	}
	if (!proc_create_single(
		    "read_requests", S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH,
		    proc_entry_module_root, procfs_module_show_read_requests)) {
		pr_warn("unable to register /proc/task07_module_entries/read_requests\n");
		remove_proc_entry("module_author", proc_entry_module_root);
		remove_proc_entry("write_requests", proc_entry_module_root);

		goto cleanup_kobject_and_procfs;
	}

	pr_debug("==BASIC_STRUCT==:Module ready\n");
	return result;

cleanup_kobject:
	kobject_put(list_obj);
	return -ENOMEM;
cleanup_kobject_and_procfs:
	kobject_put(list_obj);
	remove_proc_entry("task07_module_entries", NULL);

	return -ENOMEM;
}

static void __exit simple_struct_module_exit(void)
{
	cleanup_storage();

	remove_proc_entry("read_requests", proc_entry_module_root);
	remove_proc_entry("write_requests", proc_entry_module_root);
	remove_proc_entry("module_author", proc_entry_module_root);

	remove_proc_entry("task07_module_entries", NULL);

	kobject_put(list_obj);

	pr_debug("==BASIC_STRUCT==:Module unloaded\n");
}

module_init(simple_struct_module_init);
module_exit(simple_struct_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Valentyn Korniienko");