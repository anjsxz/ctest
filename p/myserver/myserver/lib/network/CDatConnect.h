
#pragma once

#include "stdafx.h"



//#define LINK_MYURL  "map.mylocmon.com"
//#define LINK_MYURL  "10.25.2.12"
#define LINK_MYURL "192.168.1.254"
//#define LINK_MYURL "121.199.28.225"
//#define LINK_MYURL "192.168.1.14"
//#define LINK_MYURL "12sx.boyojoy.com"


#define MYSERVER_PORT 6001
#define BUFFER_SIZE 2048

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "CDatPacket.h"



class CDatConnect
{
public:
    CDatConnect();
    ~CDatConnect();   
    bool Connect();
    bool DoRequest(CDatRequest *pReq, CDatResponse *pRes,int num);
    void setCommon(CommRequest comm);
    bool ConnectTcp();
    bool ConnectUdp();
    
    static void* request(void *data);
private:
    CommRequest m_commfield;
    char *pSend;
    CMemBuffer *pMyBuf;
    
    char *pRecv;
    int len;
    int m_reclen;
    bool m_bzip;
};

