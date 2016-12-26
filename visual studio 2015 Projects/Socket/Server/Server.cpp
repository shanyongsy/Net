//���ߣ���̫��
//���ͣ�http://www.cnblogs.com/hlxs/ 
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
			
			//_endthread(); //�û����ߣ���ֹ���������߳�
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

		// ���ռ�����Ϣ
		//cin>>buf;
		cin.getline(buf, BUF_LEN - 1);

		// ������Ϣ���ȣ�ʵ�ʳ��� + 1�ֽڵ�\0��
		WORD bufSize=0;
		while(buf[bufSize++]!='\0'); 

		// ����1��Word�����ݳ�������
		
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
	_beginthread(sends,0,list);//����һ���̹߳㲥����
	while(1)
	 {
		CSocket* csocket=server.Accept();
		
		list->Add(csocket);
		
		_beginthread(recv,0,csocket);//����һ���������ݵ��߳�
	 }
 
	getchar();
	return 0;
} 
