#include <lpc21xx.h>
#include <stdio.h>
#include <stdlib.h>
void uart_init()
{
	PINSEL0|=0X05;
	U0LCR=0X83;
	U0DLM=0X00;
	U0DLL=0X61;
	U0LCR=0X03;
}

void uart_tx(char a)
{
	U0THR=a;
	while((U0LSR & 0X40)==0);
}

void SPI_SLAVE_INIT()
{
	PINSEL0|=0X5500;
	S0SPCR=0X00;
}

unsigned char SPI_SLAVE_READ()
{
	while(!(S0SPSR & 0x80));
	return S0SPDR;
}

int main()
{
	char x;
	uart_init();
	SPI_SLAVE_INIT();
	while(1)
	{
		x=SPI_SLAVE_READ();
		uart_tx(x);
	}
}
