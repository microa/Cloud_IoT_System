/******************************************************
*Title:Tcp Server of Send.Log
*Version:1.4
*Date:March.27,2015
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
#define Port 1014
using namespace std;

const int Buf_Size = 128;
WSADATA WSD;				//WSADATA Variable
SOCKET SvrSock;				//Server Socket
SOCKET ClientSock;			//Client Socket
SOCKADDR_IN addrServ;;		//Server Address
char RcvBuf[Buf_Size];		//Receive Buffer
char SendRcvBuf[Buf_Size];	//Return to Client
int retVal;
sockaddr_in addrClient[64];
char Counter=0;
unsigned long MyMode = 0;
int iResult;
char str[128]={0};
int iLop;

void FileOpt()
{
	FILE *fFp_Content;
	char s;
	unsigned long Counter=0;
	long lFilelen;
	if(!(fFp_Content = fopen("C://TcpLog.bin","rb")))      //open/read a empty file,mode:read binary
	{
		cout<<"Can't Read the file."<<endl;
		exit(0);	//ending program
	}
	cout<<"Opened Source File!"<<endl;
	fseek(fFp_Content,0,SEEK_SET);
	fseek(fFp_Content,0,SEEK_END);
	lFilelen=ftell(fFp_Content);		// longBytes is the lenth of File
	for(s=0;s<lFilelen;s++)
	{
		fseek(fFp_Content,s,SEEK_SET);		//set point in file
		str[s]=fgetc(fFp_Content);
	}
	str[lFilelen]='\r';
	str[lFilelen+1]='\n';
	cout<<"Read Bytes :\t"<<lFilelen<<endl;
	fclose(fFp_Content);				//close file
	cout<<"File Option Accomplished!"<<endl;
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
        closesocket(SvrSock);
        WSACleanup();
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
		FileOpt();							//save received content
		send(ClientSock,str, strlen(str), 0);
		for(iLop=0;iLop<128;iLop++)
		{
			str[iLop]=0;
		}
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
		/Release Sources
		closesocket(SvrSock);		//Close Server Socket
		closesocket(ClientSock);	//Close Client Socket
		WSACleanup();
		cout<<"Disconnect!"<<endl;
	}
    return 0;
}
