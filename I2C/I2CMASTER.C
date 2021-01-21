#include <lpc21xx.h>
#include <stdio.h>
#include <stdlib.h>

void I2C_INIT(void)
{ 	

	PINSEL0 = PINSEL0 | 0x00000050; /* Configure P0.2 and P0.3 as SCL0 and SDA0 respectively */
	I2CONSET = 0X40; /* Enable I2C  */	
	I2SCLL = 75; /* I2C data rate 300Khz and 50% duty cycle */
	I2SCLH = 75; /* I2C data rate 300Khz and 50% duty cycle */
}
void I2C_START(void)
{
	I2CONSET = 0X20; 
	while((I2CONSET & 0X08)!=0X08); 
	I2CONCLR = 0X28; /* Clear SI bit */	
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
