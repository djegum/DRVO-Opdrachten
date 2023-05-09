#include <linux/init.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define MAX_MINORS 5

MODULE_LICENSE("Dual BSD/GPL");

static const char driver_name[] = "hello_driver";

struct hello_device_data {
	struct cdev cdev;
	char buffer[256];
	int minor;
};

struct hello_device_data devs[MAX_MINORS];

static int param_1 = 0;
static int param_2 = 0;

static unsigned int count = 0;

module_param_named(param1, param_1, int, S_IRUGO);
module_param_named(param2, param_2, int, S_IRUGO);

static int hello_open(struct inode *inode, struct file *file){
	count++;
	printk("hello_open(): count %d\n", count);
	struct hello_device_data *my_data =
             container_of(inode->i_cdev, struct hello_device_data, cdev);

    file->private_data = my_data;
	return 0;
}

static int hello_release(struct inode *inode, struct file *file){
	printk("hello_release()\n");
	return 0;
}

static ssize_t hello_read(struct file *file, char __user * user_buffer, size_t size, loff_t * offset){
	printk("hello_read()\n");

	struct hello_device_data *my_data = (struct hello_device_data *) file->private_data;
	ssize_t len = min(256 - *offset, size);

	if (len <= 0){
		return 0;
	}

	if (copy_to_user(user_buffer, my_data->buffer + *offset, len)){
		return -EFAULT;
	}

	*offset += len;
	return len;
}

static ssize_t hello_write(struct file *file, const char __user *user_buffer, size_t size, loff_t * offset){
	printk("hello_write()\n");

	struct hello_device_data *my_data = (struct hello_device_data *) file->private_data;
	ssize_t len = min(256 - *offset, size);

	if (len <= 0){
		return 0;
	}

	if (copy_from_user(my_data->buffer + *offset, user_buffer, len)){
		return -EFAULT;
	}

	*offset += len;
	return len;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = hello_read,
	.write = hello_write,
	.open = hello_open,
	.release = hello_release
};

dev_t dev = 0;

static int hello_init(void){
	if((alloc_chrdev_region(&dev, 0, MAX_MINORS, driver_name)) < 0){
		printk("Error while initing driver");
		return -1;
	}

	printk("Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));
	printk("Max minors: %d\n", MAX_MINORS);

	for(int i = 0; i < MAX_MINORS; i++){
		cdev_init(&devs[i].cdev, &fops);
		cdev_add(&devs[i].cdev, MKDEV(MAJOR(dev), i), 1);
		devs[i].minor = i;
	}

	printk("Kernel module inserted...");
	return 0;
}

static void hello_exit(void){
	for(int i = 0; i < MAX_MINORS; i++){
		cdev_del(&devs[i].cdev);
	}

	unregister_chrdev_region(dev, MAX_MINORS);
	printk("Goodbye world\n");
}

module_init(hello_init);
module_exit(hello_exit);
