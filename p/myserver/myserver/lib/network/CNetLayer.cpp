//
//  CNetLayer.m
//  LocMon-SNS
//
//  Created by wdd on 13-2-25.
//  Copyright 2013妤�拷��_MyCompanyName__. All rights reserved.
//
/*
#include "CNetLayer.h"
#include "CDatConnect.h"


static CNetLayer *s_netlayer = NULL;

CNetLayer* CNetLayer::sharedNetLayer(void)
{
    if (!s_netlayer)
    {
        s_netlayer = new CNetLayer();

    }
    
    return s_netlayer;
}

CNetLayer::CNetLayer()
{
    m_num = 0;
    m_ret = 0;
    m_res.clear();
    m_req.clear();
    m_message.clear();

 
    
    m_error = NETWORK_ERROR;


    

//    std::string oldFile = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("Info.plist");
//    NSString *str = [NSString stringWithUTF8String:oldFile.c_str()];
//    NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithContentsOfFile:str];
//    NSLog(@"%@",dict);
//    NSString *str2 = [dict objectForKey:@"CFBundleVersion"];
//    char *value = (char*)[str2 UTF8String];
//
//
//    std::string sysver = [[[UIDevice currentDevice]systemVersion]UTF8String];
//
//    std::string snnum;
//    if(!GameConfig::readXml("reg", "snnum", snnum))
//    {
//        CFUUIDRef puuid = CFUUIDCreate(nil);
//        CFStringRef uuidstring = CFUUIDCreateString(nil, puuid);
//        NSString *result = (NSString*)CFStringCreateCopy(NULL, uuidstring);
//        snnum =[result UTF8String];
//        CFRelease(puuid);
//        CFRelease(uuidstring);
//    }
    
    m_commfield.appver = "1.0";
    m_commfield.snnum = "1.0";//m_commfield.snnum;
    m_commfield.sysver = "1.0";
    
    pthread_mutex_init(&_mutex, NULL);
}

CNetLayer::~CNetLayer()
{
    pthread_mutex_destroy(&_mutex);

    
    
    
        
}


void CNetLayer::checkret()
{

    if(m_ret != 0)
    {

       
        
    
        
        if(m_ret == 2)
        {
            for (; m_num < m_req.size(); m_num ++ ) 
            {
    
            }
            m_res.clear();
            m_req.clear();
            m_message.clear();
            m_ret = 0;
            m_num = 0;
            return;
        }
        m_ret = 0;
        if(m_num != m_req.size())
        {
            pthread_create(&_thread, NULL, request, this);
        }
        else
        {
            m_num = 0;
            m_res.clear();
            m_req.clear();
            m_message.clear();
        }
        
        
        
    }
    
}

int CNetLayer::getError()
{
    return m_error;
}



std::string CNetLayer::getErrorStr()
{
    std::string str_eror;
    switch (m_error) {
        case NETWORK_ERROR:
            GameConfig::loadCharacter("net_error", str_eror);
            break;
        case NETWORK_NOUSER:
            GameConfig::loadCharacter("net_nouse", str_eror);
            break;
        case NETWORK_PSDERROR:
            GameConfig::loadCharacter("net_psderror", str_eror);
            break;
        case NETWORK_NOREG:
            GameConfig::loadCharacter("net_noreg", str_eror);
            break;
        case NETWORK_RUSER:
            GameConfig::loadCharacter("net_ruser", str_eror);
            break;
        case NETWORK_RLOGIN:
            GameConfig::loadCharacter("net_rlogin", str_eror);
            break;
        case NETWORK_REDEVEICE:
            GameConfig::loadCharacter("net_redeveice", str_eror);
            break;
        case NETWORK_VERUP:
            GameConfig::loadCharacter("net_verup", str_eror);
            break;
        case NETWORK_ERRORNO:
            GameConfig::loadCharacter("net_errorno", str_eror);
            break;
        default:
            GameConfig::loadCharacter("net_errorno", str_eror);
            break;
    }
    return  str_eror;
}



void CNetLayer::releasePtr()
{
   
 
}

void CNetLayer::myRequest(CDatRequest *pReq, CDatResponse *pRes,std::string message ,CCLayer *layer ,int num )
{
    CCLog("%d",m_req.size());
    if(m_req.size() > 0)
    {
  
        m_message.push_back(message);
        m_req.push_back(pReq);
        m_res.push_back(pRes);
        return;
    }
    else {
      
        m_message.push_back(message);
        m_req.push_back(pReq);
        m_res.push_back(pRes);

    }
    
    m_error = NETWORK_ERROR;
    
    pthread_create(&_thread, NULL, request, this);

    
    

}

void CNetLayer::removeNetAct()
{




}

void* CNetLayer::request(void *data)
{

    CNetLayer *netlayer = (CNetLayer*)data;

    CDatConnect *m_cdat = new CDatConnect;
    m_cdat->setCommon(netlayer->m_commfield);



    int m = netlayer->m_num;
    if(m > netlayer->m_req.size())
    {
//        CCMessageBox("Message error", "error");
    }
    netlayer->m_num ++;
    if(m_cdat->DoRequest(netlayer->m_req[m],netlayer->m_res[m], 3))
    {


        netlayer->m_error = netlayer->m_res[m]->getRetcode();
        netlayer->m_ret = 1;



    }
    else
    {
        netlayer->removeNetAct();
        netlayer->m_ret = 2;
    }

    delete (m_cdat);

}

*/




