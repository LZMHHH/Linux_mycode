#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/platform_device.h>
#include <linux/clk.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/div64.h>

#include <asm/mach/map.h>
#include <asm/arch/regs-lcd.h>
#include <asm/arch/regs-gpio.h>
#include <asm/arch/fb.h>


static struct   fb_info  *s3c_lcd;
static volatile unsigned long *gpbcon;
static volatile unsigned long *gpbdat;


static int lcd_init(void)
{
	/* 1、分配一个fb_info */
	s3c_lcd = framebuffer_alloc(0, NULL);
	
	/* 2、设置 */
	/* 2.1 设置固定参数 */
	strcpy(s3c_lcd->fix.id, "mylcd");
	s3c_lcd->fix.smem_len = 480*272*16/8;
	/* 2.2 设置可变长参数*/
	/* 2.3 设置操作函数*/
	/* 2.4 其他的设置*/
	
	/* 3、硬件相关操作 */
	/* 3.1 配置GPIO用于LCD*/
	/* 3.2 根据手册设置LCD控制器，比如VCLK的频率等*/
	/* 3.3 分配显存（framebuffer），并把地址告诉LCD控制器*/
	
	/* 4、注册 */
}


static void lcd_exit(void)
{
	
}

module_init(lcd_init);
module_exit(lcd_exit);

MODULE_LICENSE("GPL");

