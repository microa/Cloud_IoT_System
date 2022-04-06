/******************************************************
*Title:Tcp Server of Rcv.Log
*Version:1.0
*Date:March.17,2015
*@Home,Origin
******************************************************/  
#include "stdafx.h"
#include <iostream>
#include <WinSock2.h>
#include <string.h>
#pragma comment(lib,"WS2_32.lib")
#define Error -1
#define Ture 1
#define False 0
#define Port 1013
using namespace std;

const int Buf_Size = 128;
WSADATA WSD;				//WSADATA Variable
SOCKET SvrSock;				//Server Socket
SOCKET ClientSock;			//Client Socket
SOCKADDR_IN addrServ;;		//Server Address
char RcvBuf[Buf_Size];		//Receive Buffer
char SendRcvBuf[Buf_Size];	//Return to Cient Buffer
int retVal;					//Return Value
sockaddr_in addrClient[64];
char Counter=0;
unsigned long MyMode = 0;
int iResult;

void FileOpt(char *str)
{
	FILE *pFp_Content;
	char ch='a';
	unsigned long Counter=0;
	if(!(pFp_Content = fopen("C://TcpLog.bin","wb")))      //open/create a empty file,mode:write binary
	{
		cout<<"Can't Create the file."<<endl;
		exit(0);    //ending program
	}
	fputs(str,pFp_Content);     //write on new file
	Counter=(unsigned long)strlen(str);		//Count lenth of the string
	cout<<"Writed Bytes :\t"<<Counter<<endl;
	fclose(pFp_Content);		//close file
	cout<<"File Option Finished!"<<endl;
}
int Init()
{
    SendRcvBuf[0]=Error;
	RcvBuf[0]=Error;
	//1.Initialization Socket
    if (WSAStartup(MAKEWORD(2,2), &WSD) != 0)
    {
        cout<<"WSAStartup Failed!"<<endl;
        return 1;
    }
	else cout<<"WSAStartup Success!"<<endl;
	//2.Creat Socket
    SvrSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(INVALID_SOCKET == SvrSock)
    {
        cout<<"Socket Failed!"<<endl;
        WSACleanup();		//Release Socket
        return  Error;
    }
	else cout<<"Socket Success!"<<endl;
	iResult = ioctlsocket(SvrSock, FIONBIO, &MyMode);
	if (iResult != NO_ERROR)
	{
		printf("ioctlsocket failed with error: %ld\n", iResult);
	}
	//3.Set Server Socket Address
    addrServ.sin_family = AF_INET;
    addrServ.sin_port = htons(Port);
    addrServ.sin_addr.s_addr = INADDR_ANY;
	//Bind Socket
    retVal = bind(SvrSock, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
    if(SOCKET_ERROR == retVal)
    {
        cout<<"Bind Failed!"<<endl;
        closesocket(SvrSock);					//Close Socket
        WSACleanup();							//Release Socket
        return Error;
    }
	else cout<<"Bind Success!"<<endl;
	//Start to Listen
    retVal = listen(SvrSock, 1);
    if(SOCKET_ERROR == retVal)
    {
        cout<<"Listen Failed!"<<endl;
        closesocket(SvrSock);					//Close Socket
        WSACleanup();							//Release Socket
        return Error;
    }
	else cout<<"Begin to Listen..."<<endl;
	return 0;
}
int Connect()
{
    //Accept Connect
    int addrClientlen = sizeof(addrClient[0]);
    ClientSock = accept(SvrSock,(sockaddr FAR*)&addrClient, &addrClientlen);
    if(INVALID_SOCKET == ClientSock)
    {
        cout<<"Accept Failed!"<<endl;
        closesocket(SvrSock);
        WSACleanup();
        return Error;
    }
	else cout<<"Accept Success!"<<endl;
    return 0;
}
int Dialog()
{
	//Accept Client
	ZeroMemory(RcvBuf, Buf_Size);
	retVal = recv(ClientSock, RcvBuf, Buf_Size, 0);
	if (SOCKET_ERROR == retVal)
	{
		cout<<"Rcv Failed!"<<endl;
		return 0;
	}
	if(RcvBuf[0] == '0')
	{
		return 0;
	}
	cout<<"Received: "<<RcvBuf <<endl;
	if(strlen(RcvBuf) >= 1)
	{
		strcpy(SendRcvBuf,RcvBuf);			//copy receive buffer to send buffer
	}
	if(strlen(SendRcvBuf) >= 1)
	{
		FileOpt(SendRcvBuf);				//save received content
		send(ClientSock,"Sent data are confirmed.\r\n", strlen("Sent data are confirmed.\r\n"), 0);
	}
	return 1;
}
int main(int argc, char* argv[])
{
	int End=1;
	while(1)
	{
		End=1;
		Init();
		Connect();
		while(Ture)
		{
			if(End)
			{
				End=Dialog();
			}
			else
			{
				break;
			}
		}
		//Release Sources
		closesocket(SvrSock);		//Close Server Socket
		closesocket(ClientSock);	//Close Client Socket
		WSACleanup();
		cout<<"Disconnect!"<<endl;
	}
    return 0;
}
