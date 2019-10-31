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


/* 1������/2������/3��ע��һ��platform_device */

/* ƽ̨�豸��platform_device������Դ */
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


/* �ṩƽ̨�豸���豸��release */
static void led_release(struct device * dev)
{
}

/******************************************************************             
 *     ƽ̨�豸��� 
 *******************************************************************/

/* ����Ҳ ƽ̨�豸 �ṹ�� */
static struct platform_device led_dev = 
{
	.name     =   "myled",     //�ر��������֣���Ϊ���� .match ʱ���Ƚϵ������֡�
	.id       =   1,
	.num_resources   =   ARRAY_SIZE(led_resource),
	.resource        =   led_resource,     //��ʾƽ̨�豸�ṹӵ�е���Դ
	.dev = {
			.release = led_release,
			},
};

/* ��ں��� ע��һ��ƽ̨�豸 */
static int led_dev_init(void)
{
	platform_device_register(&led_dev);
	return 0;
}

/* ���ں��� ע��һ��ƽ̨�豸 */
static int led_dev_exit(void)
{
	platform_device_unregister(&led_dev);
}


module_init(led_dev_init);
module_exit(led_dev_exit);

MODULE_LICENSE("GPL");




