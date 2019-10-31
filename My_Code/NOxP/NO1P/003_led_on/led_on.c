

int main()
{
	unsigned int *pGPFCON = (unsigned int *)0X56000050;
	unsigned int *pGPFDAT = (unsigned int *)0X56000054;
	
	/* 配置 GPF4 为输出引脚 */
	*pGPFCON = 0X100;
	
	/* 设置 GPF4 输出0 */
	*pGPFDAT = 0;
	
	return 0;
}


