#include <linux/init.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

MODULE_LICENSE("Dual BSD/GPL");

int major = 500;
int minor = 0;
static const int amount = 1;

static const char driver_name[] = "hello_driver";

static int param_1 = 0;
static int param_2 = 0;

module_param_named(param1, param_1, int, S_IRUGO);
module_param_named(param2, param_2, int, S_IRUGO);

static int hello_open(struct inode *inode, struct file *file){
	printk(KERN_ALERT "hello_open()\n");
	return 0;
}

static int hello_release(struct inode *inode, struct file *file){
	printk(KERN_ALERT "hello_release()\n");
	return 0;
}

static ssize_t hello_read(struct file *file, char __user * buf, size_t lbuf, loff_t * ppos){
	printk(KERN_ALERT "hello_read()\n");
	return 0;	
}

static ssize_t hello_write(struct file *file, const char __user * buf, size_t lbuf, loff_t * ppos){
	printk(KERN_ALERT "hello_write()\n");
	return lbuf;	
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = hello_read,
	.write = hello_write,
	.open = hello_open,
	.release = hello_release
};

static struct cdev* device;

dev_t dev = 0;

static int hello_init(void){
	if((alloc_chrdev_region(&dev, 0, 1, driver_name)) < 0){
		printk(KERN_INFO "Error while initing driver");
		return -1;
	}

	major = MAJOR(dev);
	minor = MINOR(dev);

	printk(KERN_INFO "Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));
	printk(KERN_INFO "Kernel module inserted...");
	return 0;
}

static void hello_exit(void){
	dev_t device_number;
	device_number = MKDEV(major, minor);
	unregister_chrdev_region(device_number, amount);
	printk(KERN_ALERT "Goodbye world\n");
}

module_init(hello_init);
module_exit(hello_exit);
