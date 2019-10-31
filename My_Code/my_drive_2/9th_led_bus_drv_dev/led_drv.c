
/* 分配/设置/注册一个platform_driver */

#include <linux/module.h>
#include <linux/version.h>

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/io.h>


/* 1、分配/2、设置/3、注册一个platform_driver */

static int major;

static struct   class *cls;
static volatile unsigned long *gpio_con;
static volatile unsigned long *gpio_dat;
static int      pin;

static int led_open(struct inode *inode, struct file *file)
{
	//printk("first_drv_open\n");
	/* 配置为输出 */
	*gpio_con &= ~(0x3<<(pin*2));
	*gpio_con |= (0x1<<(pin*2));
	return 0;	
}

static ssize_t led_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	int val;

	//printk("first_drv_write\n");

	copy_from_user(&val, buf, count); //	copy_to_user();

	if (val == 1)
	{
		// 点灯
		*gpio_dat &= ~(1<<pin);
	}
	else
	{
		// 灭灯
		*gpio_dat |= (1<<pin);
	}
	
	return 0;
}
static struct file_operations led_fops = {
    .owner  =   THIS_MODULE,    /* 这是一个宏，推向编译模块时自动创建的__this_module变量 */
    .open   =   led_open,     
	.write	=	led_write,	   
};

static int led_probe(struct platform_device *pdev)
{
	struct resource   *res;
	
	/* 根据platform_device的资源进行ioremap */
	res      = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	gpio_con = ioremap(res->start, res->end - res->start + 1);
	gpio_dat = gpio_con + 1;
	
	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	pin = res->start;
	
	/* 注册字符设备驱动程序 */
	printk("led_probe, found led\n");
	major = register_chrdev(0, "myled", &led_fops);
	cls = class_create(THIS_MODULE, "myled");
	class_device_create(cls, NULL, MKDEV(major, 0), NULL, "led"); /* /dev/led */
	
	return 0;
	
}
static int led_remove(struct platform_device *pdev)
{
	/* 卸载字符设备驱动程序 */
	/* iounmap */
	printk("led_remove, remove led\n");

	class_device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);
	unregister_chrdev(major, "myled");
	iounmap(gpio_con);
	
	return 0;
}

/******************************************************************             
 *     平台驱动框架 
 *******************************************************************/
/* 分配+设置一个platform_driver */
struct platform_driver led_drv = 
{
	.probe   = led_probe,  //自己写的一个probe函数（资源探测函数）
	.remove  = led_remove, //自己写的也给remove函数（移除资源函数）
	.driver  = {
				.name = "myled",   //该名字要与 platform_device 中的名字一样，因为.match就是通过这两个名字来判断的。
				},
};

/* 入口函数 注册一个平台设备 */
static int led_drv_init(void)
{
	platform_driver_register(&led_drv);
	return 0;
}

/* 出口函数 注销一个平台设备 */
static void led_drv_exit(void)
{
	platform_driver_unregister(&led_drv);
}

module_init(led_drv_init);
module_exit(led_drv_exit);

MODULE_LICENSE("GPL");


