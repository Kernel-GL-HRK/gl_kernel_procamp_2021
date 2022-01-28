/**
 * @file procfs_test.c
 * @brief Linux kernel module to test procfs
 * With this module you can read from /proc/procfs_test/ entities such information:
 *  1. module_author - author name
 *  2. rd_cnt - amount of read operations from /sys/kernel/str_storage/list
 *  3. wr_cnt - amount of write operations to  /sys/kernel/str_storage/list
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <asm/bug.h>

#define PROCFS_TEST_AUTHOR      "Sergey D. <serj.dub1na@gmail.com>"
#define PROCFS_DIR_NAME         "procfs_test"
#define PROCFS_RD_CNT_FILE_NAME "rd_cnt"
#define PROCFS_WR_CNT_FILE_NAME "wr_cnt"
#define PROCFS_AUTHOR_FILE_NAME "module_author"
#define PROCFS_MAX_SIZE 2048

struct node
{
	struct list_head list;
	const char *string;
};

struct my_object
{
	struct kobject *kobj;
	struct list_head strings_list;
	struct kobj_attribute kattr;
};

static ssize_t string_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf);

static ssize_t string_store(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count);

static struct my_object str_storage =
{
	.kattr = __ATTR(list, 0660, string_show, string_store),
};

static ssize_t procfs_my_read(struct file *file, char __user *pbuf,
	size_t count, loff_t *ppos);

static struct proc_ops procfs_test_read =
{
	.proc_read = procfs_my_read,
};

static struct proc_dir_entry *procfs_dir;
static struct proc_dir_entry *procfs_file_rd_cnt;
static struct proc_dir_entry *procfs_file_wr_cnt;
static struct proc_dir_entry *procfs_author_file;
/** PROCFS buffer */
static char procfs_buff[PROCFS_MAX_SIZE];
/** Flag indicating that read operaion has finished */
static int procfs_finished = 0;
static unsigned int m_read_count = 0;
static unsigned int m_write_count = 0;

static ssize_t string_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	struct node *entry;
	struct list_head *entity;
	struct list_head *tmp;
	ssize_t len = 0;

	(void)kobj;
	(void)attr;

	list_for_each_safe(entity, tmp, &str_storage.strings_list) {
		entry = list_entry(entity, struct node, list);

		BUG_ON(entry == NULL);
		BUG_ON(entry->string == NULL);

		const size_t entry_len = strlen(entry->string);
		const size_t tmp = strlcpy(&buf[len], entry->string, PAGE_SIZE - len);
		if (tmp != entry_len) {
			BUG();
			len = -ENOMEM;
			pr_err("[STR_STORAGE] String copy failed or PAGE_SIZE was reached\n");
			break;
		}

		len += tmp;
	}

	if (len > 0)
		pr_debug("[STR_STORAGE] String storage contains:\n%s", buf);

	++m_read_count;

	return len;
}

static ssize_t string_store(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count)
{
	struct node *str;

	(void)kobj;
	(void)attr;

	str = kmalloc(sizeof(struct node), GFP_KERNEL);
	if (str == NULL)
		return -ENOMEM;

	str->string = kstrdup_const(buf, GFP_KERNEL);
	if (str->string == NULL)
		return -ENOMEM;

	pr_debug("[STR_STORAGE] String %s stored\n", str->string);

	list_add_tail(&str->list, &str_storage.strings_list);

	++m_write_count;

	return count;
}

static ssize_t procfs_my_read(struct file *file, char __user *pbuf,
	size_t count, loff_t *ppos)
{
	const char *file_name = file->f_path.dentry->d_iname;
	/* The size of the data hold in the PROCFS buffer */
	unsigned long procfs_buff_size = 0;

	/* Return 0 to indicate end of file */
	if (procfs_finished) {
		pr_info("[PROCFS_TEST] Read finished\n");
		procfs_finished = 0;
		return 0;
	}

	if (strcmp(file_name, PROCFS_RD_CNT_FILE_NAME) == 0) {
		procfs_buff_size = snprintf(procfs_buff, PROCFS_MAX_SIZE - 1,
			"You read sysfs file %u times\n", m_read_count);
	}
	else if (strcmp(file_name, PROCFS_WR_CNT_FILE_NAME) == 0) {
		procfs_buff_size = snprintf(procfs_buff, PROCFS_MAX_SIZE - 1,
			"You wrote sysfs file %u times\n", m_write_count);
	}
	else if (strcmp(file_name, PROCFS_AUTHOR_FILE_NAME) == 0) {
		procfs_buff_size = snprintf(procfs_buff, PROCFS_MAX_SIZE - 1,
			"%s\n", PROCFS_TEST_AUTHOR);
	}
	else
		BUG();

	/* Plus 1 for '\0' */
	++procfs_buff_size;
	BUG_ON(procfs_buff_size > count);
	procfs_finished = 1;

	if (copy_to_user(pbuf, procfs_buff, procfs_buff_size)) {
		return -EFAULT;
	}

	pr_info("[PROCFS_TEST] Read %lu bytes\n", procfs_buff_size);
	/* Return the number of bytes "read" */
	return procfs_buff_size;
}

static int procfs_test_init(void)
{
	str_storage.kobj = kobject_create_and_add("str_storage", kernel_kobj);
	if (str_storage.kobj == NULL)
		return -ENOMEM;

	const int res = sysfs_create_file(str_storage.kobj,
		&str_storage.kattr.attr);
	if (res) {
		pr_err("[PROCFS_TEST] Failed to create sysfs file\n");
		kobject_put(str_storage.kobj);
		return res;
	}

	INIT_LIST_HEAD(&str_storage.strings_list);

	procfs_dir = proc_mkdir(PROCFS_DIR_NAME, NULL);
	if (procfs_dir == NULL) {
		pr_err("[PROCFS_TEST] Can not create procfs directory\n");
		return -ENOMEM;
	}

	procfs_file_rd_cnt = proc_create(PROCFS_RD_CNT_FILE_NAME, 0444, procfs_dir, &procfs_test_read);
	if (procfs_file_rd_cnt == NULL) {
		pr_err("[PROCFS_TEST] Error while creating procfs entry %s\n", PROCFS_RD_CNT_FILE_NAME);
		remove_proc_entry(PROCFS_DIR_NAME, NULL);
		return -ENOMEM;
	}

	procfs_file_wr_cnt = proc_create(PROCFS_WR_CNT_FILE_NAME, 0444, procfs_dir, &procfs_test_read);
	if (procfs_file_wr_cnt == NULL) {
		pr_err("[PROCFS_TEST] Can not create procfs entry %s\n", PROCFS_WR_CNT_FILE_NAME);
		remove_proc_entry(PROCFS_RD_CNT_FILE_NAME, procfs_dir);
		remove_proc_entry(PROCFS_DIR_NAME, NULL);
		return -ENOMEM;
	}

	procfs_author_file = proc_create(PROCFS_AUTHOR_FILE_NAME, 0444, procfs_dir, &procfs_test_read);
	if (procfs_author_file == NULL) {
		pr_err("[PROCFS_TEST] Can not create procfs entry %s\n", PROCFS_AUTHOR_FILE_NAME);
		remove_proc_entry(PROCFS_RD_CNT_FILE_NAME, procfs_dir);
		remove_proc_entry(PROCFS_WR_CNT_FILE_NAME, procfs_dir);
		remove_proc_entry(PROCFS_DIR_NAME, NULL);
		return -ENOMEM;
	}

	return res;
}

static void procfs_test_exit(void)
{
	struct node *entry;
	struct list_head *entity;
	struct list_head *tmp;

	list_for_each_safe(entity, tmp, &str_storage.strings_list) {
		entry = list_entry(entity, struct node, list);

		list_del(entity);

		BUG_ON(entry == NULL);
		BUG_ON(entry->string == NULL);

		kfree(entry->string);
		kfree(entry);
	}

	kobject_put(str_storage.kobj);
	str_storage.kobj = NULL;

	proc_remove(procfs_dir);
}

module_init(procfs_test_init);
module_exit(procfs_test_exit);

MODULE_DESCRIPTION("String Storage kernel module");
MODULE_AUTHOR(PROCFS_TEST_AUTHOR);
MODULE_LICENSE("GPL");
