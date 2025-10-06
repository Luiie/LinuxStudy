#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/spinlock.h> // For read-write locks
#include <linux/uaccess.h>  // For copy_to_user/copy_from_user

#define HANIX_DEVICE_NAME "hanix-device"
#define HANIX_MAJOR_NUMBER 177
#define HANIX_BUFFER_SIZE 32

// Define and initialize a read-write lock
static DEFINE_RWLOCK(hanixDevice_RWLock);

// Kernel buffer to hold data for the device (32 bytes, initialized to 0)
static char hanuixDevice_buffer[HANIX_BUFFER_SIZE] = {0,};

static int hanuixDevice_open(struct inode *inode, struct file *fp)
{
    // Get the Minor number from the inode structure
    int minor = iminor(inode);
    printk(KERN_DEBUG "%s - minor : %d\n", __func__, minor); 
    
    return 0;
}

// __user: attribute, this pointer points to user-space data
static ssize_t hanuixDevice_read(struct file *fp, char __user *buf, size_t len, loff_t *ppos)
{
    int writtenBytes = 0;
    
    read_lock(&hanixDevice_RWLock);
    
    if (HANIX_BUFFER_SIZE <= len + *ppos) {
        len = HANIX_BUFFER_SIZE - *ppos;
    }
    // Copy data from the kernel buffer (comento_device_buf) to the user buffer
    writtenBytes = len - copy_to_user(buf, hanuixDevice_buffer + *ppos, len);
    *ppos += writtenBytes;
    
    read_unlock(&hanixDevice_RWLock);
    
    return writtenBytes;
}

// const: Not be modified by the Kernel
static ssize_t hanuixDevice_write(struct file *fp, const char __user *buf, size_t len, loff_t *ppos)
{
    int readBytes = 0;

    write_lock(&hanixDevice_RWLock);

    if (HANIX_BUFFER_SIZE <= len + *ppos) {
        len = HANIX_BUFFER_SIZE - *ppos;
    }
    // Copy data from the user buffer (*buf) to the kernel buffer
    readBytes = len - copy_from_user(hanuixDevice_buffer + *ppos, buf, len);
    *ppos += readBytes;

    write_unlock(&hanixDevice_RWLock);

    return readBytes;
}

static struct file_operations hanuixDevice_fileOperations = {
    .open = hanuixDevice_open,
    .read  = hanuixDevice_read,
    .write = hanuixDevice_write,
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
