/* External declaration */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h> // struct of_device_id
#include <linux/of.h> // of_match_ptr macro
#include <linux/ioport.h> // struct resource

MODULE_LICENSE("Dual BSD/GPL");

static const struct of_device_id g_ids[] = {
    { .compatible = "gpio-extern-jochem", },
    { } // ends with empty; MUST be last member
};

static int gpio_ex_probe(struct platform_device* dev) {
    printk("gpio_ex_probe(%s)\n", dev->name);
    return 0;
}

static int gpio_ex_remove(struct platform_device* dev) {
    printk("gpio_ex_remove()\n");
    return 0;
}

struct platform_driver g_driver = {
    .probe = gpio_ex_probe, // obliged
    .remove = gpio_ex_remove, // obliged
    // .shutdown // optional
    // .suspend // optional
    // .suspend_late // optional
    // .resume_early // optional
    // .resume // optional
    .driver = {
        .name = "gpio-extern-jochem", // name of the driver
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(g_ids), // compatible device id
    }
};

static int gpio_ex_init(void) {
    int result;
    result = platform_driver_register(&g_driver);
    printk("gpio_ex_init()\n");
    return result;
}

static void gpio_ex_exit(void) {
    printk("gpio_ex_exit()\n");
    platform_driver_unregister(&g_driver);
}

/* module init/exit functions */
module_init(gpio_ex_init);
module_exit(gpio_ex_exit);