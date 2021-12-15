/*
 * Simple ProcFS module
 */
//#define pr_fmt(fmt) "%s: " fmt,  KBUILD_MODNAME
#include <linux/init.h>
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/device.h>
#include <linux/err.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define MOD_AUTHOR	"Volodymyr Kniazkyi <kniazkyi@gmail.com>"
MODULE_AUTHOR(MOD_AUTHOR);
MODULE_DESCRIPTION("Simple ProcFS module");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

#define DIR_NAME		"my_dir"
#define RD_FILE_NAME	"rd_file"
#define WR_FILE_NAME	"wr_file"
#define INF_FILE_NAME	"my_info"

static char msg[200];

size_t	read_counter;
size_t	write_counter;

static ssize_t rd_readcounter(struct file *file, char __user *pbuf,
						size_t count, loff_t *ppos)
{
	ssize_t num, not_copied;
	static ssize_t msg_len = 1;

	if (msg_len == 1) {
		read_counter++;
		msg_len = sprintf(msg, "You tried to read %d times\n", read_counter);
	}

	num = min_t(ssize_t, msg_len, count);
	if (num) {
		not_copied = copy_to_user((void __user *)pbuf, msg, num);
		num  -= not_copied;
	}

	if (num != 0)
		msg_len = 0;
	else
		msg_len = 1;

	return num;
}

static ssize_t read_info(struct file *file, char __user *pbuf,
						size_t count, loff_t *ppos)
{
	ssize_t num, not_copied;
	static ssize_t msg_len = 1;

	if (msg_len == 1) {
		msg_len = sprintf(msg, "Module name %s version %s\n",
		 					THIS_MODULE->name, 
							THIS_MODULE->version);
		msg_len += sprintf(msg+msg_len, "Module author %s\n", MOD_AUTHOR);
	}

	num = min_t(ssize_t, msg_len, count);
	if (num) {
		not_copied = copy_to_user((void __user *)pbuf, msg, num);
		num  -= not_copied;
	}

	if (num != 0)
		msg_len = 0;
	else
		msg_len = 1;

	return num;
}

static ssize_t rd_writecounter(struct file *file, char __user *pbuf,
						size_t count, loff_t *ppos)
{
	ssize_t num, not_copied;
	static ssize_t msg_len = 1;

	if (msg_len == 1)
		msg_len = sprintf(msg, "You tried to write %d times\n", write_counter);

	num = min_t(ssize_t, msg_len, count);
	if (num) {
		not_copied = copy_to_user((void __user *)pbuf, msg, num);
		num  -= not_copied;
	}

	if (num != 0)
		msg_len = 0;
	else
		msg_len = 1;

	return num;
}

static ssize_t inc_writecounter(struct file *file, const char __user *pbuf,
						size_t count, loff_t *ppos)
{
	write_counter++;
	return count;
}

static struct proc_ops ops_read = {
	.proc_read = rd_readcounter,
};

static struct proc_ops ops_info = {
	.proc_read = read_info,
};

static struct proc_ops ops_write = {
	.proc_read = rd_writecounter,
	.proc_write = inc_writecounter,
};

static struct proc_dir_entry *dir;
static struct proc_dir_entry *read_f, *write_f, *info_f;


static int procfs_module_init(void)
{
	int res = 0;

	read_counter = 0;
	write_counter = 0;

	dir = proc_mkdir(DIR_NAME, NULL);
	if (dir == NULL) {
		pr_info("procfs_module: Can not create procfs directory\n");
		return -ENOMEM;
	}
	read_f = proc_create(RD_FILE_NAME, 0444, dir, &ops_read);
	if (read_f == NULL) {
		pr_info("procfs_module: Can not create procfs entry %s\n", RD_FILE_NAME);
		remove_proc_entry(DIR_NAME, NULL);
		return -ENOMEM;
	}

	info_f = proc_create(INF_FILE_NAME, 0444, dir, &ops_info);
	if (info_f == NULL) {
		pr_info("procfs_module: Can not create procfs entry %s\n", INF_FILE_NAME);
		remove_proc_entry(RD_FILE_NAME, dir);
		remove_proc_entry(DIR_NAME, NULL);
		return -ENOMEM;
	}

	write_f = proc_create(WR_FILE_NAME, 0666, dir, &ops_write);
	if (write_f == NULL) {
		pr_info("procfs_module: Can not create procfs entry %s\n", WR_FILE_NAME);
		remove_proc_entry(INF_FILE_NAME, dir);
		remove_proc_entry(RD_FILE_NAME, dir);
		remove_proc_entry(DIR_NAME, NULL);
		return -ENOMEM;
	}

	pr_info("module loaded\n");
	return res;
}

static void procfs_module_exit(void)
{
	proc_remove(dir);
	pr_info("module exited\n");
}

module_init(procfs_module_init);
module_exit(procfs_module_exit);
