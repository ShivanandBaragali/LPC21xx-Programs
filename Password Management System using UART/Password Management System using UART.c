#include <lpc214x.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void delay_ms(unsigned int counts)
{
	T0PR = 59999;
	T0TCR = 0x02;
	T0TCR = 0x01;

	while(T0TC < counts/4);

	T0TCR = 0x00;
}

void LCD_CMD(char command)
{
	IO0PIN = ( (IO0PIN & 0xFFFF00FF) | (command<<8) );
	IO0SET = 0x00000040; 
	IO0CLR = 0x00000030; 
	delay_ms(2);
	IO0CLR = 0x00000040; 
	delay_ms(5);
}

void LCD_INIT(void)
{
	IO0DIR = 0x0000FFF0; 
	delay_ms(20);
	LCD_CMD(0x38);  
	LCD_CMD(0x0C);   
	LCD_CMD(0x06); 
	LCD_CMD(0x01);   
	LCD_CMD(0x80);  
}

void LCD_CHAR (char msg)
{
		IO0PIN = ( (IO0PIN & 0xFFFF00FF) | (msg<<8) );
		IO0SET = 0x00000050; 
		IO0CLR = 0x00000020; 
		delay_ms(2);
		IO0CLR = 0x00000040; 
		delay_ms(5);
}


void LCD_STRING (char* msg)
{
	uint8_t i=0;
	while(msg[i]!=0)
	{
		LCD_CHAR(msg[i]);
		i++;
	}
}

void UART0_INIT()
{
	PINSEL0 = 0x05;
	U0LCR = 0x83;
	U0DLM = 0x00;
	U0DLL = 0x61;
	U0LCR = 0x03;
}

char UART0_RXchar()
{
	while((U0LSR & 0x01)==0);
	return(U0RBR);
}

void UART0_TXchar(unsigned char ch)
{
	while ((U0LSR & 0x20)==0);
	U0THR=ch;
}

void UART0_TXstring(char* ptr)
{
	while(*ptr!=0)
	{
		UART0_TXchar(*ptr);
		ptr++;
	}
}

int main(void)
{
	uint8_t ptr,count=0,flag=0;
	char ch,password[9],password_chk[9];
	
	IODIR1 |= 0x10000;
	IOSET1 |= 0X10000;
	
	UART0_INIT();
	LCD_INIT();
	
	
	UART0_TXstring("Set a password (8 digit max)(press enter when done): ");
	for(ptr=0;ptr<8;ptr++)
	{
		ch=UART0_RXchar();
		if(ch!='\n'&& ch!='\r')
			password[ptr]=ch;
		else
			break;
	}
	
	UART0_TXstring("Enter password (press enter when done): ");
	for(ptr=0;ptr<8;ptr++)
	{
		ch=UART0_RXchar();
		if(ch!='\n'&& ch!='\r')
			password_chk[ptr]=ch;
		else
			break;
	}
	
	for(;;)
	{

		if(strcmp(password,password_chk)==0 && flag==0)
		{
			UART0_TXstring("Welcome\n");
			flag=1;
		}
		else if(strcmp(password,password_chk)!=0)
		{			
			if(count==3)
			{
				IOCLR1 |= 0x10000;
				delay_ms(1000);
				IOSET1 |= 0x10000;
				delay_ms(1000);
				IOCLR1 |= 0x10000;
				delay_ms(1000);
				IOSET1 |= 0x10000;
				
				count++;
			}
			if(count<3)
			{
				UART0_TXstring("Wrong password\n");
				count++;
				UART0_TXstring("Enter password (press enter when done): ");
				for(ptr=0;ptr<8;ptr++)
				{
					ch=UART0_RXchar();
					if(ch!='\n'&& ch!='\r')
						password_chk[ptr]=ch;
					else
						break;
				}
			}
		}

		if(!(IOPIN1 & 0x20000))
		{
			delay_ms(200);
			if(IOPIN1 & 0x20000)
			{
				UART0_TXstring("Reset password (8 digit max)(press enter when done): ");
				for(ptr=0;ptr<8;ptr++)
				{
					ch=UART0_RXchar();
					if(ch!='\n'&& ch!='\r')
						password[ptr]=ch;
					else
						break;
				}
				count=0;
				flag=0;
				
				UART0_TXstring("Enter password (press enter when done): ");
				for(ptr=0;ptr<8;ptr++)
				{
					ch=UART0_RXchar();
					if(ch!='\n'&& ch!='\r')
						password_chk[ptr]=ch;
					else
						break;
				}
			count++;
			}
		}
	}
}
