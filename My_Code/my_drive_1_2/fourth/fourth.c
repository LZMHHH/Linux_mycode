#include <linux/module.h>     //最基本的头文件，其中定义了MODULE_LICENSE这一类宏
#include <linux/fs.h>		  //file_operations等结构体的定义
#include <linux/init.h>       //定义module_init等模块

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>
#include <linux/poll.h>


//用于自动创建设备节点的
static struct class *fourthdrv_class;
static struct class_device	*fourthdrv_class_dev;

volatile unsigned long *gpfcon;
volatile unsigned long *gpfdat;

volatile unsigned long *gpgcon;
volatile unsigned long *gpgdat;

#define  DEVICE_NAME  "fourth"   //设备名称

static DECLARE_WAIT_QUEUE_HEAD(button_waitq);

/* 中断事件标志, 中断服务程序将它置1，fourth_drv_read将它清0 */
static volatile int ev_press = 0;

struct pin_desc{
	unsigned int pin;
	unsigned int key_val;
};

/* 键值: 按下时, 0x01, 0x02, 0x03, 0x04 */
/* 键值: 松开时, 0x81, 0x82, 0x83, 0x84 */
static unsigned char key_val;

//dev_id,irqaction结构中的成员之一
struct pin_desc pins_desc[4] = {
	{S3C2410_GPF0, 0x01},
	{S3C2410_GPF2, 0x02},
	{S3C2410_GPG3, 0x03},
	{S3C2410_GPG11, 0x04},
};

//中断执行函数
static irqreturn_t buttons_irq(int irq, void *dev_id)
{
	struct pin_desc * pindesc = (struct pin_desc *)dev_id;
	unsigned int pinval;
	
	pinval = s3c2410_gpio_getpin(pindesc->pin);//当前引脚bit值
	
	if(pinval)
	{
		/* 松开 */
		key_val = 0x80 | pindesc->key_val;
	}
	else
	{
		/* 按下 */
		key_val = pindesc->key_val;
	}
	
	ev_press = 1;                           /* 标志中断发生了 */
    wake_up_interruptible(&button_waitq);   /* 唤醒休眠的进程 */
	
	return IRQ_RETVAL(IRQ_HANDLED);
}


//////////////////////////////////////////////////////////////////////////////
//定义open函数
static int fourth_drv_open(struct inode *inode, struct file *file)
{
	printk("%s drive open\n",DEVICE_NAME);
	
	/* 配置GPF0,2为输入引脚 */
	/* 配置GPG3,11为输入引脚 */
	request_irq(IRQ_EINT0,  buttons_irq, IRQT_BOTHEDGE, "S2", &pins_desc[0]);
	request_irq(IRQ_EINT2,  buttons_irq, IRQT_BOTHEDGE, "S3", &pins_desc[1]);
	request_irq(IRQ_EINT11, buttons_irq, IRQT_BOTHEDGE, "S4", &pins_desc[2]);
	request_irq(IRQ_EINT19, buttons_irq, IRQT_BOTHEDGE, "S5", &pins_desc[3]);	
	return 0;
}
//定义close函数
int fourth_drv_close(struct inode *inode, struct file *file)
{
	free_irq(IRQ_EINT0, &pins_desc[0]);
	free_irq(IRQ_EINT2, &pins_desc[1]);
	free_irq(IRQ_EINT11, &pins_desc[2]);
	free_irq(IRQ_EINT19, &pins_desc[3]);
	return 0;
}

//定义write函数
static ssize_t fourth_drv_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	printk("%s drive write\n",DEVICE_NAME);
	return 0;
}

//定义read函数
static ssize_t fourth_drv_read(struct file *file, char __user *buf, 
                                         size_t size, loff_t *ppos)
{
	if (size != 1)
		return -EINVAL;
	
	/* 如果没有按键动作, 休眠 */
	wait_event_interruptible(button_waitq, ev_press);
	
	/* 如果有按键动作, 返回键值 */
	copy_to_user(buf, &key_val, 1);
	ev_press = 0;    //清除标志
	
	return 1;
}


static unsigned forth_drv_poll(struct file *file, poll_table *wait)
{
	unsigned int mask = 0;
	poll_wait(file, &button_waitq, wait); // 不会立即休眠
	
	if (ev_press)
		mask |= POLLIN | POLLRDNORM;

	return mask;
}
/* 这个结构是字符设备驱动程序的核心
 * 当应用程序操作设备文件时所调用的open、read、write等函数，
 * 最终会调用这个结构中指定的对应函数
 */
static struct file_operations fourth_drv_fops = {
    .owner  =   THIS_MODULE,    /* 这是一个宏，推向编译模块时自动创建的__this_module变量 */
    .open   =   fourth_drv_open,     
	.read	=	fourth_drv_read,	   
	.write	=	fourth_drv_write,	
	.release=   fourth_drv_close,
	.poll   =   forth_drv_poll,
};

int major;
/*
 * 执行insmod命令时就会调用这个函数 
 */
static int __init fourth_drv_init(void)
{
	/* 注册字符设备
     * 参数为主设备号、设备名字、file_operations结构；
     * 这样，主设备号就和具体的file_operations结构联系起来了，
     * 操作主设备为MAJOR的设备文件时，就会调用s3c24xx_leds_fops中的相关成员函数
     * MAJOR可以设为0，表示由内核自动分配主设备号
     */
	
	major = register_chrdev(0, DEVICE_NAME, &fourth_drv_fops);
	if (major < 0) {
      printk(DEVICE_NAME " can't register major number\n");
      return major;
    }
	
	//创建一个类先
	fourthdrv_class = class_create(THIS_MODULE, DEVICE_NAME);
	//在这个类下面创建一个设备
	fourthdrv_class_dev = class_device_create(fourthdrv_class, NULL, MKDEV(major, 0), NULL, "buttons"); /* /dev/buttons */

	//映射而已
	gpfcon = (volatile unsigned long *)ioremap(0x56000050, 16);
	gpfdat = gpfcon + 1;
	gpgcon = (volatile unsigned long *)ioremap(0x56000060, 16);
	gpgdat = gpgcon + 1;
	
	
	return 0;
}

/*
 * 执行rmmod命令时就会调用这个函数 
 */
static void __exit fourth_drv_exit(void)
{
    /* 卸载驱动程序 */
    unregister_chrdev(major, DEVICE_NAME);
	class_device_unregister(fourthdrv_class_dev);
	class_destroy(fourthdrv_class);
	iounmap(gpfcon);
	iounmap(gpgcon);
	printk(DEVICE_NAME " exit success\n");

}

/* 这两行指定驱动程序的初始化函数和卸载函数 */
module_init(fourth_drv_init);
module_exit(fourth_drv_exit);

/* 描述驱动程序的一些信息，不是必须的 */
MODULE_AUTHOR("lss wechat qabc132321");
MODULE_VERSION("0.1.0");
MODULE_DESCRIPTION("S3C2410/S3C2440 LED Driver");
MODULE_LICENSE("GPL");





