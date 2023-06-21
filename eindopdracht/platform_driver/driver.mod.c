#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x516e49f9, "module_layout" },
	{ 0x1e073ce3, "sysfs_remove_file_ns" },
	{ 0x5048f746, "i2c_del_driver" },
	{ 0x8cc426ce, "sysfs_create_file_ns" },
	{ 0x84dc8cc8, "i2c_register_driver" },
	{ 0x7c1318e3, "device_create" },
	{ 0xebdab104, "__class_create" },
	{ 0x8b06ea8d, "devm_kmalloc" },
	{ 0x7c32d0f0, "printk" },
	{ 0x3df0823f, "class_destroy" },
	{ 0xaf87d05c, "device_destroy" },
	{ 0x91715312, "sprintf" },
	{ 0x29f2af1, "i2c_smbus_read_word_data" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("i2c:ard");
