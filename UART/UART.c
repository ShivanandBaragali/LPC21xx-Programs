#include <lpc21xx.h>
#include <stdint.h>
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

void uart_tx(char ch)
{
	U0THR=ch;
	while((U0LSR & 0X40)==0);
}

int main()
{
	char ch='A';
	uart_init();
	while(ch<='Z')
	{
		if(!(IOPIN0 & 0X01))
		{
			delay(250);
			if((IOPIN0 & 0X01))
			{
			uart_tx(ch);
			ch++;
			}
		}
	}
}
