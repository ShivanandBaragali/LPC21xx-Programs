#include <lpc21xx.h>
#include <stdio.h>
#include <stdlib.h>

void I2C_INIT(void)
{

	PINSEL0 = PINSEL0 | 0x00000050;
	I2CONSET = 0X40;
	I2SCLL = 75;
	I2SCLH = 75;
}
void I2C_START(void)
{
	I2CONSET = 0X20;
	while((I2CONSET & 0X08)!=0X08);
	I2CONCLR = 0X28;
}

void I2C_WRITEADD( char data )
{
	I2CONSET=0X40;
	I2DAT=data;
	 while((I2CONSET & 0X08)!=0X08);
	I2CONCLR = 0X08;
}

int main()
{
	int i=0;
	I2C_INIT();
	I2C_START();
	I2C_WRITEADD(0X00);
	while((I2STAT & 0X18)!=0X18);
	while(i<26)
	{
	I2C_WRITEADD('A'+i);
			while((I2STAT & 0X28)!=0X28);
		i++;
	}
}
