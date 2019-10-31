#include <linux/module.h>
#include <linux/version.h>

#include <linux/init.h>

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>


/* 1、分配/2、设置/3、注册一个platform_device */

/* 平台设备（platform_device）的资源 */
static struct resource led_resource[] = 
{
	[0] = {
			.start = 0x56000050,
			.end   = 0x56000050 + 8 - 1,
			.flags = IORESOURCE_MEM,
			},
	[1] = {
			.start = 5,
			.end   = 5,
			.flags = IORESOURCE_IRQ,
			}
};


/* 提供平台设备中设备的release */
static void led_release(struct device * dev)
{
}

/******************************************************************             
 *     平台设备框架 
 *******************************************************************/

/* 定义也 平台设备 结构体 */
static struct platform_device led_dev = 
{
	.name     =   "myled",     //特别留意名字，因为调用 .match 时，比较的是名字。
	.id       =   1,
	.num_resources   =   ARRAY_SIZE(led_resource),
	.resource        =   led_resource,     //表示平台设备结构拥有的资源
	.dev = {
			.release = led_release,
			},
};

/* 入口函数 注册一个平台设备 */
static int led_dev_init(void)
{
	platform_device_register(&led_dev);
	return 0;
}

/* 出口函数 注销一个平台设备 */
static int led_dev_exit(void)
{
	platform_device_unregister(&led_dev);
}


module_init(led_dev_init);
module_exit(led_dev_exit);

MODULE_LICENSE("GPL");




