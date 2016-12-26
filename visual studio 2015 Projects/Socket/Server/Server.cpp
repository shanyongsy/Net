//作者：陈太汉
//博客：http://www.cnblogs.com/hlxs/ 
#include <windows.h>
#include <process.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib") 
#include "SServer.h"
#include "CSocket.h"
#include <vector>
#include "ClientList.h"
const int BUF_LEN=1024;
const int LISTEN_PORT = 2017;
void recv(PVOID pt)
{
	CSocket* csocket=(CSocket*)pt;
	if(csocket!=NULL)
	{
		int count= csocket->Receive(BUF_LEN); 
		if(count==0)
		{  
			ClientList* list=ClientList::GetInstance(); 
			list->Remove(csocket);
			
			//_endthread(); //用户下线，终止接收数据线程
		}
	} 
}

void sends(PVOID pt)
{
	ClientList* list=(ClientList*)pt;
	while(1)
	{
		char* buf=new char[BUF_LEN] ;
		memset(buf, 0, BUF_LEN);

		// 接收键盘消息
		//cin>>buf;
		cin.getline(buf, BUF_LEN - 1);

		// 计算消息长度（实际长度 + 1字节的\0）
		WORD bufSize=0;
		while(buf[bufSize++]!='\0'); 

		// 加入1个Word的数据长度数据
		
		memcpy_s(buf + sizeof(bufSize), bufSize, buf, bufSize);
		bufSize += sizeof(bufSize);
		memcpy_s(buf, sizeof(bufSize), &bufSize, sizeof(bufSize));



		for(int i=list->Count()-1;i>=0;i--)
		{
			(*list)[i]->Send(buf,bufSize);  
		} 
		delete buf;
	}
}
 
int main(int argc, char* argv[])
{
	SServer server;
	bool isStart=server.Start(LISTEN_PORT);
	if(isStart)
	{
		cout<<"server start success...listen port is "<< LISTEN_PORT << endl;
	}else
	{
		cout<<"server start error"<<endl;
	} 
	ClientList* list=ClientList::GetInstance();
	_beginthread(sends,0,list);//启动一个线程广播数据
	while(1)
	 {
		CSocket* csocket=server.Accept();
		
		list->Add(csocket);
		
		_beginthread(recv,0,csocket);//启动一个接收数据的线程
	 }
 
	getchar();
	return 0;
} 
