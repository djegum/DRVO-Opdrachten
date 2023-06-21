#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <linux/kdev_t.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("I2C Platform Driver with /sys/class entry");

struct my_device_data {
    struct i2c_client *client;
};

static struct class *my_class;
static struct device *my_device;  // Global variable to store the device instance

static int my_device_probe(struct i2c_client *client,
                           const struct i2c_device_id *id)
{
    struct my_device_data *data;
    struct device *dev;

    // Allocate memory for your device data
    data = devm_kzalloc(&client->dev, sizeof(*data), GFP_KERNEL);
    if (!data)
        return -ENOMEM;

    // Store the device data within the I2C client data structure
    i2c_set_clientdata(client, data);

    // Create the class
    my_class = class_create(THIS_MODULE, "my_i2c_class");
    if (IS_ERR(my_class))
        return PTR_ERR(my_class);

    // Create the device
    dev = device_create(my_class, &client->dev, MKDEV(0, 0), data, "my_i2c_device");
    if (IS_ERR(dev)) {
        class_destroy(my_class);
        return PTR_ERR(dev);
    }

    my_device = dev;  // Store the device instance in the global variable
    data->client = client;

    pr_info("My I2C device probed\n");
    return 0;
}

static int my_device_remove(struct i2c_client *client)
{
    struct my_device_data *data = i2c_get_clientdata(client);

    // Destroy the device
    device_destroy(my_class, MKDEV(0, 0));

    // Destroy the class
    class_destroy(my_class);

    pr_info("My I2C device removed\n");
    return 0;
}

static const struct i2c_device_id my_device_id[] = {
    { "ard", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, my_device_id);

static struct i2c_driver my_device_driver = {
    .driver = {
        .name = "my_i2c_device",
        .owner = THIS_MODULE,
    },
    .probe = my_device_probe,
    .remove = my_device_remove,
    .id_table = my_device_id,
};

static ssize_t my_attribute_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct my_device_data *data = dev_get_drvdata(dev);
    struct i2c_client *client = data->client;

    uint16_t readout = i2c_smbus_read_word_data(client, 0xE3);
    int value = (int)(((176 * readout) / 65536) - 47);
    return sprintf(buf, "Temprature is %dC\n", value);
}

static ssize_t my_attribute_store(struct device *dev, struct device_attribute *attr,
                                  const char *buf, size_t count)
{
    return count;
}

static struct device_attribute dev_attr_my_attribute = {
    .attr = {
        .name = "my_attribute",
        .mode = S_IRUGO | S_IWUSR,
    },
    .show = my_attribute_show,
    .store = my_attribute_store,
};

static int __init my_device_init(void)
{
    int ret = i2c_add_driver(&my_device_driver);
    if (ret)
        return ret;

    // Create the sysfs attribute
    ret = sysfs_create_file(&my_device->kobj, &dev_attr_my_attribute.attr);
    if (ret) {
        i2c_del_driver(&my_device_driver);
        return ret;
    }

    pr_info("My I2C driver loaded\n");
    return 0;
}

static void __exit my_device_exit(void)
{
    sysfs_remove_file(&my_device->kobj, &dev_attr_my_attribute.attr);
    i2c_del_driver(&my_device_driver);
    pr_info("My I2C driver unloaded\n");
}

module_init(my_device_init);
module_exit(my_device_exit);
