#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/of.h>
#include <linux/of_device.h>

#define DRIVER_NAME "ard"

static int my_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    // Device initialization and setup code goes here
    pr_info("Probing I2C device: %s with address 0x%02x\n", client->name, client->addr);

    uint16_t data = i2c_smbus_read_word_data(client, 0xE3);
    int value = (int)(((176 * data) / 65536) - 47);
    pr_info("data read: 0x%02x\n", data);
    pr_info("temprature is: %dC\n", value);

    return 0; // Success
}

static int my_i2c_remove(struct i2c_client *client)
{
    // Device cleanup code goes here
    pr_info("Removing I2C device: %s\n", client->name);

    // ...

    return 0;
}

static const struct i2c_device_id my_i2c_id[] = {
    { "my_i2c_device", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, my_i2c_id);

static const struct of_device_id my_i2c_of_match[] = {
    { .compatible = "ard" },
    { }
};
MODULE_DEVICE_TABLE(of, my_i2c_of_match);

static struct i2c_driver my_i2c_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = of_match_ptr(my_i2c_of_match),
    },
    .probe = my_i2c_probe,
    .remove = my_i2c_remove,
    .id_table = my_i2c_id,
};

static int __init my_i2c_init(void)
{
    return i2c_add_driver(&my_i2c_driver);
}

static void __exit my_i2c_exit(void)
{
    i2c_del_driver(&my_i2c_driver);
}

module_init(my_i2c_init);
module_exit(my_i2c_exit);

MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("I2C driver for my device");
MODULE_LICENSE("GPL");
