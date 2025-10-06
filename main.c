#include <linux/module.h>

static int __init hanix_module_init(void)
{
    printk(KERN_DEBUG "%s\n", __func__);
    return 0;
}

static void __exit hanix_module_exit(void)
{
    printk(KERN_DEBUG "%s\n", __func__);
}

module_init(hanix_module_init);
module_exit(hanix_module_exit);


MODULE_AUTHOR("Luiie<dbsldkfls@naver.com>");
MODULE_DESCRIPTION("An hanix example driver");
MODULE_LICENSE("GPL v2");  
