/***********************************DS18B20.Head***************************************
*	Creat Date:20140116.PM
*	Revise Date:March,14,2015
*	Ver:20150314
*	Function:Return a integer PlusMinuserature value.
*	         Such as 34.2 C,it'll return 342.
*	Release Note:Revise from my old code,Jan.16,2014@LYUN
*	Release Note:Debug.Repair the negative celsius bug,Mar.14,2015@Home,Origin
**********************************End Head Explan*************************************/
sbit DQ=P2^7;
//bit Note=0;
void DelayNuts(unsigned long Nuts)
{
	while(Nuts--);
}
void RESET_DS18B20(void)
{
	unsigned char x=0;
	DQ=1;
	DelayNuts(8);
	DQ=0;
	DelayNuts(85);
	DQ=1;
	DelayNuts(14);
	x=DQ;
	DelayNuts(20);	   
}
void WriteByte(unsigned char dat)  
{
	unsigned char i=0;
	for(i=8;i>0;i--)
		{
			DQ=0;
			DQ=dat&0x01; 
			DelayNuts(5);
			DQ=1;
			dat>>=1;
			
		}
	DelayNuts(4);
}
unsigned char Read(void)
{
	unsigned char i=0;
	unsigned char ReadData=0;
	for(i=8;i>0;i--)
		{
			DQ=0;
			ReadData>>=1;
			DQ=1;
			if(DQ)
			ReadData|=0x80;
			DelayNuts(5);
		}
	return(ReadData);
}  
unsigned int ReadTemp(void)     
{
  unsigned char PlusMinus;
  unsigned char LSB=0,MSB=0;
  short Temp=0;
	RESET_DS18B20();
	WriteByte(0xCC);
	WriteByte(0x44); 
	DelayNuts(125);	
	RESET_DS18B20();
	WriteByte(0xCC);
	WriteByte(0xBE);
	LSB=Read(); 
	MSB=Read();
	if(MSB>7)
    {
        MSB=~MSB;
        LSB=~LSB; 
        PlusMinus=0;
    }else PlusMinus=1;	  
    Temp=MSB;
    Temp<<=8;    
    Temp+=LSB;
    Temp=(float)Temp*0.625;    
	if(PlusMinus)return Temp; 
	else return -Temp;
}