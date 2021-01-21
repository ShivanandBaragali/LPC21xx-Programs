#include <lpc21xx.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void delay_ms(unsigned int count)
{
	T0PR=5999;
	T0TCR=0x02;
	T0TCR=0x01;
	while(T0TC!=count);
	T0TCR=0x00;
}

void LCD_CMD(char command)
{
	IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((command & 0xF0)<<8) );
	IO0SET = 0x00000040;
	IO0CLR = 0x00000030;
	delay_ms(50);
	IO0CLR = 0x00000040;
	delay_ms(5);
	IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((command & 0x0F)<<12) );
	IO0SET = 0x00000040;
	IO0CLR = 0x00000030;
	delay_ms(50);
	IO0CLR = 0x00000040;
	delay_ms(50);
	
}

void LCD_INIT(void)
{
	IO0DIR = 0x0000FFF0;
	delay_ms(20);
	LCD_CMD(0x02);
	LCD_CMD(0x28);
	LCD_CMD(0x0C);
	LCD_CMD(0x06);
	LCD_CMD(0x01);
	LCD_CMD(0x80);
}

void LCD_STRING (char* msg)
{
	uint8_t i=0;
	while(msg[i]!=0)
	{
		IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((msg[i] & 0xF0)<<8) );
		IO0SET = 0x00000050;
		IO0CLR = 0x00000020;
		delay_ms(20);
		IO0CLR = 0x00000040;
		delay_ms(50);
		IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((msg[i] & 0x0F)<<12) );
		IO0SET = 0x00000050;
		IO0CLR = 0x00000020;
		delay_ms(20);
		IO0CLR = 0x00000040;
		delay_ms(50);
		i++;
	}
}

void LCD_CHAR (char msg)
{
		IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((msg & 0xF0)<<8) );
		IO0SET = 0x00000050;
		IO0CLR = 0x00000020;
		delay_ms(20);
		IO0CLR = 0x00000040;
		delay_ms(50);
		IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((msg & 0x0F)<<12) );
		IO0SET = 0x00000050;A
		IO0CLR = 0x00000020;
		delay_ms(20);
		IO0CLR = 0x00000040;
		delay_ms(50);
}

int main()
{
	uint32_t result;
	float voltage;
	char volt[20];
	LCD_INIT();
	PINSEL1 = 0x01000000;
	ADCR=0X00200402;
	while(1)
	{
		ADCR|=1<<24;
		while(!(ADDR & 0X80000000));
		result=ADDR;
		result=result>>6;
		result=(result & 0x000003ff);
		voltage=(result/1023.0)*3.3;
		LCD_CMD(0X80);
		sprintf(volt, "Voltage=%.2f V", voltage);
		LCD_STRING(volt);
	}
}
