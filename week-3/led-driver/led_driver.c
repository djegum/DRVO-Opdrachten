#include <linux/init.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/uaccess.h>

/* PIN */
#define PIN 19
/* base address */
#define GPIO1_ADDR 0x4804c000
/* register offsets in uint32_t sizes */
#define GPIO_OE 0x4D // 0x134
#define GPIO_DATAIN 0x4E // 0x138
#define GPIO_CLEARDATAOUT 0x64 // 0x190
#define GPIO_SETDATAOUT 0x65 // 0x194
/* max size in bytes */
#define GPIO_MAX 0x198

#define MAX_MINORS 1

MODULE_LICENSE("Dual BSD/GPL");

struct led_driver_device_data {
	struct cdev cdev;
	char buffer[256];
	int minor;
};

static const char driver_name[] = "led_driver";
struct led_driver_device_data devs[MAX_MINORS];
dev_t dev = 0;
static unsigned int count = 0;

uint32_t* gpio1;
uint32_t oe;

static int led_driver_open(struct inode *inode, struct file *file){
	count++;
	printk("led_driver_open(): count %d\n", count);
	struct led_driver_device_data *my_data =
             container_of(inode->i_cdev, struct led_driver_device_data, cdev);

    file->private_data = my_data;
	return 0;
}

static int led_driver_release(struct inode *inode, struct file *file){
	printk("led_driver_release()\n");
	return 0;
}

static ssize_t led_driver_read(struct file *file, char __user * user_buffer, size_t size, loff_t * offset){
	printk("led_driver_read()\n");

	struct led_driver_device_data *my_data = (struct led_driver_device_data *) file->private_data;
	ssize_t len = min(256 - *offset, size);

	if (len <= 0){
		return 0;
	}

    // if (user_buffer[0] == '0'){
    //     printk(KERN_INFO "LED OFF\n");
    // }else if(user_buffer[0] == '1'){
    //     printk(KERN_INFO "LED ON\n");
    // }else{
    //     printk(KERN_INFO "LED UNKNOWN\n");
    // }

	if (copy_to_user(user_buffer, my_data->buffer + *offset, len)){
		return -EFAULT;
	}

	*offset += len;
	return len;
}

static ssize_t led_driver_write(struct file *file, const char __user *user_buffer, size_t size, loff_t * offset){
	printk("led_driver_write()\n");

	struct led_driver_device_data *my_data = (struct led_driver_device_data *) file->private_data;
	ssize_t len = min(256 - *offset, size);

	if (len <= 0){
		return 0;
	}

	if (copy_from_user(my_data->buffer + *offset, user_buffer, len)){
		return -EFAULT;
	}


	gpio1 = ioremap( GPIO1_ADDR, GPIO_MAX * sizeof(uint32_t) );
    barrier();
    oe = ioread32( gpio1 + GPIO_OE );
    rmb();
    iowrite32( (oe & (~(1<<PIN))), gpio1 + GPIO_OE );
    wmb(); // write memory barrier
    iounmap(gpio1);

	gpio1 = ioremap(GPIO1_ADDR, GPIO_MAX);
	long input = -1;
	int ret = kstrtol(my_data->buffer, 0, &input);
	printk(KERN_INFO "%d\n", input);
	barrier();
	if(input == 1){
    	iowrite32( (1<<PIN), gpio1 + GPIO_SETDATAOUT ); // Pin 19 aan
		printk(KERN_INFO "ON\n");
	}else if(input == 0){
    	iowrite32( (1<<PIN), gpio1 + GPIO_CLEARDATAOUT ); // Pin 19 uit
		printk(KERN_INFO "OFF\n");
	}
	wmb(); // write memory barrier
	iounmap(gpio1);

	*offset += len;
	return len;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = led_driver_read,
	.write = led_driver_write,
	.open = led_driver_open,
	.release = led_driver_release
};

static int led_driver_init(void) {
    if((alloc_chrdev_region(&dev, 0, MAX_MINORS, driver_name)) < 0){
		printk(KERN_INFO "led_driver: Error while initing driver");
		return -1;
	}

	printk(KERN_INFO "led_driver: Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));
	printk(KERN_INFO "led_driver: Max minors: %d\n", MAX_MINORS);

    int i;
	for(i = 0; i < MAX_MINORS; i++){
		cdev_init(&devs[i].cdev, &fops);
		cdev_add(&devs[i].cdev, MKDEV(MAJOR(dev), i), 1);
		devs[i].minor = i;
	}

	/* output instellen */
	gpio1 = ioremap( GPIO1_ADDR, GPIO_MAX * sizeof(uint32_t) );
	barrier();
	oe = ioread32( gpio1 + GPIO_OE );
	rmb();
	iowrite32( (oe & (~(1<<PIN))), gpio1 + GPIO_OE );
	wmb(); // write memory barrier
	iounmap(gpio1);
	/* ledje aan en uit zetten */
	gpio1 = ioremap(GPIO1_ADDR, GPIO_MAX);
	barrier();
	// iowrite32( (1<<PIN), gpio1 + GPIO_SETDATAOUT ); // Pin 19 aan
	iowrite32( (1<<PIN), gpio1 + GPIO_CLEARDATAOUT ); // Pin 19 uit
	wmb(); // write memory barrier
	iounmap(gpio1);

	printk(KERN_INFO "led_driver: Kernel module inserted...");
    return 0;
}

static void led_driver_exit(void) {
    int i;
	for(i = 0; i < MAX_MINORS; i++){
		cdev_del(&devs[i].cdev);
	}

	unregister_chrdev_region(dev, MAX_MINORS);

	printk(KERN_INFO "led_driver: Goodbye world\n");
}

module_init(led_driver_init);
module_exit(led_driver_exit);