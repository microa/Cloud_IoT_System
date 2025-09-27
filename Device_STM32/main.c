#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h"
#include "exti.h"
#include "wdg.h"
#include "timer.h"
#include "lcd.h"	   		
void LEDCtrl()		//LED Control
{
	if(USART_RX_BUF[0]=='R'&&USART_RX_BUF[1]=='E'&&USART_RX_BUF[2]=='D')
	{
		LED0=0;
	}
	if(USART_RX_BUF[0]=='R'&&USART_RX_BUF[1]=='E'&&USART_RX_BUF[2]=='D'
		&&USART_RX_BUF[3]=='O'&&USART_RX_BUF[4]=='F'&&USART_RX_BUF[5]=='F')
	{
		LED0=1;
	}
	if(USART_RX_BUF[0]=='G'&&USART_RX_BUF[1]=='R'&&USART_RX_BUF[2]=='E'
		&&USART_RX_BUF[3]=='E'&&USART_RX_BUF[4]=='N')
	{
		LED1=0;
	}
	if(USART_RX_BUF[0]=='G'&&USART_RX_BUF[1]=='R'&&USART_RX_BUF[2]=='E'
		&&USART_RX_BUF[3]=='E'&&USART_RX_BUF[4]=='N'&&USART_RX_BUF[5]=='O'
		&&USART_RX_BUF[6]=='F'&&USART_RX_BUF[7]=='F')
	{
		LED1=1;
	}
}
int main(void)
{		
	u8 x=0,u8LastLen=0x00,len=0xFF;
	u8	u8Lop,u8Sec=5;
	u16	u16Line=0; 
	u32 u32Tick=0,u32Mode=0;
	u8 Last_Data[64];
  Stm32_Clock_Init(9);//System Clock Set
	delay_init(72);		//Delay Function Initialization
	uart_init(72,9600); //Init UART Port 1
	LED_Init();
 	LCD_Init();
	POINT_COLOR=GREEN;	
	LCD_Clear(BLACK);	
	LCD_ShowString(0,u16Line,"Welcome Access!");	
	u16Line+=16;
	LCD_ShowString(0,u16Line,"System Ready!");
	u16Line+=16;
	printf("AT+CWMODE=1\r\n");
	LCD_ShowString(0,u16Line,"First Signal Sent!");
	u16Line+=16;
	LCD_ShowString(0,u16Line,"Waiting Response!");
	u16Line+=16;
  while(1) 
	{
		if(USART_RX_STA&0x80)
		{
			len=USART_RX_STA&0x3f;			//get the lenth of receive data
			if((Last_Data[0]!=USART_RX_BUF[0])||(u8LastLen!=len))	//if it's same to previous data,then don't display.
			{
				LCD_ShowString(0,u16Line,USART_RX_BUF);
				u16Line+=16;
			}
			LEDCtrl();
			u8LastLen=len;
			while((USART1->SR&0X40)==0);//Wait Sent.
			USART_RX_STA=0;
			for(u8Lop=0;u8Lop<64;u8Lop++)
			{
				Last_Data[u8Lop]=USART_RX_BUF[u8Lop];
				USART_RX_BUF[u8Lop]=0;
			}
			if(u16Line>318)			//Judge the end of 240*320 screen line;
			{
				u16Line=0;
				LCD_Clear(BLACK);
			}
		}
		u32Mode++;
		u32Tick=u32Mode;
		if(u32Mode>11)
		{
			u32Mode=10;
		}
		switch (u32Tick)
		{
			case 1:printf("AT+CWMODE=1\r\n");	u8Sec=5;u32Tick=80;break;
			case 2:printf("AT+RST\r\n");			u8Sec=5;u32Tick=80;break;
			case 3:printf("AT+CIPMODE=1\r\n");u8Sec=5;u32Tick=80;break;
			case 4:printf("AT+CIPMUX=0\r\n");	u8Sec=5;u32Tick=80;break;
			case 5:printf("AT+CWLAP\r\n");		u8Sec=5;u32Tick=80;break;
			case 6:printf("AT+CWJAP=\"Route's Name\",\"Route's Password\"\r\n");	u8Sec=5;u32Tick=80;break;//change to suit your net environment
			case 7:printf("AT+CWJAP?\r\n");		u8Sec=5;u32Tick=80;break;
			case 8:printf("AT+CIFSR\r\n");		u8Sec=5;u32Tick=80;break;
			case 9:printf("AT+CIPSTART=\"TCP\",\"Your Server\",Port\r\n");u8Sec=5;u32Tick=80;break;//the *.*.*.* is your server address,And don't forget the Port
			case 10:printf("AT+CIPSEND\r\n");	u8Sec=5;u32Tick=80;break;
			case 11:printf("Acquire Info! From:STM32\r\n");u8Sec=5;u32Tick=80;break;
			default:break;
		}
		for(u8Lop=0;u8Lop<u8Sec+1;u8Lop++)delay_ms(1000);
	  x++;
		if(x==12)x=0;
		//LED0=!LED0;					 
		delay_ms(100);	
	}											    
}

























