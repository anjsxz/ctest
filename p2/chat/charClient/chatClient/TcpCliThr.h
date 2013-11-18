#ifndef TCPCLITHR_H
#define TCPCLITHR_H


#include <stdio.h>
#include <vector>
#include <pthread.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>


#include "mysocket.h"
#include "mythread.h"
#include "tcpclithr.h"
#include "mysync.h"

using namespace std;

//#define ETH_NAME "eth1"

extern const int MAX_MESSAGE_LEN;
extern const int DEFAULT_PORT;

class TcpCliThr : public MySocket
{
	vector <MyThread*> *ThrSet;
    
public:
    
	TcpCliThr();
	TcpCliThr(int port, char *server);
	virtual ~TcpCliThr();
    
	int ConnectServ();
	virtual void DealRecv(MyThread *thread);
	virtual void DealSend(MyThread *thread);
	int CreateThr(MyThread **, MyThread **);
	void AddThread(MyThread *thread);
	void DelThread(MyThread *thread);
	int WaitAllThr();
	char *getMessage(char *buf, int len, FILE *fp);
    
	class Receiver : public MyThread
	{
	public:
        
		int socket;
		TcpCliThr *server;
        
		Receiver(int connsocket, TcpCliThr *serv) {
			socket = connsocket;
			server = serv;
		}
		void run() { server->DealRecv(this); }
	};
    
	class Sender : public MyThread
	{
	public:
        
		int socket;
		TcpCliThr *server;
        
		Sender(int connsocket, TcpCliThr *serv) {
			socket = connsocket;
			server = serv;
		}
		void run() { server->DealSend(this); }
	};
};


#endif