#include <linux/module.h>     //最基本的头文件，其中定义了MODULE_LICENSE这一类宏
#include <linux/fs.h>		  //file_operations等结构体的定义
#include <linux/init.h>       //定义module_init等模块

#include <linux/kernel.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>

#define  DEVICE_NAME  "led"   //设备名称
#define  LED_MAJOR     56        //主设备号

//用于自动创建设备节点的参数容器
static struct class *leds_class;
static struct class_device	*leds_class_devs[4];

//static int minor;
static unsigned long gpio_va;    //存放虚拟映射的内容地址（基地址）
#define GPIO_OFT(x) ((x) - 0x56000000)  //偏移地址
#define GPFCON  (*(volatile unsigned long *)(gpio_va + GPIO_OFT(0x56000050)))
#define GPFDAT  (*(volatile unsigned long *)(gpio_va + GPIO_OFT(0x56000054)))


//辅助函数----------------------------------------------------------------
//根据次设备号来初始化LED设备
void led_set_init(int minor)
{
	switch(minor)
	{
		case 0: /* /dev/leds   操作所有次设备*/
		{
			// 配置3引脚为输出
			GPFCON &= ~(0x3<<(4*2));
			GPFCON |= (1<<(4*2));
			
			GPFCON &= ~(0x3<<(5*2));
			GPFCON |= (1<<(5*2));

			GPFCON &= ~(0x3<<(6*2));
			GPFCON |= (1<<(6*2));

			// 全关闭
			GPFDAT |= (1<<4);
			GPFDAT |= (1<<5);
			GPFDAT |= (1<<6);
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
            break;
        }

        case 3: /* /dev/led3 */
        {
            s3c2410_gpio_cfgpin(S3C2410_GPF6, S3C2410_GPF6_OUTP);
            s3c2410_gpio_setpin(S3C2410_GPF6, 0);

            break;
        }
	}
}


//根据次设备号和键值控制单个LED设备
void led_controller(int minor, int key)
{
	int i = 0;
	switch(minor)
	{
		case 0:   /* 全控 */
		{
			for(; i < 3; i++)
			{
				if(key == 0)
				{
					GPFDAT |= (1<<(4+i));    //LED1 灭
				}
				else if(key == 1)
				{
					GPFDAT &= ~(1<<(4+i));    //LED1 开
				}
				else
				{
					printk("arguments number is wrong.%d\n",(i+4));
				}
			}
			break;
		}
		
		case 1: //LED1
		{
				if(key == 0)
				{
					GPFDAT |= (1<<(4));    //LED1 灭
				}
				else if(key == 1)
				{
					GPFDAT &= ~(1<<(4));    //LED1 开
				}
				else
				{
					printk("arguments number of LED1 is wrong.\n");
				}
				break;
				
		}
		
		case 2: //LED2
		{
				if(key == 0)
				{
					GPFDAT |= (1<<(5));    //LED2 灭
				}
				else if(key == 1)
				{
					GPFDAT &= ~(1<<(5));    //LED2 开
				}
				else
				{
					printk("arguments number of LED2 is wrong.\n");
				}
				break;
		}
		
		case 3: //LED3
		{
				if(key == 0)
				{
					GPFDAT |= (1<<(6));    //LED3灭
				}
				else if(key == 1)
				{
					GPFDAT &= ~(1<<(6));    //LED3 开
				}
				else
				{
					printk("arguments number of LED3 is wrong.\n");
				}
				break;
		}
	}
}


//框架   结构----------------------------------------------------------------------------------
//定义open函数
static int s3c24xx_leds_open(struct inode *inode, struct file *file)
{
	printk("%s drive open\n",DEVICE_NAME);
	
	int minor = MINOR(inode->i_rdev);   //取出次设备号
	
	led_set_init(minor);
	
	
	
	return 0;
}

//定义write函数
static ssize_t s3c24xx_leds_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	
	int minor = MINOR(file->f_dentry->d_inode->i_rdev);
	volatile unsigned int argv[3];  
	int i = 0;
	
	
#if 0	
	//如果传入的参数数量不对，那么直接返回即可
	if(count != 12 && count != 8)
	{
		printk("arguments count is wrong.%d\n", count);
		return 0;
	}
#endif
	
	copy_from_user(argv, buf, count);//从用户空间获取传入的数据
	
	
	if(count == 12)
	{
		led_controller(1,argv[0]);
		led_controller(2,argv[1]);
		led_controller(3,argv[2]);
	}
	else if(count == 8)
	{
		led_controller(minor,argv[1]);
	}
	
	printk("%s drive write success\n",DEVICE_NAME);
	
	return 0;
}

//定义read函数
static ssize_t s3c24xx_leds_read(struct file *filp, char __user *buff, 
                                         size_t count, loff_t *offp)
{
	printk("%s drive read\n",DEVICE_NAME);
	return 0;
}

/* 这个结构是字符设备驱动程序的核心
 * 当应用程序操作设备文件时所调用的open、read、write等函数，
 * 最终会调用这个结构中指定的对应函数
 */
static struct file_operations s3c24xx_leds_fops = {
    .owner  =   THIS_MODULE,    /* 这是一个宏，推向编译模块时自动创建的__this_module变量 */
    .open   =   s3c24xx_leds_open,     
	.read	=	s3c24xx_leds_read,	   
	.write	=	s3c24xx_leds_write,	   
};


/*
 * 执行insmod命令时就会调用这个函数 
 */
static int __init s3c24xx_leds_init(void)
{
	/* 注册字符设备
     * 参数为主设备号、设备名字、file_operations结构；
     * 这样，主设备号就和具体的file_operations结构联系起来了，
     * 操作主设备为LED_MAJOR的设备文件时，就会调用s3c24xx_leds_fops中的相关成员函数
     * LED_MAJOR可以设为0，表示由内核自动分配主设备号
     */
	int ret;
	int minor = 0;  //从设备号
	
	
	gpio_va = ioremap(0x56000000, 0x100000);    //内存映射
	if (!gpio_va) {
		return -EIO;
	}
	
	//如果主设备号为0，则系统会自动分配主设备号
	ret = register_chrdev(LED_MAJOR, DEVICE_NAME, &s3c24xx_leds_fops);
	if (ret < 0) {
      printk(DEVICE_NAME " can't register LED_MAJOR number\n");
      return ret;
    }

	//先创建一个class
	leds_class = class_create(THIS_MODULE, "ledS");   
	if (IS_ERR(leds_class))
		return PTR_ERR(leds_class);
	
	//在上面的类下创建一个设备
	leds_class_devs[0] = class_device_create(leds_class, NULL, MKDEV(LED_MAJOR, 0), NULL, "leds"); /* /dev/leds */
	
	for (minor = 1; minor < 4; minor++)  /* /dev/led1,2,3 */
	{
		leds_class_devs[minor] = class_device_create(leds_class, NULL, MKDEV(LED_MAJOR, minor), NULL, "led%d", minor);
		if (unlikely(IS_ERR(leds_class_devs[minor])))
			return PTR_ERR(leds_class_devs[minor]);
	}
	
	printk("init success\n");
	
	return 0;
}

/*
 * 执行rmmod命令时就会调用这个函数 
 */
static void __exit s3c24xx_leds_exit(void)
{
	int minor;
	
    /* 卸载驱动程序 */
    unregister_chrdev(LED_MAJOR, DEVICE_NAME);
	
	
	for (minor = 0; minor < 4; minor++)
	{
		class_device_unregister(leds_class_devs[minor]);
	}
	
	class_destroy(leds_class);
	
	printk(DEVICE_NAME " exit success\n");
	
	iounmap(gpio_va);

}

/* 这两行指定驱动程序的初始化函数和卸载函数 */
module_init(s3c24xx_leds_init);
module_exit(s3c24xx_leds_exit);

/* 描述驱动程序的一些信息，不是必须的 */
MODULE_AUTHOR("lss wechat qabc132321");
MODULE_VERSION("0.1.0");
MODULE_DESCRIPTION("S3C2410/S3C2440 LEDs Driver");
MODULE_LICENSE("GPL");





