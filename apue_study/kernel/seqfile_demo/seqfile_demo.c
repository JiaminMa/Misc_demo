#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>

#define MAX_NUM 5

static void *ct_seq_start(struct seq_file *s, loff_t *pos)
{
    loff_t *spos = kmalloc(sizeof(loff_t), GFP_KERNEL);
    printk("%s pos:%d\n", __func__, *pos);
    if (*pos > MAX_NUM) {
        return NULL;
    }

    if (!*spos) {
        return NULL;
    }
    *spos = *pos;
    return spos;
}

static void *ct_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
    printk("%s\n", __func__);
    loff_t *spos = (loff_t *)v;
    *pos = ++(*spos);
    if (*pos > MAX_NUM) {
        return NULL;
    } 
    return spos;
}

static void ct_seq_stop(struct seq_file *s, void *v)
{
    printk("%s\n", __func__);
    kfree(v);
}

static int ct_seq_show(struct seq_file *s, void *v)
{
    printk("%s\n", __func__);
    loff_t *spos = (loff_t *)v;
    seq_printf(s, "%lld\n", *spos);
    return 0;
}

static const struct seq_operations ct_seq_ops = {
    .start = ct_seq_start,
    .next = ct_seq_next,
    .stop = ct_seq_stop,
    .show = ct_seq_show
};

static int ct_open(struct inode *inode, struct file *file){
    printk("ct_open!\n");
    return seq_open(file, &ct_seq_ops);
}

static const struct file_operations ct_file_ops = 
{
    .owner = THIS_MODULE,
    .open = ct_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = seq_release
};

static int ct_init(void)
{
    printk("%s\n", __func__);
    proc_create_data("seqfile_demo", 0644, NULL, &ct_file_ops, NULL);
    return 0;
}

static void ct_exit(void)
{
    printk("%s\n", __func__);
    remove_proc_entry("seqfile_demo", NULL);
}

module_init(ct_init);
module_exit(ct_exit);

MODULE_LICENSE("GPL");

