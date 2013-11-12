//
//  CNetLayer.h
//  LocMon-SNS
//
//  Created by wdd on 13-2-25.
//  Copyright 2013骞�__MyCompanyName__. All rights reserved.
//
#pragma once



#include "pthread.h"
#include "CDatPacket.h"
#include "CDatConnect.h"

#define NETWORK_OK 1
#define NETWORK_NOUSER 2   //
#define NETWORK_PSDERROR 3 //
#define NETWORK_ERROR 4   //
#define NETWORK_NOREG  101  //
#define NETWORK_RUSER  102 //
#define NETWORK_RLOGIN 103 //
#define NETWORK_REDEVEICE 104 //
#define NETWORK_VERUP  105 //
#define NETWORK_ERRORNO 999 //


class CNetLayer
{
public:

    CNetLayer();
    ~CNetLayer();

    static CNetLayer* sharedNetLayer(void);
    

    void releasePtr();
    void removeNetAct();
    void checkret();
    int getError();
    std::string getErrorStr();
    void myRequest(CDatRequest *pReq, CDatResponse *pRes,std::string message = "",int num = 1);
    static void* request(void *data);

    
    int m_ret;
    int m_num;
    std::vector<std::string> m_message;
    std::vector<CDatRequest*> m_req;
    std::vector<CDatResponse*> m_res; 

   
    
    CommRequest m_commfield;






    
    int m_error;

    int m_type;
    pthread_t           _thread;
    pthread_mutex_t     _mutex;
};
