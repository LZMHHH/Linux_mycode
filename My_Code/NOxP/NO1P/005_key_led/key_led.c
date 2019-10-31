
#include "s3c2440_soc.h"

void delay(volatile int d)
{
	while(d--);
}

int app(void)
{
	int vla1;
	int vla2;
	
	/* 设置GPFCON让GPF4/5/6配置为输出引脚 */
	GPFCON &= ~((3<<8) | (3<<10) | (3<<12));   /* 先把相应位置 清零 */
	GPFCON |=  ((1<<8) | (1<<10) | (1<<12));   /* 先把相应位置 01 */
	
	/* 配置3个按键引脚为输入引脚:
	 * GPF0(S2),GPF2(S3),GPG3(S4)
	 */
	 GPFCON &= ~((3<<0) | (3<<4));
	 GPGCON &= ~((3<<6));
	 
	 /* 循环点亮 */
	 while(1)
	 {
		 vla1 = GPFDAT;
		 vla2 = GPGDAT;
		 
		 if(vla1 & (1<<0))  // 按键S2 松开
		 {
			 GPFDAT |= (1<<6);  // 关灯
		 }
		 else
		 {
			 GPFDAT &= ~(1<<6); // 开灯
		 }
		 
		 if(vla1 & (1<<2))  // 按键S3 松开
		 {
			 GPFDAT |= (1<<5);  // 关灯
		 }
		 else
		 {
			 GPFDAT &= ~(1<<5); // 开灯
		 }
		 
		 if(vla2 & (1<<3))  // 按键S4 松开
		 {
			 GPFDAT |= (1<<4);  // 关灯
		 }
		 else
		 {
			 GPFDAT &= ~(1<<4); // 开灯
		 }
		 
	 }
	
	
	return 0;
}