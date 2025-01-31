#include <linux/kernel.h>
#include <linux/kern_levels.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/module.h>

#include "ioct_test.h"


MODULE_LICENSE("GPL");

#define ECE_BUF_SIZE 256

//static char ece_buffer[ECE_BUF_SIZE];
int isReg;
int major;
int ece_offset_w;
int ece_offset_r;
int ece_size;

char an[10];

static char an1[10] = "beagle";
static char an2[10] = "cat";
static char an3[10] = "parrot";
static char an4[10] = "fish";
static char an5[10] = "row boat";
static char an6 [10] = "program";
static char an7[10] = "kernel";


int ece_init(void);
void ece_end(void);
static ssize_t ece_write(struct file*, const char*, size_t, loff_t*);
static ssize_t ece_read(struct file*, char*, size_t, loff_t*);
static int ece_open(struct inode*, struct file*);
static int ece_close(struct inode*, struct file*);
static long int ece_ioctl(struct file*, unsigned cmd, unsigned long arg);
static struct file_operations ece_fops = 
{
    .read = ece_read,
    .write = ece_write,
    .open = ece_open,
    .release = ece_close,
    .unlocked_ioctl = ece_ioctl,
};

int ece_init(void)
{
    major = register_chrdev(0, "seminar4", &ece_fops);
    ece_offset_w = 0;
    ece_offset_r = 0;
    ece_size = 0;

    if(major < 0) {
        isReg = 0;
        printk(KERN_INFO " ECE4310: Start FAIL\n");
    } else {
        isReg = 1;
        printk(KERN_INFO "ECE4310: Start here \n");
    }

    return 0;
}

void ece_end(void)
{
    if(isReg){
        unregister_chrdev(major, "seminar_module");
    }
    printk(KERN_INFO " ECE4310: End here\n");

}

static ssize_t ece_write(struct file *fp, const char *buf, size_t count, loff_t* offset)
{
    int ret = 0;
    
    if(ece_offset_w +count >= ECE_BUF_SIZE)
    {
        printk(KERN_INFO" ECE4310: Write overflow. Abort. \n");
        printk(KERN_INFO" The current count is %d\n", (int)(ece_offset_w + count));
        return -1;
    }

    printk(KERN_INFO " ECE4310: Copy from user. \n");
    ret=copy_from_user(&an[ece_offset_w], buf, count);
    if(ret != 0)
    {
        printk(KERN_INFO "ECE4310: ERR copy from user. \n");
        return -1;
    }
    

    return count;
}

static ssize_t ece_read(struct file *fp, char *buf, size_t count, loff_t* offset)
{
    
    int ret;
    if(ece_offset_r + count >= ECE_BUF_SIZE)
    {
        printk(KERN_INFO "ECE 4310 Read overflow. Abort. \n");
        return -1;
    }
    printk(KERN_INFO "ECE4310: Copy to user. \n");
    ret=copy_to_user(buf,&an[ece_offset_r],count);
    if(ret!=0)
    {
        printk(KERN_INFO " ECE 4310: ERR Copy to user. \n");
        return -1;
    }

    

    return count;
}

static int ece_open(struct inode*, struct file*)
{   
    printk(KERN_INFO "Seminar4 Module has been opened! \n");

    
    return 0;

}

static int ece_close(struct inode*, struct file*)
{   
    ece_offset_w = 0;
    ece_offset_r = 0;

    printk(KERN_INFO "Seminar4 Module has been closed! \n");

    return 0;
    
}
int32_t rand_num;
static long int ece_ioctl(struct file*, unsigned cmd, unsigned long arg)
{
    switch (cmd) {
        case WR_VALUE:
            if(copy_from_user(&rand_num, (int32_t *) arg, sizeof(rand_num)))
                printk(KERN_INFO"Error copying data using ioctl\n");
            
            printk(KERN_INFO"Copied random number data from user : %d\n Now generating random word\n", rand_num);
            
            switch ((int)rand_num)
            {
                case 1:
                    for (int i = 0; i<10; i++){
                        an[i] = an1[i];
                    }
                    break;
                case 2:
                    for (int i = 0; i<10; i++){
                        an[i] = an2[i];
                    }
                    break;
                case 3:
                    for (int i = 0; i<10; i++){
                        an[i] = an3[i];
                    }
                    break;
                case 4:
                    for (int i = 0; i<10; i++){
                        an[i] = an4[i];
                    }
                    break;
                case 5:
                    for (int i = 0; i<10; i++){
                        an[i] = an5[i];
                    }
                    break;
                case 6:
                    for (int i = 0; i<10; i++){
                        an[i] = an6[i];
                    }
                    break;
                case 7:
                    for (int i = 0; i<10; i++){
                        an[i] = an7[i];
                    }
                    break;
    
                default:
                    for (int i = 0; i<10; i++){
                        an[i] = an1[i];
                    }
                    break;
            }

            break;
        case RD_VALUE:
            if(copy_to_user((int32_t *) arg, &rand_num, sizeof(rand_num)))
                printk(KERN_INFO"Error copying datato user using ioctl\n");
            printk(KERN_INFO"Copied data to user : %d\n", rand_num);

            break;
            

    }
    return 0;
}

module_init(ece_init);
module_exit(ece_end);
