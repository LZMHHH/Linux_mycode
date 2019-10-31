#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>

#define DEVICE_NAME     "led"  /* 加载模式后，执行”cat /proc/devices”命令看到的设备名称 */
#define LED_MAJOR       112     /* 主设备号 */


static struct class *leds_class;
static struct class_device	*leds_class_devs[4];


/* bit0<=>D10, 0:亮, 1:灭 
 *  bit1<=>D11, 0:亮, 1:灭 
 *  bit2<=>D12, 0:亮, 1:灭 
 */ 
static char leds_status = 0x0;  
static DECLARE_MUTEX(leds_lock); // 定义赋值

//static int minor;
static unsigned long gpio_va;

#define GPIO_OFT(x) ((x) - 0x56000000)
#define GPFCON  (*(volatile unsigned long *)(gpio_va + GPIO_OFT(0x56000050)))
#define GPFDAT  (*(volatile unsigned long *)(gpio_va + GPIO_OFT(0x56000054)))



/* 应用程序对设备文件/dev/leds执行open(...)时，
 * 就会调用s3c24xx_leds_open函数
 */
static int s3c24xx_leds_open(struct inode *inode, struct file *file)
{
	int minor = MINOR(inode->i_rdev);  //MINOR(inode->i_cdev);
	
	switch(minor)
	{
		case 0: /* /dev/leds */
		{
			// 配置3引脚为输出
            //s3c2410_gpio_cfgpin(S3C2410_GPF4, S3C2410_GPF4_OUTP);
            GPFCON &= ~(0x3<<(4*2));
            GPFCON |= (1<<(4*2));
            
            //s3c2410_gpio_cfgpin(S3C2410_GPF5, S3C2410_GPF5_OUTP);
            GPFCON &= ~(0x3<<(5*2));
            GPFCON |= (1<<(5*2));

            //s3c2410_gpio_cfgpin(S3C2410_GPF6, S3C2410_GPF6_OUTP);
            GPFCON &= ~(0x3<<(6*2));
            GPFCON |= (1<<(6*2));

            // 都输出0
            //s3c2410_gpio_setpin(S3C2410_GPF4, 0);
            GPFDAT &= ~(1<<4);
            
            //s3c2410_gpio_setpin(S3C2410_GPF5, 0);
            GPFDAT &= ~(1<<5);
            //s3c2410_gpio_setpin(S3C2410_GPF6, 0);
            GPFDAT &= ~(1<<6);
			
			break;
		}
		
		case 1: /* /dev/led1 */
        {
            s3c2410_gpio_cfgpin(S3C2410_GPF4, S3C2410_GPF4_OUTP);
            s3c2410_gpio_setpin(S3C2410_GPF4, 0);
            
            
            break;
        }

        case 2: /* /dev/led2 */
        {
            s3c2410_gpio_cfgpin(S3C2410_GPF5, S3C2410_GPF5_OUTP);
            s3c2410_gpio_setpin(S3C2410_GPF5, 0);
            leds_status &= ~(1<<1);
            break;
        }

        case 3: /* /dev/led3 */
        {
            s3c2410_gpio_cfgpin(S3C2410_GPF6, S3C2410_GPF6_OUTP);
            s3c2410_gpio_setpin(S3C2410_GPF6, 0);

            
            break;
        }
        
	}
	
    return 0;
}	

static ssize_t s3c24xx_leds_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	return 0;
}

static ssize_t s3c24xx_leds_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	return 1;
}

/* 这个结构是字符设备驱动程序的核心
 * 当应用程序操作设备文件时所调用的open、read、write等函数，
 * 最终会调用这个结构中指定的对应函数
 */
static struct file_operations s3c24xx_leds_fops = 
{
	.owner = THIS_MODULE,   /* 这是一个宏，推向编译模块时自动创建的__this_module变量 */
	.open  = s3c24xx_leds_open,
	.write = s3c24xx_leds_write,
	.read  = s3c24xx_leds_read,
};

/*
 * 执行insmod命令时就会调用这个函数 
 */
static int __init s3c24xx_leds_init(void)
{
	int ret;
	
	/* 注册字符设备
     * 参数为主设备号、设备名字、file_operations结构；
     * 这样，主设备号就和具体的file_operations结构联系起来了，
     * 操作主设备为LED_MAJOR的设备文件时，就会调用s3c24xx_leds_fops中的相关成员函数
     * LED_MAJOR可以设为0，表示由内核自动分配主设备号
     */
	 
	ret = register_chrdev(LED_MAJOR, DEVICE_NAME, &s3c24xx_leds_fops);
	if (ret < 0) {
      printk(DEVICE_NAME " can't register major number\n");
      return ret;
    }
	
	printk(DEVICE_NAME " initialized\n");
    return 0;
}

/*
 * 执行rmmod命令时就会调用这个函数 
 */
static void __exit s3c24xx_leds_exit(void)
{
	/* 卸载驱动程序 */
    unregister_chrdev(LED_MAJOR, DEVICE_NAME);
}

/* 这两行指定驱动程序的初始化函数和卸载函数 */
module_init(s3c24xx_leds_init);
module_exit(s3c24xx_leds_exit);

/* 描述驱动程序的一些信息，不是必须的 */
MODULE_AUTHOR("lss wechat qabc132321");
MODULE_VERSION("0.1.0");
MODULE_DESCRIPTION("S3C2410/S3C2440 LED Driver");
MODULE_LICENSE("GPL");

