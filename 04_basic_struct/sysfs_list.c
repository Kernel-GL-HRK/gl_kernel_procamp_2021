// SPDX-License-Identifier: GPL-2.0-only

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/types.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Victor Krasnoshchok <ct3da21164@protonmail.ch>");
MODULE_DESCRIPTION("Interface to a linked list of strings through sysfs.");
MODULE_VERSION("0.1");

/* ------------------------------------------ */
/* List-related logic and data manipulation.  */

struct data_item {
	struct list_head list_node;
	char *payload;
	size_t payload_len;
};

static struct list_head root_node = { NULL, NULL };

static ssize_t add_node(struct list_head *parent, const char *node_data,
			size_t node_data_len)
{
	struct data_item *new_item;
	size_t payload_len;

	if (parent == NULL || node_data == NULL)
		return -EINVAL;

	new_item = kmalloc(sizeof(struct data_item), GFP_KERNEL);
	if (new_item == NULL)
		return -ENOMEM;

	payload_len = node_data_len + 1; /* Including \0 */
	new_item->payload = kzalloc(payload_len, GFP_KERNEL);
	if (new_item->payload == NULL) {
		pr_err("Failed to alloc. %lu bytes for payload.", payload_len);
		kfree(new_item);
		return -ENOMEM;
	}

	strncpy(new_item->payload, node_data, node_data_len);
	new_item->payload_len = node_data_len;

	list_add_tail(&new_item->list_node, parent);

	/* No extra newline needed here - echo appends NLs by itself */
	pr_info("Node added: %s", new_item->payload);
	return node_data_len;
}

static ssize_t serialize_list(struct list_head *list_root, char *output)
{
	ssize_t total_len = 0;
	char *wr_ptr = output;
	struct list_head *curr_node;
	struct data_item *curr_data;

	if (output == NULL)
		return -EINVAL;

	list_for_each(curr_node, list_root) {
		curr_data = list_entry(curr_node, struct data_item, list_node);
		strncpy(wr_ptr, curr_data->payload, curr_data->payload_len);
		wr_ptr += curr_data->payload_len;
		total_len += curr_data->payload_len;
	}

	return total_len;
}

static void destroy_list(struct list_head *list_root)
{
	struct data_item *curr_data_item;
	struct data_item *placeholder;

	list_for_each_entry_safe(curr_data_item, placeholder, list_root,
				  list_node) {
		kfree(curr_data_item->payload);
		list_del(&curr_data_item->list_node);
		kfree(curr_data_item);
	}

	pr_info("The list is%sempty\n",
			(list_empty_careful(list_root) ? " " : " NOT "));
}

/* ------------------------------------------ */
/* Generic module context.                    */

static ssize_t mod_sysfs_show(struct kobject *kobj, struct kobj_attribute *attr,
			      char *buf)
{
	(void)kobj;
	(void)attr;

	return serialize_list(&root_node, buf);
}

static ssize_t mod_sysfs_store(struct kobject *kobj,
			       struct kobj_attribute *attr, const char *buf,
			       size_t count)
{
	(void)kobj;
	(void)attr;

	return add_node(&root_node, buf, count);
}

static struct module_context {
	struct kobj_attribute mod_attr;
	struct kobject *mod_kobj;
} mod_sysfs_ctx = { __ATTR(list, 0644, mod_sysfs_show, mod_sysfs_store), NULL };

static int module_context_init(struct module_context *ctx)
{
	int ret;

	if (ctx == NULL)
		return -EINVAL;

	if (ctx->mod_kobj != NULL) {
		pr_err("Context has been already initialized.");
		return -EEXIST;
	}

	ctx->mod_kobj = kobject_create_and_add("mod_sysfs", kernel_kobj);
	if (ctx->mod_kobj == NULL)
		return -ENOMEM;

	ret = sysfs_create_file(ctx->mod_kobj, &ctx->mod_attr.attr);
	if (ret) {
		kobject_put(ctx->mod_kobj);
		ctx->mod_kobj = NULL;
	}

	return ret;
}

static void module_context_cleanup(struct module_context *ctx)
{
	if (ctx->mod_kobj != NULL)
		kobject_put(ctx->mod_kobj);
}

static int __init mod_sysfs_init(void)
{
	int ret = module_context_init(&mod_sysfs_ctx);

	if (ret != 0) {
		pr_err("Ctx init. failed: %d", ret);
		return ret;
	}

	INIT_LIST_HEAD(&root_node);
	return 0;
}

static void __exit mod_sysfs_exit(void)
{
	destroy_list(&root_node);
	module_context_cleanup(&mod_sysfs_ctx);
}

module_init(mod_sysfs_init);
module_exit(mod_sysfs_exit);

