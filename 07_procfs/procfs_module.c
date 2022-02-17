#define pr_fmt(fmt) "%s: " fmt, KBUILD_MODNAME
#define DIR_NAME "my_folder"
#define AUTHOR_ENTRY "author"
#define WRITE_ENTRY "write"
#define READ_ENTRY "read"
#define MOD_AUTHOR "Artem Shyshko"
#define MOD_LICENSE "GPL"

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/err.h>
#include <linux/uaccess.h>
#include <linux/string.h>

static char output_msg[PAGE_SIZE];
static ssize_t msg_size;
static int count_read = 0, count_write = 0;

static ssize_t file_read(struct file *file, char __user *pbuf, size_t count, loff_t *ppos)
{
    static bool is_read = true;

    if (is_read)
    {
        char* file_name = file->f_path.dentry->d_iname;
        count_read++;
        if (!strcmp(file_name, WRITE_ENTRY))
        {
            sprintf(output_msg, "Write calls = %d\n", count_write);
            msg_size = strlen(output_msg);
        }
        else if (!strcmp(file_name, READ_ENTRY))
        {
            sprintf(output_msg, "Read calls = %d\n", count_read);
            msg_size = strlen(output_msg);
        }
        else if (!strcmp(file_name, AUTHOR_ENTRY))
        {
            sprintf(output_msg, "Author's name is %s\n", MOD_AUTHOR);
            msg_size = strlen(output_msg);
        }
        else
        {
            sprintf(output_msg, "Select an entry\n");
            msg_size = strlen(output_msg);
        }
        copy_to_user(pbuf, output_msg, msg_size);
        is_read = false;
        return msg_size;
    }

    is_read = true;
    return 0;
}

static ssize_t file_write(struct file *file, const char __user *pbuf, size_t count, loff_t *ppos)
{
    count_write++;
    return count;
}

static struct proc_ops operations = 
{
    .proc_read = file_read,
    .proc_write = file_write,
};

static struct proc_dir_entry *dir;
static struct proc_dir_entry *ent_auth, *ent_write, *ent_read;


static int proc_init(void)
{
    dir = proc_mkdir(DIR_NAME, NULL);
    if (dir == NULL)
    {
        pr_err("Error creating procfs directory\n");
        return -ENOMEM;
    }

    ent_auth = proc_create(AUTHOR_ENTRY, 0666, dir, &operations);
    if (ent_auth == NULL)
    {
        remove_proc_entry(DIR_NAME, NULL);
        pr_err("Error creating author's name entry\n");
        return -ENOMEM;
    }

    ent_write = proc_create(WRITE_ENTRY, 0666, dir, &operations);
    if (ent_auth == NULL)
    {
        remove_proc_entry(AUTHOR_ENTRY, dir);
        remove_proc_entry(DIR_NAME, NULL);
        pr_err("Error creating write entry\n");
        return -ENOMEM;
    }

    ent_read = proc_create(READ_ENTRY, 0666, dir, &operations);
    if (ent_auth == NULL)
    {
        remove_proc_entry(WRITE_ENTRY, dir);
        remove_proc_entry(AUTHOR_ENTRY, dir);
        remove_proc_entry(DIR_NAME, NULL);
        pr_err("Error creating read entry\n");
        return -ENOMEM;
    }

    pr_info("Module loaded\n");
    return 0;
}

static void proc_exit(void)
{
    proc_remove(dir);
    pr_info("Module exited\n");
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_AUTHOR(MOD_AUTHOR);
MODULE_LICENSE(MOD_LICENSE);
