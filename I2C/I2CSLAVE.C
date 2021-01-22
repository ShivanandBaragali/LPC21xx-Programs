#include <lpc21xx.h>
#include <stdio.h>
#include <stdlib.h>
char data;
void UART0_INIT(void)
{
	PINSEL0 |=0x00000005;
	U0LCR = 0x83;
	U0DLM = 0x00;
	U0DLL = 0x61;
	U0LCR = 0x03;
}
void UART0_TxChar(char ch)
{
	U0THR = ch;
	while( (U0LSR & 0x40) == 0 );
}

void I2C_INIT(void)
{
	PINSEL0 = PINSEL0 | 0x00000050;
	I2CONSET = 0X44;
	I2ADR=0X01;
}
void I2C_READ_ACK( void )
{
	while((I2CONSET & 0X08)!=0X08);
	I2CONCLR=0X08;
	data=I2DAT;
}

int main()

{
 I2C_INIT();
 UART0_INIT();
	I2C_READ_ACK();
 while((I2STAT & 0X70)!=0X70);
	I2CONCLR=0X08;


	while(1)

{
	I2C_READ_ACK();
 while((I2STAT & 0X90)!=0X90);
	I2CONCLR=0X08;
	UART0_TxChar(data);
}
}
