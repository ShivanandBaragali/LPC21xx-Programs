#include <lpc21xx.h>
#include <stdint.h>
#include <stdlib.h>
void delay(unsigned int count)
{
	T0PR=59999;
	T0TCR=0x02;
	T0TCR=0x01;
	while(T0TC<count);
	T0TCR=0x00;
}

void uart_init()
{
	PINSEL0|=0X05;
	U0LCR=0X83;
	U0DLM=0X00;
	U0DLL=0X61;
	U0LCR=0X03;
}

unsigned char uart_rx()
{
	while((U0LSR & 0X01)==0);
	return U0RBR;
}

void uart_tx(char a)
{
	U0THR=a;
	while((U0LSR & 0X40)==0);
}

int main()
{
	char n1='0',n2='0';
	uart_init();
	for(;;)
	{
		delay(250);
		uart_tx(n1);
		uart_tx(n2);
		n2++;
		if(n2==':')
			n2='0';
		if(n2=='0')
			n1++;
		if(n1==':')
		{
			n1='0';
			n2='0';
		}
	}
}
