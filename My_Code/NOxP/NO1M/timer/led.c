
#include "led.h"



void Led_Init(void)
{
	/* 设置GPFCON让GPF4/5/6配置为输出引脚 */
	GPFCON &= ~((3<<8) | (3<<10) | (3<<12));
	GPFCON |=  ((1<<8) | (1<<10) | (1<<12));
	
	//默认关闭
	Led_off(4);
	Led_off(5);
	Led_off(6);
}

void Led_Con(char LedNum, char LedSwitch)
{
	if(LedNum == 1)
	{
		if(LedSwitch == eLedOn)
		{
			Led_on(4);
		}
		else
		{
			Led_off(4);
		}
	}
	if(LedNum == 2)
	{
		if(LedSwitch == eLedOn)
		{
			Led_on(5);
		}
		else
		{
			Led_off(5);
		}
	}
	if(LedNum == 3)
	{
		if(LedSwitch == eLedOn)
		{
			Led_on(6);
		}
		else
		{
			Led_off(6);
		}
	}
}

int led_test(void)
{
	int val = 0;  /* val: 0b000, 0b111 */
	int tmp;

	/* 设置GPFCON让GPF4/5/6配置为输出引脚 */
	GPFCON &= ~((3<<8) | (3<<10) | (3<<12));
	GPFCON |=  ((1<<8) | (1<<10) | (1<<12));

	/* 循环点亮 */
	while (1)
	{
		tmp = ~val;
		tmp &= 7;
		GPFDAT &= ~(7<<4);
		GPFDAT |= (tmp<<4);
		delay(100000);
		val++;
		if (val == 8)
			val =0;
		
	}

	return 0;
}






