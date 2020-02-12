#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

static int __init hello_init(void)
{
    printk("==========Hello Module=========\n");
	return 0;
}
module_init(hello_init);

static void __exit hello_exit(void)
{
    printk("==========Goodbye Module=========\n");
}
module_exit(hello_exit);

MODULE_LICENSE("GPL v2");
