#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>

static char m_kbuf[128] = {'a', 'b', 'c', '\0', -1};

static int proc_file_open(struct inode *inode, struct file *file)
{
    printk("Open Operation!\n");
    return 0;
}

static int proc_file_read(struct file *file, char __user *buf, 
    size_t size, loff_t *ppos)
{
    printk("Read Operation!\n");
    copy_to_user(buf, m_kbuf, strlen(m_kbuf));
    return strlen(buf);
}

static int proc_file_write(struct file *file, const  char __user *buf, 
    size_t count, loff_t *ppos)
{
    printk("Write Operation!\n");
    copy_from_user(m_kbuf, buf, strlen(buf));
    return strlen(buf);
}

int proc_file_release(struct inode *inode, struct file *file)
{
    printk("Release Operation!\n");
    return 0;
}

static const struct file_operations proc_file_ops = {
    .open = proc_file_open,
    .read = proc_file_read,
    .write = proc_file_write,
    .release = proc_file_release,
};

static int __init proc_file_init(void)
{
    printk("==========Hello Proc file Module=========\n");
    proc_create_data("my_proc_file", 0644, NULL, &proc_file_ops, NULL);
    return 0;
}

static void __exit proc_file_exit(void)
{
    printk("==========Goodbye Proc file Module=========\n");
    remove_proc_entry("my_proc_file", NULL);
}

module_init(proc_file_init);
module_exit(proc_file_exit);

MODULE_LICENSE("GPL v2");
