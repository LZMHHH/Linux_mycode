
#include "s3c2440_soc.h"

void delay(volatile int d)
{
	while(d--);
}

int app(void)
{
	int vla1;
	int vla2;
	
	/* ����GPFCON��GPF4/5/6����Ϊ������� */
	GPFCON &= ~((3<<8) | (3<<10) | (3<<12));   /* �Ȱ���Ӧλ�� ���� */
	GPFCON |=  ((1<<8) | (1<<10) | (1<<12));   /* �Ȱ���Ӧλ�� 01 */
	
	/* ����3����������Ϊ��������:
	 * GPF0(S2),GPF2(S3),GPG3(S4)
	 */
	 GPFCON &= ~((3<<0) | (3<<4));
	 GPGCON &= ~((3<<6));
	 
	 /* ѭ������ */
	 while(1)
	 {
		 vla1 = GPFDAT;
		 vla2 = GPGDAT;
		 
		 if(vla1 & (1<<0))  // ����S2 �ɿ�
		 {
			 GPFDAT |= (1<<6);  // �ص�
		 }
		 else
		 {
			 GPFDAT &= ~(1<<6); // ����
		 }
		 
		 if(vla1 & (1<<2))  // ����S3 �ɿ�
		 {
			 GPFDAT |= (1<<5);  // �ص�
		 }
		 else
		 {
			 GPFDAT &= ~(1<<5); // ����
		 }
		 
		 if(vla2 & (1<<3))  // ����S4 �ɿ�
		 {
			 GPFDAT |= (1<<4);  // �ص�
		 }
		 else
		 {
			 GPFDAT &= ~(1<<4); // ����
		 }
		 
	 }
	
	
	return 0;
}