#include "ClientList.h"
typedef vector<CSocket*>::iterator Iter; 

ClientList::ClientList()
{
	InitializeCriticalSection(&g_cs);//初始化g_cs的成员 
}

ClientList::~ClientList()
{
	DeleteCriticalSection(&g_cs);//删除关键段 
}

void ClientList::Add(CSocket* socket)
{
	EnterCriticalSection(&g_cs);//进入关键段
	if(socket!=NULL)
	{
		_list.push_back(socket);
		cout << "新上线一个用户，在线人数：" << _list.size() << endl;
	}
	LeaveCriticalSection(&g_cs);//退出关键段  
}

int ClientList::Count() const
{
	return _list.size();
}

CSocket* ClientList::operator[](size_t index)
{ 
	assert(index>=0 && index<_list.size()); 
	return _list[index];
}

void ClientList::Remove(CSocket* socket)
{ 
	EnterCriticalSection(&g_cs);//进入关键段
	Iter iter=Find(socket);
	if(iter!=_list.end())
	{ 
		delete *iter; 
		_list.erase(iter);

		cout << "一个用户下线，在线人数：" << _list.size() << endl;
	}
	LeaveCriticalSection(&g_cs);//退出关键段  
}

Iter ClientList::Find(CSocket* socket)
{
	EnterCriticalSection(&g_cs);//进入关键段
	Iter iter=_list.begin();
	while(iter!=_list.end())
	{
		if(*iter==socket)
		{
			LeaveCriticalSection(&g_cs);
			return iter;
		}
		iter++;
	}
	LeaveCriticalSection(&g_cs);//退出关键段  
	return iter;
}

void ClientList::Clear()
{
	EnterCriticalSection(&g_cs);//进入关键段
	for(int i=_list.size()-1;i>=0;i--)
	{
		delete _list[i];
	}
	_list.clear();
	LeaveCriticalSection(&g_cs);//退出关键段  
}
