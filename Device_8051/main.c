/*******************************************************
*	Title:STC12C Driver,TFT Screen,ESP8266 WiFi Module
*	Version:Original Ver.
*	Release Note:
*	Date:Mar~May,2015@Home,Origin.
*******************************************************/
#include <STC12C5A60S2.H>
#include <intrins.h>
#include <sys.C>
#include <font.C>
#include <lcd.H>
#include <lcd.C>
#include <DS18B20.C>
#include <UART.C>
void DispOne(u16 x,u16 y,u8 num,u8 mode)//Block Display A Char
{
	x*=8;
	y*=16;
	LCD_ShowChar(x,y,num,mode);
}
void DispStr(u16 x,u16 y,const u8 *p) 	//Block Display A string
{   
	x*=8;
	y*=16;
  LCD_ShowString(x,y,p);
}
void DispTemp(short Tx)			//Display Temprature
{
	unsigned char H,T,U,D;
	POINT_COLOR=WHITE;
	H=Tx/1000;
	T=Tx/100%10;
	U=Tx/10%10;
	D=Tx%10;
	if(Tx>=0)
	{
		DispStr(6,12,'+');
	}
	else
	{
		DispStr(6,12,'-');
	}
	DispOne(7,12,Num[H],0);
	DispOne(8,12,Num[T],0);
	DispOne(9,12,Num[U],0);
	DispOne(10,12,'.',0);
	DispOne(11,12,Num[D],0);
	DispOne(13,12,'C',0);
	//while(1);
	POINT_COLOR=GREEN;
}
void ITR_Init()			//Interrupt Init
{
	EA=1;
	TMOD|=0x01;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	ET0=1;
	TR0=1;
}
void Delay(unsigned int Counter)
{
	delayms(Counter);
}
void AT_CMD()
{
	SendString("AT+CWMODE=1\r\n");		Delay(5000);	DispOne(10,5,'.',0);
	SendString("AT+RST\r\n");					Delay(5000);	DispOne(11,5,'.',0);
	SendString("AT+CIPMODE=1\r\n");		Delay(5000);	DispOne(12,5,'.',0);
	SendString("AT+CIPMUX=0\r\n");		Delay(5000);	DispOne(13,5,'.',0);
	SendString("AT+CWLAP\r\n");				Delay(5000);	DispOne(14,5,'.',0);
	SendString("AT+CWJAP=\"Route Device Name\",\"PassWord\"\r\n");		Delay(5000);	DispOne(15,5,'.',0);//change to suit your net environment
	SendString("AT+CWJAP?\r\n");			Delay(5000);	DispOne(16,5,'.',0);
	SendString("AT+CIFSR\r\n");				Delay(5000);	DispOne(17,5,'.',0);
	SendString("AT+CIPSTART=\"TCP\",\"*.*.*.*\",Port\r\n");	Delay(5000);	DispOne(18,5,'.',0);//the *.*.*.* is your server address,And don't forget the Port
	SendString("AT+CIPSEND\r\n");			Delay(5000);	DispOne(19,5,'.',0);
	SendString("Acquire Info! From:STC12\r\n");		Delay(5000);	DispOne(20,5,'.',0);
}
void Send_TempVal(short Tv)
{
	char STmp[5];
	STmp[0]='T';
	STmp[1]=Num[Tv/1000];
	STmp[2]=Num[Tv/100%10];
	STmp[3]=Num[Tv/10%10];
	STmp[4]=Num[Tv%10];
	SendString(STmp);
	SendString("\r\n");
}
void main()
{ 
	Lcd_Init();   //TFT Screen Init
	LCD_Clear(BLACK); //fill screed with black color
	BACK_COLOR=BLACK;
	POINT_COLOR=GREEN;
	DispStr(0,0,"Screen Initialized!");
	DispStr(0,1,"MCU:12C5A60S2,Speed:1MIPS"); //Display MCU Information
	if(ReadTemp()!=0)
	{
		DispStr(0,2,"DS18B20 Ready!");
	}
	else
	{
		DispStr(0,2,"Ds18b20 Error.");
	}
	UART_Init();					//UART Init
	DispStr(0,3,"UART Ready!");
	ITR_Init();						//Interrupt Init
	DispStr(0,4,"Interrupt Onset.");
	DispStr(0,5,"Connecting");
	AT_CMD();							//ESP8266 Init
	DispStr(0,6,"Connectted!");
	DispStr(0,12,"Real:");
	while(1)
	{
		TempVal=ReadTemp();			//Read Celsius
		DispTemp(TempVal);			//Display Celsius Value
		if(Cnt>30)
		{
			Send_TempVal(TempVal);		//Send Temprature to UART
			Cnt=0;
		}
	}

}
void ITR1()interrupt 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	Cnt++;
}
