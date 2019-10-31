

#ifndef _LED_H_
#define _LED_H_


#include "s3c2440_soc.h"

enum {
	LEDON  = 0,
	LEDOFF = 1,
};

enum {
	eLedOff = 0,
	eLedOn  = 1,
};

#if LEDON

	#define Led_on(n)  (GPFDAT |= (LEDON<<n))
	#define Led_off(n) (GPFDAT &= ~(LEDON<<n))

#else
	#define Led_off(n)  (GPFDAT |= (LEDOFF<<n))
	#define Led_on(n) (GPFDAT &= ~(LEDOFF<<n))
	
#endif


void Led_Init(void);
void Led_Con(char LedNum, char LedSwitch);
int  led_test(void);

#endif



