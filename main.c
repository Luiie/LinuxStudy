#include <linux/module.h>
#include <linux/fs.h>

#define HANIX_DEVICE_NAME "hanix-device"
#define HANIX_MAJOR_NUMBER 177

static int hanuixDevice_open(struct inode *inode, struct file *fp)
{
    // Get the Minor number from the inode structure
    int minor = iminor(inode);
    printk(KERN_DEBUG "%s - minor : %d\n", __func__, minor); 
    
    return 0;
}

static struct file_operations hanuixDevice_fileOperations = {
    .open = hanuixDevice_open,
};

static int __init hanixModule_init(void)
{
    printk(KERN_DEBUG "%s\n", __func__);
    return register_chrdev(HANIX_MAJOR_NUMBER,
                           HANIX_DEVICE_NAME,
                           &hanuixDevice_fileOperations);
}

static void __exit hanixModule_exit(void)
{
    printk(KERN_DEBUG "%s\n", __func__);
    unregister_chrdev(HANIX_MAJOR_NUMBER, HANIX_DEVICE_NAME);
}

module_init(hanixModule_init);
module_exit(hanixModule_exit);


MODULE_AUTHOR("Luiie<dbsldkfls@naver.com>");
MODULE_DESCRIPTION("An hanix example driver");
MODULE_LICENSE("GPL v2");  
