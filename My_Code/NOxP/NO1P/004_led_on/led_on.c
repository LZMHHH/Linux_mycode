
void delay(volatile int d)
{
	while(d--);
}

int app(int ledx)
{
	unsigned int *pGPFCON = (unsigned int *)0X56000050;
	unsigned int *pGPFDAT = (unsigned int *)0X56000054;
	
	if(ledx == 4)
	{
		/* 配置 GPF4 为输出引脚 */
		*pGPFCON = 0X100;
		/* 设置 GPF4 输出0 */
		*pGPFDAT = 0;
	}
	
	if(ledx == 5)
	{
		/* 配置 GPF5 为输出引脚 */
		*pGPFCON = 0X400;
		/* 设置 GPF5 输出0 */
		*pGPFDAT = 0;
	}
	
	return 0;
}


