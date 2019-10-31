#include <linux/module.h>     //最基本的头文件，其中定义了MODULE_LICENSE这一类宏
#include <linux/fs.h>		  //file_operations等结构体的定义
#include <linux/init.h>       //定义module_init等模块


#define  DEVICE_NAME  "first"   //设备名称
#define  MAJOR        55        //主设备号

//定义open函数
static int first_drv_open(struct inode *inode, struct file *file)
{
	printk("%s drive open\n",DEVICE_NAME);
	return 0;
}

//定义write函数
static ssize_t first_drv_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	printk("%s drive write\n",DEVICE_NAME);
	return 0;
}

//定义read函数
static ssize_t first_drv_read(struct file *filp, char __user *buff, 
                                         size_t count, loff_t *offp)
{
	printk("%s drive read\n",DEVICE_NAME);
	return 0;
}

/* 这个结构是字符设备驱动程序的核心
 * 当应用程序操作设备文件时所调用的open、read、write等函数，
 * 最终会调用这个结构中指定的对应函数
 */
static struct file_operations first_drv_fops = {
    .owner  =   THIS_MODULE,    /* 这是一个宏，推向编译模块时自动创建的__this_module变量 */
    .open   =   first_drv_open,     
	.read	=	first_drv_read,	   
	.write	=	first_drv_write,	   
};


/*
 * 执行insmod命令时就会调用这个函数 
 */
static int __init first_drv_init(void)
{
	/* 注册字符设备
     * 参数为主设备号、设备名字、file_operations结构；
     * 这样，主设备号就和具体的file_operations结构联系起来了，
     * 操作主设备为MAJOR的设备文件时，就会调用s3c24xx_leds_fops中的相关成员函数
     * MAJOR可以设为0，表示由内核自动分配主设备号
     */
	int ret;
	
	ret = register_chrdev(MAJOR, DEVICE_NAME, &first_drv_fops);
	if (ret < 0) {
      printk(DEVICE_NAME " can't register MAJOR number\n");
      return ret;
    }

	return 0;
}

/*
 * 执行rmmod命令时就会调用这个函数 
 */
static void __exit first_drv_exit(void)
{
    /* 卸载驱动程序 */
    unregister_chrdev(MAJOR, DEVICE_NAME);
	
	printk(DEVICE_NAME " exit success\n");

}

/* 这两行指定驱动程序的初始化函数和卸载函数 */
module_init(first_drv_init);
module_exit(first_drv_exit);

/* 描述驱动程序的一些信息，不是必须的 */
MODULE_AUTHOR("lss wechat qabc132321");
MODULE_VERSION("0.1.0");
MODULE_DESCRIPTION("S3C2410/S3C2440 LED Driver");
MODULE_LICENSE("GPL");





