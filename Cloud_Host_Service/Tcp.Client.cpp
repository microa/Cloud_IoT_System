/******************************************************
*Title:Tcp Client
*Version:0.0
*Date:May.16,2015
*@Home,Origin
******************************************************/
#include <iostream>
#include <WINSOCK2.H>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
char IP_Addr[15];	//IP buffer
int Port;			//Port
char Msg[128];		//message
char RcvBuf[128];	//Receive buffer
void main()
{
	int err;
	WORD versionRequired;
	WSADATA wsaData;
	versionRequired=MAKEWORD(1,1);
	err=WSAStartup(versionRequired,&wsaData);//version of the protocal
	if (!err)
	{
		printf("Created Client Socket!\n");
	}
	else
	{
		printf("A unknow false occurred!\n");
		return;
	}
	SOCKET clientSocket=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN clientsock_in;
	cout<<"Input IP:";		//display hint of operation
	cin>>IP_Addr;			//save ip buffer
	clientsock_in.sin_addr.S_un.S_addr=inet_addr(IP_Addr);	//setting ip address
	clientsock_in.sin_family=AF_INET;		//Address family
	cout<<"Input Port:";
	cin>>Port;
	clientsock_in.sin_port=htons(Port);
	//bind(clientSocket,(SOCKADDR*)&clientsock_in,strlen(SOCKADDR));//Bind
	//listen(clientSocket,5);
	connect(clientSocket,(SOCKADDR*)&clientsock_in,sizeof(SOCKADDR));//Begin to link
	//recv(clientSocket,receiveBuf,101,0);
	//printf("%s\n",receiveBuf);
	while(1)
	{
		cout<<"New Message:";
		//getchar();
		//gets(Msg);		//two lines equals cin.getline(Msg,128);
		cin.getline(Msg,128);
		send(clientSocket,Msg,strlen(Msg)+1,0);
		cout<<"Message sent successfully!"<<endl;		//Report
		//recv(clientSocket,RcvBuf,129,0);			//Receive function
	}
	
	closesocket(clientSocket);			//close socket
	WSACleanup();					//end and unleash resources
	
}


