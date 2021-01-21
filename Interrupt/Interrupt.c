#include<lpc21xx.h>
void delay(int  count)
{
	T0PR=59999;
	T0TCR=0X02;
	T0TCR=0X01;
	while(count!=T0TC);
	T0TCR=0X00;
}

__irq void EXT2ISR(void)
{
	EXTINT|=1<<2;
	IOSET0|=0X02;
	delay(500);
	IOCLR0|=0X02;
	VICVectAddr=0x00;
}

__irq void EXT2ISR2(void)
{
	EXTINT|=1<<1;
	IOSET0|=0X04;
	delay(500);
	IOCLR0|=0X04;
	VICVectAddr=0x00;
}

void ext_init()
{
	EXTMODE&=1<<2;
	EXTPOLAR&=1<<2;
	PINSEL0|=0X03<<14|0X03<<6;
	VICIntEnable|=1<<15;
	VICIntEnable|=1<<16;
	VICVectCntl0=1<<5|16;
	VICVectCntl1=1<<5|15;
	VICVectAddr0=(unsigned) EXT2ISR;
	VICVectAddr1=(unsigned) EXT2ISR2;
}

int main()
{
	ext_init();
	IODIR0 |= 0X07;
	while(1)
	{
		IOCLR0|=0X01;
		delay(500);
		IOSET0|=0X01;
		delay(500);
	}
}
