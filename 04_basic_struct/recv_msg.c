/*
 * Receiving messages module
 */
#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/list.h>

MODULE_AUTHOR("Volodymyr Kniazkyi <kniazkyi@gmail.com>");
MODULE_DESCRIPTION("Message receive module");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

struct msg_list {
	struct list_head list;
	char *msg_data;
};

static struct list_head head;

/*
 * Read all data from message list
 */
static ssize_t message_show_all(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	struct msg_list *curr_msg;
	struct list_head *listptr;
	size_t wr_len = 0;

	list_for_each(listptr, &head) {
		curr_msg = list_entry(listptr, struct msg_list, list);

		//pr_info("Read data: %s",curr_msg->msg_data);

		strcpy(buf+wr_len, curr_msg->msg_data);
		wr_len += strlen(curr_msg->msg_data);
	}
	return wr_len;
}

/*
 * Add new entry to message list (FIFO or Stack)
 */
static ssize_t message_add_new(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count)
{
	//allocate memory buffer for new entry of msg_list
	struct msg_list *new_msg = kmalloc(sizeof(struct msg_list), GFP_KERNEL);

	if (!new_msg)
		return -ENOMEM;

	//allocate memory buffer for  message data
	new_msg->msg_data = kmalloc(count, GFP_KERNEL);

	if (!new_msg->msg_data)
		return -ENOMEM;

	strcpy(new_msg->msg_data, buf);

	//add new data to FIFO
	list_add_tail(&new_msg->list, &head);
	//add new data to Stack
	//list_add(&new_msg->list, &head);

	//pr_info("New message received (length=%i): %s",(int)count,new_msg->msg_data);
	return count;
}

static struct kobj_attribute list_attribute =
	__ATTR(list, 0664, message_show_all, message_add_new);

static struct kobject *message_kobj;

static int message_module_init(void)
{
	int res;

	message_kobj = kobject_create_and_add("recv_msg", kernel_kobj);
	if (!message_kobj)
		return -ENOMEM;

	res = sysfs_create_file(message_kobj, &list_attribute.attr);
	if (res)
		kobject_put(message_kobj);

	INIT_LIST_HEAD(&head);
	//pr_info("module init\n");
	return res;
}

static void message_module_exit(void)
{
	struct msg_list *curr_msg;
	struct msg_list *tmp_msg;

	list_for_each_entry_safe(curr_msg, tmp_msg, &head, list) {
		//delete message from list
		list_del(&curr_msg->list);

		//pr_info("Clear message: %s",curr_msg->msg_data);

		//free memory for message buffer
		kfree(curr_msg->msg_data);
		//free memory for message
		kfree(curr_msg);
	}
	if (!list_empty(&head))
		pr_info("Alarm!!! message list is not empty\n");

	kobject_put(message_kobj);
	//pr_info("module exit\n");
}

module_init(message_module_init);
module_exit(message_module_exit);
