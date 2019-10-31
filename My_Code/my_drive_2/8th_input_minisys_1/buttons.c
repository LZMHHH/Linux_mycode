
/* 参考drivers\input\keyboard\gpio_keys.c */

#include <linux/module.h>     //最基本的头文件，其中定义了MODULE_LICENSE这一类宏
#include <linux/fs.h>		  //file_operations等结构体的定义
#include <linux/init.h>       //定义module_init等模块
#include <linux/version.h>

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

#include <asm/gpio.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>


struct pin_desc
{
	           int   irq;
	           char  *name;
	unsigned   int   pin;
	unsigned   int   key_val;
	
};

struct pin_desc pins_desc[4] = 
{
	{IRQ_EINT0,  "S2", S3C2410_GPF0,   KEY_W},
	{IRQ_EINT2,  "S3", S3C2410_GPF2,   KEY_Q},
	{IRQ_EINT11, "S4", S3C2410_GPG3,   KEY_ENTER},
	{IRQ_EINT19, "S5",  S3C2410_GPG11, KEY_LEFTSHIFT},
};

static struct input_dev *buttons_dev;
static struct pin_desc *irq_pd;
static struct timer_list buttons_timer;

static irqreturn_t buttons_irq(int irq, void *dev_id)
{
	/* 10ms后调用定时器回调函数 */
	irq_pd = (struct pin_desc *)dev_id;
	mod_timer(&buttons_timer, jiffies + HZ/100);
	return IRQ_RETVAL(IRQ_HANDLED);
	
}

static void buttons_timer_function(unsigned long data)
{
	struct pin_desc * pindesc = irq_pd;
	unsigned int pinval;

	if (!pindesc)
		return;
	
	pinval = s3c2410_gpio_getpin(pindesc->pin);

	if (pinval)
	{
		/* 松开 : 最后一个参数: 0-松开, 1-按下 */
		input_event(buttons_dev, EV_KEY, pindesc->key_val, 0);
		input_sync(buttons_dev);
	}
	else
	{
		/* 按下 */
		input_event(buttons_dev, EV_KEY, pindesc->key_val, 1);
		input_sync(buttons_dev);
	}
}


//子系统入口函数
static int buttons_init(void)
{
	int i;
	
	/* 1. 分配一个input_dev结构体 */
	buttons_dev = input_allocate_device();
	buttons_dev->name = "keys";
	
	/* 2.  设置 */
	/* 2.1 能产生哪类事件 */
	set_bit(EV_KEY, buttons_dev->evbit); //设置成EV_KEY（按键类）。设置evbit数组中的某一位为EV_KEY事件。
	set_bit(EV_REP, buttons_dev->evbit); //重复类事件
	
	/* 2.2 能产生这类操作中的哪些事件：W,Q,ENTER,ALT */
	set_bit(KEY_W, buttons_dev->keybit);//设置成KEY_W事件。设置key_bit数组中的某一位为KEY_W。作为事件驱动之一。
	set_bit(KEY_Q, buttons_dev->keybit);
	set_bit(KEY_ENTER, buttons_dev->keybit);
	set_bit(KEY_LEFTSHIFT, buttons_dev->keybit);
	
	/* 注册 */
	input_register_device(buttons_dev);
	
	/* 硬件相关操作 */
	init_timer(&buttons_timer);
	buttons_timer.function = buttons_timer_function;
	add_timer(&buttons_timer);  //告诉内核由该函数
	
	for(i=0;i<4;i++)
	{
		request_irq(pins_desc[i].irq, buttons_irq, IRQT_BOTHEDGE, pins_desc[i].name, &pins_desc[i]);
	}
	
	return 0;	
	
}

/* 出口函数 */
static void buttons_exit(void)
{
	int i;
	for(i=0;i<4;i++)
	{
		free_irq(pins_desc[i].irq, &pins_desc[i]);
	}
	del_timer(&buttons_timer);
	input_unregister_device(buttons_dev);
	input_free_device(buttons_dev);	
}



module_init(buttons_init);
module_exit(buttons_exit);

/* 协议 */
MODULE_LICENSE("GPL");


