#define ID_ADDR_RAM 0xF1	//MCU ID in RAM address:0xF1

#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long

typedef unsigned char BYTE;
typedef unsigned int WORD;

unsigned char code Num[]={"0123456789"};
unsigned char ID[7];
short TempVal=0;
unsigned int Cnt=0;
char Tick=0;
char Rcv=0;


void delayms(unsigned int count)  // /* X1ms */
{
        int i,j;
        for(i=0;i<count;i++)
                for(j=0;j<1000;j++);
}



/*
void MCU_ID()
{
	BYTE *iptr;
	char cLop;
	iptr=ID_ADDR_RAM;
	for(cLop=0;cLop<7;cLop++)
	{
		ID[cLop]=(*iptr++);
	}
}*/