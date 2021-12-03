#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>

#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME

struct my_list{
	struct list_head list;
	char * record; 
};

static struct my_list node_list;

static ssize_t module_read(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	struct list_head *listptr;
	struct my_list * entry;

	pr_info("show_list %lu\n", node_list.list);
	list_for_each(listptr, &(node_list.list)) {
		entry = list_entry(listptr, struct my_list, list);
		pr_info("record = %s (list %lu, prev = %lu, next = %lu)\n", \
				entry->record, &entry->list, entry->list.prev, entry->list.next);
		//strcat(buf, entry->record);
		sprintf ( buf + strlen(buf), "%s \n", entry->record );
	}
	return strlen(buf);
}

static ssize_t module_write(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	struct my_list *new_node;
	new_node = kmalloc(sizeof(struct my_list), GFP_KERNEL);
	INIT_LIST_HEAD(&new_node->list);
	if(new_node) {
		new_node->record = kmalloc(strlen(buf), GFP_KERNEL);
		sscanf(buf, "%s\n", new_node->record);
		list_add(&new_node->list, &node_list.list);
		pr_info("Adding new record: %s, head = %lu, prev = %lu, next = %lu\n", new_node->record, \
				node_list.list, node_list.list.prev, node_list.list.next);
	}
	return count;
}

static struct kobj_attribute list_attribute = \
		__ATTR(List, 0664, module_read, module_write);

static struct kobject *module_kobj;

int start_module(void)
{
	int res = 0;

	pr_info("Starting List object");
	INIT_LIST_HEAD(&node_list.list);

	module_kobj = kobject_create_and_add("My_list", module_kobj);

	if(!module_kobj){
		pr_crit("");
		return -ENOMEM;
	}
	pr_info("List obbject created: %s , %lu", module_kobj->name, &module_kobj);

	res = sysfs_create_file(module_kobj, &list_attribute.attr);
	if(res){
		kobject_put(module_kobj);
	}
	return res;
}

void exit_module(void) {
	struct my_list *entry;
	struct list_head *pos, *temp;


	list_for_each_safe(pos, temp, &(node_list.list)) {
		entry = list_entry(pos, struct my_list, list);
		if(!entry){
			pr_info("Error allocatin memory for entry %u\n",  -ENOMEM);
			return;
		}
		else
		{
			pr_info("Deleting record: %s\n", entry->record);
			kfree(entry->record);
			pr_info("Deleting list node: %lu\n", entry->list);
			list_del(&(entry->list));
		}
	}

	kobject_put(module_kobj);
	pr_info("Module exit.\n");

}

module_init(start_module)
module_exit(exit_module)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Serhii.Sherepera");
MODULE_DESCRIPTION("Lesson_4 \"Module interfaces\" homework");
MODULE_VERSION("0.1");
