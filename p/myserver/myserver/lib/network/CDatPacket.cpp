//
//  MyCocos2DClass.m
//  LocMon-SNS
//
//  Created by wdd on 13-2-19.
//  Copyright 2013骞�__MyCompanyName__. All rights reserved.
//
#include "stdafx.h"
#include  "CDatPacket.h"
#include <zlib.h>
#include "ZipUtils.h"

#if CC_TEXTURE_ATLAS_USE_VAO
#include "iconv.h"
#else
#endif

int timeStart=0;
int g_sysTimer=0;
CDatResponse::CDatResponse(void)
{
    m_pData = new CMemBuffer;
    memset(&m_head, 0, sizeof(m_head));
}
CDatResponse::~CDatResponse(void)
{
    if(m_pData)	m_pData->ReleaseBuffer();
}


CDatRequest::CDatRequest()
{
//    m_commField.retcode = 4;
}

CDatRequest::~CDatRequest()
{
    
}

#pragma mark Regist
CDatRegistResponse::CDatRegistResponse(CDatRegistRequest*req)
{
    m_req = req;
}

int CDatRegistResponse::Marshal()
{
    json::Json jsonSend;
    json::Json jsonCommon;
    
    jsonCommon["code"] = "01";
    jsonCommon["appver"] = m_req->m_commField.appver.c_str();
    jsonCommon["msg"] = "Success";
    jsonCommon["retcode"] = 1;
    jsonCommon["systime"] = (int)time(NULL);
    
    json::JsonValue jv = jsonCommon;
    jsonSend["commresponse"] = jv;
    
    jsonSend["lang"]="en";
    jsonSend["otherid"]="0";
    jsonSend["othername"]="Player-9145";
    jsonSend["sex"]=1;
    jsonSend["userid"]=9145;
    jsonSend["username"]="Player-9145";
    std::string strJson;
  
    jsonSend.Dump(strJson);
    m_head.key = HEAD_KEY;
    m_head.uLen = strlen(strJson.c_str()) ;
    m_head.cZip = 1;
    
    char *pBuf = m_pData->GetBuffer(sizeof(m_head) + m_head.uLen);
    memcpy(pBuf, &m_head, sizeof(m_head));
    
    pBuf += sizeof(m_head);
    memcpy(pBuf, strJson.c_str(), m_head.uLen);
    printf("send:%s\n",pBuf);

    char *out = NULL;
    char *in = m_pData->GetData()+sizeof(DAT_HEAD);
   int len = ZipUtils::ccDeflateMemory((unsigned char*)in, m_pData->GetLength()-sizeof(DAT_HEAD),(unsigned char**) &out);
    
    if(len<=0)
        return -1 ;
    
    for(int i=0;i<len;i++)
    {
        out[i]+=(BYTE)i;
    }
    
    len+=sizeof(DAT_HEAD);
    m_pSendLen=len;
    m_pSend = (char*)malloc(len);
    memcpy(m_pSend, m_pData->GetData(), sizeof(DAT_HEAD));
    memcpy( m_pSend+sizeof(DAT_HEAD), out, len-sizeof(DAT_HEAD));
    
    DAT_HEAD *pHead = (DAT_HEAD*)m_pSend;
    pHead->uLen = len-sizeof(DAT_HEAD);
    
    return len;
    
}

CDatRegistRequest::CDatRegistRequest(json::Json &jc){
   UnMarshal(jc);
}

int CDatRegistRequest::UnMarshal(json::Json &jc)
{

    return 1;
}
//ReqLogin *CDatRegistResponse::GetUserInfo()
//{
//    return &m_reqRegist;
//}

#pragma mark Login

CDatLoginRequest::CDatLoginRequest(json::Json&jc)
{
    UnMarshal(jc);
}
int CDatLoginRequest::UnMarshal(json::Json&jc)
{
    
    return 0;
}
CDatLoginResponse::CDatLoginResponse(CDatLoginRequest*req)
{
    m_req=req;
}
int CDatLoginResponse::Marshal()
{
    json::Json jsonSend;
    json::Json jsonCommon;
    
    
    jsonCommon["code"] = "02";
    jsonCommon["appver"] = m_req->m_commField.appver.c_str();
    jsonCommon["msg"] = "Success";
    jsonCommon["retcode"] = 1;
    jsonCommon["systime"] = (int)time(NULL);
    
    json::JsonValue jv = jsonCommon;
    jsonSend["commresponse"] = jv;
    
    jsonSend["userid"]=9145;//int
    jsonSend["datetime"]=(int)time(NULL);//int
    jsonSend["starttime"]= (int)time(NULL);//int 行动力的起始时间
    //用户详细信息表
    jsonSend["UserName"]="Player-9145";
    jsonSend["OtherId"]="0";
    jsonSend["OtherName"]="0";
    jsonSend["Email"]="";
    jsonSend["Age"]=22;//int
    jsonSend["sex"]=1;//int
    jsonSend["score"]=23;//int
    jsonSend["diamond"]=22;//int
    jsonSend["action"]=22;//int
    jsonSend["highscore"]=233;//int
    jsonSend["lv"]=2;//int
    jsonSend["curex"]=2;//int
    jsonSend["goldnum"]=22;//int
    jsonSend["silvernum"]=22;//int
    jsonSend["bronzenum"]=2;//int
    jsonSend["day"]=1;//int
    jsonSend["week"]=1;//int
    jsonSend["num"]=2;//int
    //升级数据列表
    json::JsonArray uplist;
    m_uplist.clear();
    Uplist t={300,1,0,300,1,1};
    m_uplist.push_back(t);
    for (int i=0; i<m_uplist.size(); i++)
    {
        json::Json jsontmp;
        jsontmp["levupex"] = m_uplist[i].levupex;
        jsontmp["lv"] = m_uplist[i].lv;
        jsontmp["percent"] = m_uplist[i].percent;
        jsontmp["rule"] = m_uplist[i].rule;
        jsontmp["sliver"] = m_uplist[i].sliver;
        jsontmp["type"] = m_uplist[i].type;
        json::JsonValue jsontmp_v = jsontmp;
        uplist.Add(jsontmp_v);
    }
    json::JsonValue uplist_v = uplist;
    jsonSend["uplist"] = uplist_v;
    //好友列表
    json::JsonArray friendlist;
    m_friendlist.clear();
    FriendInfo friendinfo_t={9145,"0","0",300,1,1,0,0,"0","0"};
    m_friendlist.push_back(friendinfo_t);
    for (int i=0; i<m_friendlist.size(); i++)
    {
        json::Json jsontmp;
        jsontmp["friendid"] = m_friendlist[i].friendid;
        jsontmp["friendname"] = m_friendlist[i].friendname.c_str();
        jsontmp["otherid"] = m_friendlist[i].otherid.c_str();
        jsontmp["friendscore"] = m_friendlist[i].friendscore;
        jsontmp["maxscore"] = m_friendlist[i].maxscore;
        jsontmp["goldnum"] = m_friendlist[i].goldnum;
        jsontmp["silvernum"] = m_friendlist[i].silvernum;
        jsontmp["bronzenum"] = m_friendlist[i].bronzenum;
        json::JsonValue jsontmp_v = jsontmp;
        friendlist.Add(jsontmp_v);
    }
    json::JsonValue friendlist_v = friendlist;
    jsonSend["friendlist"] = friendlist_v;

     //通知列表
    //分享显示列表
    //微薄显示列表
    //每日奖励列表
     //星期好友排名
    //购买钻石基本表
     //周任务列表
    json::JsonArray missionlist;
    m_missionlist.clear();
    TaskData missionlist_t={7,200,2,"ddd",1,1,0,0};
    m_missionlist.push_back(missionlist_t);
    for (int i=0; i<m_missionlist.size(); i++)
    {
        json::Json jsontmp;
        jsontmp["week"] = m_missionlist[i].week;
        jsontmp["description_text"] = m_missionlist[i].description_text.c_str();
        jsontmp["type"] = m_missionlist[i].type;
        jsontmp["num"] = m_missionlist[i].num;
        jsontmp["award_num"] = m_missionlist[i].award_num;
        jsontmp["award_type"] = m_missionlist[i].award_type;
        json::JsonValue jsontmp_v = jsontmp;
        missionlist.Add(jsontmp_v);
    }
    json::JsonValue missionlist_v = missionlist;
    jsonSend["missionlist"] = missionlist_v;
    //活动列表
    
    std::string strJson;
    
    jsonSend.Dump(strJson);
    m_head.key = HEAD_KEY;
    m_head.uLen = strlen(strJson.c_str()) ;
    m_head.cZip = 0;
     printf("\nstrJson send:\n%s",strJson.c_str());
    char *out = NULL;
    int len=0;
    char *pBuf = m_pData->GetBuffer(sizeof(m_head) + m_head.uLen);
    memcpy(pBuf, &m_head, sizeof(m_head));
    
    pBuf += sizeof(m_head);
    memcpy(pBuf, strJson.c_str(), m_head.uLen);
    if (m_head.cZip ==1) {
        
        char *in = m_pData->GetData()+sizeof(DAT_HEAD);
        len = ZipUtils::ccDeflateMemory((unsigned char*)in, m_pData->GetLength()-sizeof(DAT_HEAD),(unsigned char**) &out);
        
        if(len<=0)
            return -1 ;
        
        for(int i=0;i<len;i++)
        {
            out[i]+=(BYTE)i;
        }
    }else{
        out = new char[m_head.uLen + 1];
        memset(out, 0, m_head.uLen + 1);
        memcpy(out, strJson.c_str(), m_head.uLen);
        len=m_head.uLen+1;
    }
    
//    printf("\nout send %d:\n%s\n",len,out);
//    {
//        char *pBuffer;
//        char *pbuf2;
//        pbuf2 = (char*)malloc(m_head.uLen );
//        memset(pbuf2, 0, m_head.uLen);
//        memcpy(pbuf2, out, m_head.uLen);
//        
//        for(int i=0;i<m_head.uLen;i++)
//            pbuf2[i]-=(BYTE)i;
//        
//        int length = ZipUtils::ccInflateMemory((unsigned char*)pbuf2, m_head.uLen, (unsigned char**) &pBuffer);
//        
//        free(pbuf2);
//        
//        if(length < 0)
//        {
//            return 0;
//        }
//    
//        printf("\nlength:%d,inflate:%s\n",length,pBuffer);
//        
//    }
    
    
    len+=sizeof(DAT_HEAD);
    m_pSendLen=len;
    m_pSend = (char*)malloc(len);
    memcpy(m_pSend, m_pData->GetData(), sizeof(DAT_HEAD));
    memcpy( m_pSend+sizeof(DAT_HEAD), out, len-sizeof(DAT_HEAD));
    
    DAT_HEAD *pHead = (DAT_HEAD*)m_pSend;
    pHead->uLen = len-sizeof(DAT_HEAD);
   
     return 1;
}

#pragma mark   UpdateUserInfo    06
CDatUpdateUserInfoRequest::CDatUpdateUserInfoRequest(json::Json&jc)
{
    UnMarshal(jc);
}

int CDatUpdateUserInfoRequest::UnMarshal(json::Json&jc)
{
      return m_head.uLen;
}


CDatUpdateUserInfoResponse::CDatUpdateUserInfoResponse(CDatUpdateUserInfoRequest*req)
{
    m_req=req;
}
int CDatUpdateUserInfoResponse::Marshal()
{
    json::Json jsonSend;
    json::Json jsonCommon;
    
    jsonCommon["code"] = "06";
    jsonCommon["appver"] = m_req->m_commField.appver.c_str();
    jsonCommon["msg"] = "Success";
    jsonCommon["retcode"] = 1;
    jsonCommon["systime"] = (int)time(NULL);
    
    json::JsonValue jv = jsonCommon;
    jsonSend["commresponse"] = jv;
    
   
    jsonSend["userid"]=9145;
 
    
    //好友列表
    //通知列表
    std::string strJson;
    jsonSend.Dump(strJson);
    m_head.key = HEAD_KEY;
    m_head.uLen = strlen(strJson.c_str()) ;
    m_head.cZip = 0;
    
    char *out = NULL;
    int len=0;
    char *pBuf = m_pData->GetBuffer(sizeof(m_head) + m_head.uLen);
    memcpy(pBuf, &m_head, sizeof(m_head));
    
    pBuf += sizeof(m_head);
    memcpy(pBuf, strJson.c_str(), m_head.uLen);
    if (m_head.cZip ==1) {
        
        char *in = m_pData->GetData()+sizeof(DAT_HEAD);
        len = ZipUtils::ccDeflateMemory((unsigned char*)in, m_pData->GetLength()-sizeof(DAT_HEAD),(unsigned char**) &out);
        
        if(len<=0)
            return -1 ;
        
        for(int i=0;i<len;i++)
        {
            out[i]+=(BYTE)i;
        }
    }else{
        out = new char[m_head.uLen + 1];
        memset(out, 0, m_head.uLen + 1);
        memcpy(out, strJson.c_str(), m_head.uLen);
        len=m_head.uLen+1;
    }
    
    
    len+=sizeof(DAT_HEAD);
    m_pSendLen=len;
    
    m_pSend = (char*)malloc(len);
    memcpy(m_pSend, m_pData->GetData(), sizeof(DAT_HEAD));
    memcpy( m_pSend+sizeof(DAT_HEAD), out, len-sizeof(DAT_HEAD));
    
    DAT_HEAD *pHead = (DAT_HEAD*)m_pSend;
    pHead->uLen = len-sizeof(DAT_HEAD);
    
    return len;
    

     return 1;
}



#pragma mark GET Action data  11
CDatgetActionRequest::CDatgetActionRequest(json::Json&jc)
{
    UnMarshal(jc);
}
int CDatgetActionRequest::UnMarshal(json::Json&jc)
{
    
    
    return 1;
}


CDatgetActionResponse::CDatgetActionResponse(CDatgetActionRequest*req)
{
    m_req=req;
}
int CDatgetActionResponse::Marshal()
{
    json::Json jsonSend;
    json::Json jsonCommon;
    
    jsonCommon["code"] = "11";
    jsonCommon["appver"] = m_req->m_commField.appver.c_str();
    jsonCommon["msg"] = "Success";
    jsonCommon["retcode"] = 1;
    jsonCommon["systime"] = (int)time(NULL);
    
    json::JsonValue jv = jsonCommon;
    jsonSend["commresponse"] = jv;
    
    
    jsonSend["userid"]=9145;
    jsonSend["starttime"]=(int)time(NULL);
    
    std::string strJson;
    jsonSend.Dump(strJson);
    m_head.key = HEAD_KEY;
    m_head.uLen = strlen(strJson.c_str()) ;
    m_head.cZip = 0;
    
    char *out = NULL;
    int len=0;
    char *pBuf = m_pData->GetBuffer(sizeof(m_head) + m_head.uLen);
    memcpy(pBuf, &m_head, sizeof(m_head));
    
    pBuf += sizeof(m_head);
    memcpy(pBuf, strJson.c_str(), m_head.uLen);
    if (m_head.cZip ==1) {
        
        char *in = m_pData->GetData()+sizeof(DAT_HEAD);
        len = ZipUtils::ccDeflateMemory((unsigned char*)in, m_pData->GetLength()-sizeof(DAT_HEAD),(unsigned char**) &out);
        
        if(len<=0)
            return -1 ;
        
        for(int i=0;i<len;i++)
        {
            out[i]+=(BYTE)i;
        }
    }else{
        out = new char[m_head.uLen + 1];
        memset(out, 0, m_head.uLen + 1);
        memcpy(out, strJson.c_str(), m_head.uLen);
        len=m_head.uLen+1;
    }
 
 
    len+=sizeof(DAT_HEAD);
    m_pSendLen=len;
    
    m_pSend = (char*)malloc(len);
    memcpy(m_pSend, m_pData->GetData(), sizeof(DAT_HEAD));
    memcpy( m_pSend+sizeof(DAT_HEAD), out, len-sizeof(DAT_HEAD));
    
    DAT_HEAD *pHead = (DAT_HEAD*)m_pSend;
    pHead->uLen = len-sizeof(DAT_HEAD);
    
    return len;
    
}
//int CDatgetActionResponse::GetMonInfo()
//{
//    return time;
//}

#pragma mark Access is unalready installed little account of the game   17
CDatInstalledGameRequest::CDatInstalledGameRequest(json::Json&jc)
{
    UnMarshal(jc);
}
int CDatInstalledGameRequest::UnMarshal(json::Json&jc)
{
    return 0;
}
    CDatInstalledGameResponse::CDatInstalledGameResponse(CDatInstalledGameRequest*req)
{
    m_req =req;
}
int CDatInstalledGameResponse::Marshal()
{
    json::Json jsonSend;
    json::Json jsonCommon;
    
    jsonCommon["code"] = "17";
    jsonCommon["appver"] = m_req->m_commField.appver.c_str();
    jsonCommon["msg"] = "Success";
    jsonCommon["retcode"] = 1;
    jsonCommon["systime"] = (int)time(NULL);
    
    json::JsonValue jv = jsonCommon;
    jsonSend["commresponse"] = jv;
    
    
    json::JsonArray list;
    m_list.clear();
    InstalledGame installgame_t={"9145"};
    m_list.push_back(installgame_t);
    for (int i=0; i<m_list.size(); i++)
    {
        json::Json jsontmp;
        jsontmp["id"] = m_list[i].id.c_str();
        json::JsonValue jsontmp_v = jsontmp;
        list.Add(jsontmp_v);
    }
    json::JsonValue list_v = list;
    jsonSend["list"] = list_v;
    
    
    std::string strJson;
    jsonSend.Dump(strJson);
    m_head.key = HEAD_KEY;
    m_head.uLen = strlen(strJson.c_str()) ;
    m_head.cZip = 0;
    
    char *out = NULL;
    int len=0;
    char *pBuf = m_pData->GetBuffer(sizeof(m_head) + m_head.uLen);
    memcpy(pBuf, &m_head, sizeof(m_head));
    
    pBuf += sizeof(m_head);
    memcpy(pBuf, strJson.c_str(), m_head.uLen);
    if (m_head.cZip ==1) {
        
        char *in = m_pData->GetData()+sizeof(DAT_HEAD);
        len = ZipUtils::ccDeflateMemory((unsigned char*)in, m_pData->GetLength()-sizeof(DAT_HEAD),(unsigned char**) &out);
        
        if(len<=0)
            return -1 ;
        
        for(int i=0;i<len;i++)
        {
            out[i]+=(BYTE)i;
        }
    }else{
        out = new char[m_head.uLen + 1];
        memset(out, 0, m_head.uLen + 1);
        memcpy(out, strJson.c_str(), m_head.uLen);
        len=m_head.uLen+1;
    }
    
    
    len+=sizeof(DAT_HEAD);
    m_pSendLen=len;
    
    m_pSend = (char*)malloc(len);
    memcpy(m_pSend, m_pData->GetData(), sizeof(DAT_HEAD));
    memcpy( m_pSend+sizeof(DAT_HEAD), out, len-sizeof(DAT_HEAD));
    
    DAT_HEAD *pHead = (DAT_HEAD*)m_pSend;
    pHead->uLen = len-sizeof(DAT_HEAD);
    
    return len;

}
//std::vector<std::string> *CDatInstalledGameResponse::GetMonInfo()
//{
//    return &m_notifyinfo;
//}
#pragma mark 赠送体力   21

CDatGiveStrengthRequest::CDatGiveStrengthRequest(json::Json&jc){
    UnMarshal(jc);
}

int CDatGiveStrengthRequest::UnMarshal(json::Json&jc)
{
    return 1;
  }

CDatGiveStrengthResponse::CDatGiveStrengthResponse(CDatGiveStrengthRequest*req)
{
    m_req=req;

}

int CDatGiveStrengthResponse::Marshal()
{
    json::Json jsonSend;
    json::Json jsonCommon;
    
    jsonCommon["code"] = "21";
    jsonCommon["appver"] = m_req->m_commField.appver.c_str();
    jsonCommon["msg"] = "Success";
    jsonCommon["retcode"] = 1;
    jsonCommon["systime"] = (int)time(NULL);
    
    json::JsonValue jv = jsonCommon;
    jsonSend["commresponse"] = jv;
    
    
    jsonSend["userid"]=9145;
    jsonSend["friendId"]=3;
    jsonSend["type"]=1;
    
    std::string strJson;
    jsonSend.Dump(strJson);
    m_head.key = HEAD_KEY;
    m_head.uLen = strlen(strJson.c_str()) ;
    m_head.cZip = 0;
    
    char *out = NULL;
    int len=0;
    char *pBuf = m_pData->GetBuffer(sizeof(m_head) + m_head.uLen);
    memcpy(pBuf, &m_head, sizeof(m_head));
    
    pBuf += sizeof(m_head);
    memcpy(pBuf, strJson.c_str(), m_head.uLen);
    if (m_head.cZip ==1) {
        
        char *in = m_pData->GetData()+sizeof(DAT_HEAD);
        len = ZipUtils::ccDeflateMemory((unsigned char*)in, m_pData->GetLength()-sizeof(DAT_HEAD),(unsigned char**) &out);
        
        if(len<=0)
            return -1 ;
        
        for(int i=0;i<len;i++)
        {
            out[i]+=(BYTE)i;
        }
    }else{
        out = new char[m_head.uLen + 1];
        memset(out, 0, m_head.uLen + 1);
        memcpy(out, strJson.c_str(), m_head.uLen);
        len=m_head.uLen+1;
    }
    
    
    len+=sizeof(DAT_HEAD);
    m_pSendLen=len;
    
    m_pSend = (char*)malloc(len);
    memcpy(m_pSend, m_pData->GetData(), sizeof(DAT_HEAD));
    memcpy( m_pSend+sizeof(DAT_HEAD), out, len-sizeof(DAT_HEAD));
    
    DAT_HEAD *pHead = (DAT_HEAD*)m_pSend;
    pHead->uLen = len-sizeof(DAT_HEAD);
    
    return len;

}
std::string toUTF8(const char* text)
{
    std::string in = (char*) text;
    std::string out = "";
    
    int len = strlen(text);
    
    for(int i=0;i< len ;i++)
    {
        if(text[i] == '\\')
        {
            if(len == (i+1))
            {
                out + text[i];
                break;
            }
            else if(text[i+1] == 'u')
            {
                if(len <= (i+5))
                {
                    out + text[i];
                    break;
                }
                
                char hex0 = Hex(text[i+2]);
                char hex1 = Hex(text[i+3]);
                char hex2 = Hex(text[i+4]);
                char hex3 = Hex(text[i+5]);
                
                out += ((14 << 4) + hex0);
                out += ((8 << 4) + (hex1 << 2) + (hex2 >> 2));
                out += ((8 << 4) + (((int)hex2 & 0x3) << 4) + hex3);
                
                i+=5;
            }
            else
                out += text[i];
        }
        else
            out += text[i];
    }
    return out;
}
char Hex(const char pos)
{
    unsigned char h = pos - 48;
    if (h > 48){  //A-F don't immediately follow 0-9, so have to pull them down a little
        h -= 39;
    } else if (h > 9){  //neither do a-f
        h -= 7;
    }
    return (char)(h);
}
