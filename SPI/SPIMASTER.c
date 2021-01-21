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

void SPI_MASTER_INIT()
{
	PINSEL0|=0X1500;
	S0SPCR=0X20;
	S0SPCCR=0X10;
}

void SPI_MASTER_WRITE(char data)
{
	char x;
	IOCLR0=1<<7;
	S0SPDR=data;
	while((S0SPSR & 1<<7)!=1<<7);
	x=S0SPDR;
	uart_tx(x);
	IOSET0=1<<7;
}

int main()
{
	uart_init();
	SPI_MASTER_INIT();
	while(1)
	{
		SPI_MASTER_WRITE('a');
	}
}
