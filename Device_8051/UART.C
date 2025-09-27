#define FOSC 12000000
#define BAUD 9600

#define NONE_PARITY     0   //无校验位
#define ODD_PARITY      1   //奇校验
#define EVEN_PARITY     2   //偶校验
#define MARK_PARITY     3   //标记校验
#define SPACE_PARITY    4   //空校验

#define PARITYBIT NONE_PARITY

#define S2RI  0x01
#define S2TI  0x02
#define S2RB8 0x04
#define S2TB8 0x08


bit busy;

void SendData(char dat);
void SendString(char *s);

void UART_Init()
{
#if (PARITYBIT == NONE_PARITY)
    S2CON = 0x5a;               //8位可变波特率 (无校验位)
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    S2CON = 0xda;               //9位可变波特率,校验位初始为1
#elif (PARITYBIT == SPACE_PARITY)
    S2CON = 0xd5;               //9位可变波特率,校验位初始为0
#endif

    BRT = -(FOSC/32/BAUD);      //设置独立波特率发生器的重载初值
    AUXR = 0x14;                //独立波特率发生器工作在1T模式
    IE2 = 0x01;                 //使能串口2中断
    EA = 1;                     //开总中断
		
		//SendString("Tsiov Test");
    
}

void Uart2() interrupt 8 using 1
{
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;         //清除接收完成标志
        Rcv = S2BUF;             //P0显示串口数据
//        P2 = (S2CON & S2RB8);   //P2.2显示校验位
    }
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;         //清除发送完成标志
        busy = 0;
    }
}

void SendData(char dat)
{
    while (busy);               //等待上个数据发送完成
    ACC = dat;                  //取得偶校验位P
    if (P)						//根据P来设置串口数据的校验位
    {
#if (PARITYBIT == ODD_PARITY)
        S2CON &= ~S2TB8;        //置校验位为0
#elif (PARITYBIT == EVEN_PARITY)
        S2CON |= S2TB8;         //置校验位为1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        S2CON |= S2TB8;         //置校验位为1
#elif (PARITYBIT == EVEN_PARITY)
        S2CON &= ~S2TB8;        //置校验位为0
#endif
    }
    busy = 1;
    S2BUF = ACC;                //发送数据
}

void SendString(char *s)
{
    while (*s)                  //判断字符串结束标志
    {
        SendData(*s++);         //发送字符
    }
}
