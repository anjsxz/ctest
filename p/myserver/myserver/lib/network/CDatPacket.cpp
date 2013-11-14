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


CDatRequest::CDatRequest(void)
{
    m_commField.retcode = 4;
}

int CDatRequest::ccInflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out)
{
    unsigned int outLenghtHint = 256 * 1024;
    /* ret value */
    int err = Z_OK;
    
    int bufferSize = outLenghtHint;
    *out = new unsigned char[bufferSize];
    
    z_stream d_stream; /* decompression stream */    
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    
    d_stream.next_in  = in;
    d_stream.avail_in = inLength;
    d_stream.next_out = *out;
    d_stream.avail_out = bufferSize;
    
    /* window size to hold 256k */
    if( (err = inflateInit2(&d_stream, 15 + 32)) != Z_OK )
        return err;
    
    for (;;) 
    {
        err = inflate(&d_stream, Z_NO_FLUSH);
        
        if (err == Z_STREAM_END)
        {
            break;
        }
        
        switch (err) 
        {
            case Z_NEED_DICT:
                err = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR: 
                inflateEnd(&d_stream);
                return err;
        }
        
        // not enough memory ?
        if (err != Z_STREAM_END) 
        {
            return -1;
        }
    }
    
    
    err = inflateEnd(&d_stream);
    return err;
}

int	CDatRequest::UnMarshal(char *pData,int len, bool zip)
{
    char *pBuf = pData;
    /////
    memcpy(&m_head, pData, sizeof(m_head));
    pBuf += sizeof(m_head);
    //////
    
    
    
    char *pBuffer;
    
    
    if(m_head.cZip)
    {
        char *pbuf2;
        pbuf2 = (char*)malloc(m_head.uLen );
        memset(pbuf2, 0, m_head.uLen);
        memcpy(pbuf2, pBuf, m_head.uLen);
        
        for(int i=0;i<m_head.uLen;i++)
            pbuf2[i]-=(BYTE)i;
        
        int length ;//= ZipUtils::ccInflateMemory((unsigned char*)pbuf2, m_head.uLen, (unsigned char**) &pBuffer);
        
        free(pbuf2);
        
        if(length < 0)
        {
            return 0;
        }
    }
    else
    {
        pBuffer = new char[m_head.uLen + 1];
        memset(pBuffer, 0, m_head.uLen + 1);
        memcpy(pBuffer, pBuf, m_head.uLen);
       
    }
    
    
    printf("%s",pBuffer);
    
    //////////////
    //    if(zip)
    //    {
    //        ;////////unzip
    //    }
    //    else
    //    {
    //        pBuffer = (char*)malloc(len + 1);
    //        memset(pBuffer, 0, len + 1);
    //        memcpy(pBuffer, pBuf, len);
    //        printf("%s",pBuffer);
    //    }
    ////////////////
    json::Json *pJson = NULL;
    json::Json jComm;
    json::JsonValue jv;
    if(json::Json::Parse(pJson, pBuffer) != 0)
    {
        if(pJson)
            delete pJson;
        if(pBuffer)
            delete [] pBuffer;
        return 0;
    }
    
    
    
    try {
        jv = (*pJson)["commresponse"];
        jComm = jv;
        m_commField.msg =(char*) jComm["msg"];
        m_commField.retcode = (int)jComm["retcode"];
        m_commField.systime = (int )jComm["systime"];
        m_commField.code = (char*)jComm["code"];
        g_sysTimer = m_commField.systime;
        timeStart=0;
        if(m_commField.retcode == 1)
        {
            UnMarshal(*pJson);
        }
        
        
        
    } catch (json::JsonException *e) {

        printf("%s",e->what());
        if(pJson)
            delete pJson;
        if(pBuffer)
            delete [] pBuffer;
        return 0;
    }
    catch(...)
    {

    }
    delete pJson;
	delete [] pBuffer;
	return 1;
    
}

int CDatRequest::getRetcode()
{
    return m_commField.retcode;
}

CDatRequest::~CDatRequest(void)
{
    
}

#pragma mark Regist
CDatRegistResponse::CDatRegistResponse()
{
}

int CDatRegistResponse::Marshal()
{
    json::Json jsonSend;
    json::Json jsonCommon;
    
   jsonSend["code"] = "01";
    
 
    
    json::JsonValue jv = jsonCommon;
    jsonSend["commrequest"] = jv;
    std::string strJson;
  
    jsonSend.Dump(strJson);
    m_head.key = HEAD_KEY;
    m_head.uLen = strlen(strJson.c_str()) ;
    m_head.cZip = 1;
    
    char *pBuf = m_pData->GetBuffer(sizeof(m_head) + m_head.uLen);
    memcpy(pBuf, &m_head, sizeof(m_head));
    
    pBuf += sizeof(m_head);
    memcpy(pBuf, strJson.c_str(), m_head.uLen);
    printf("%s",pBuf);

    return m_head.uLen;
    
}



int CDatRegistRequest::UnMarshal(json::Json &jc)
{
    
    if (jc.GetItemSize()!=0)
    {
        m_reqRegist.username= ((char *)jc["username"]);
//        m_reqRegist.otherid= ((char *)jc["otherid"]);
        m_reqRegist.othername= ((char *)jc["othername"]);
        m_reqRegist.sex= (int)jc["sex"];
        m_reqRegist.lang= (char *)jc["lang"];
        m_UserID=(int)jc["userid"];
    }
    else 
    {
        
    }
    return 1;
}
//ReqLogin *CDatRegistResponse::GetUserInfo()
//{
//    return &m_reqRegist;
//}

#pragma mark Login


CDatLoginResponse::CDatLoginRequest(ReqLogin reqLogin,std::string source,std::vector<std::string> list,int userid)
{
    m_reqLogin=reqLogin;
    m_source=source;
    m_list=list;
    m_userId=userid;
}
int CDatLoginResponse::Marshal()
{
    json::Json jsonSend;
    json::Json jsonCommon;
    json::JsonArray jsonList;
    
    
    
    jsonSend["code"] = "02";
    
    jsonCommon["snnum"] = m_commField.snnum.c_str();
    jsonCommon["sysver"] = m_commField.sysver.c_str();
    jsonCommon["appver"] = m_commField.appver.c_str();
    jsonCommon["version"] = m_commField.verions.c_str();
    
    json::JsonValue jv = jsonCommon;
    jsonSend["commrequest"] = jv;
    
    jsonSend["username"] = m_reqLogin.username.c_str();
    jsonSend["otherid"] = m_reqLogin.otherid.c_str();
    jsonSend["othername"] = m_reqLogin.othername.c_str();
    jsonSend["sex"] = m_reqLogin.sex;
    jsonSend["lang"] = m_reqLogin.lang.c_str();
    jsonSend["email"] = m_reqLogin.email.c_str();
    jsonSend["source"]=m_source.c_str();
    jsonSend["userid"] = m_userId;
    for (int i=0; i<m_list.size(); i++)
    {
        json::Json jsonmoninf;
        jsonmoninf["id"] = m_list[i].c_str();
        json::JsonValue jvmoninfo = jsonmoninf;
        jsonList.Add(jvmoninfo);
    }
    
    json::JsonValue jvary = jsonList;
    jsonSend["list"] = jvary;
    std::string strJson;
    jsonSend.Dump(strJson);
    
    printf("%s",strJson.c_str());
    
    
    m_head.key = HEAD_KEY;
    m_head.uLen = strlen(strJson.c_str()) ;
    m_head.cZip = 1;
    
    
    char *pBuf = m_pData->GetBuffer(sizeof(m_head) + m_head.uLen);
    memcpy(pBuf, &m_head, sizeof(m_head));
    
    pBuf += sizeof(m_head);
    memcpy(pBuf, strJson.c_str(), m_head.uLen);
    return m_head.uLen;
}
CDatLoginResponse::CDatLoginResponse()
{
    
}
int CDatLoginResponse::UnMarshal(json::Json &jc)
{
    
    try{
        
        {  //获取请求版本号
            json::Json jComm;
            json::JsonValue jv;
            jv = jc["commresponse"];
            jComm = jv;
            m_appver =(char*) jComm["appver"];
        }
        m_userid= (int)jc["userid"];
        m_datetime=(int)jc["datetime"];
        m_starttime=(int)jc["starttime"];//行动力的起始时间
        //用户详细信息表
        m_userinfo.UserId=m_userid;
        m_userinfo.User= (char*)jc["UserName"];
        m_userinfo.OtherId  = (char * )jc["OtherId"];
        m_userinfo.OtherName=((char *)jc["OtherName"]);
        m_userinfo.Email  = (char *)jc["Email"];
        m_userinfo.Age  = (int )jc["Age"];
        m_userinfo.Sex   = (int )jc["sex"];
        m_userinfo.curScore  = (int )jc["score"];
        m_userinfo.curSliver = (int )jc["diamond"];
        m_userinfo.curAction  = (int )jc["action"];
        m_userinfo.maxScore  = (int )jc["highscore"];
        m_userinfo.lv = (int )jc["lv"];
//        m_userinfo.ani = (int )jc["sx"];
        m_userinfo.curex=(int )jc["curex"];
        m_userinfo.goldnum = (int )jc["goldnum"];
        m_userinfo.silvernum = (int )jc["silvernum"];
        m_userinfo.bronzenum = (int )jc["bronzenum"];
        m_day= (int )jc["day"];
        m_week= (int )jc["week"];
        m_number= (int )jc["num"];
//      m_userinfo.UserId  = (int )jc["UserId"];
        //升级数值列表
        json::JsonValue  isexist;
        isexist=jc["uplist"];
        if (!isexist.IsEmpty())
        {
            json::JsonArray ary1;
            ary1 = jc["uplist"];
            for(int i = 0; i < ary1.GetSize(); i ++)
            {
                GetlvEx achi;
                json::Json jnv = ary1[i];
                achi.lv = (int)jnv["lv"];
                achi.levupex  = (int )jnv["levupex"];
                achi.type  = (int)jnv["type"];
                achi.bonuscore  = (int )jnv["percent"];
                achi.reward  = (int )jnv["sliver"];
                achi.getEx  = (int )jnv["rule"];
                m_exinfo.push_back(achi);
            }
        }
        //好友列表
        json::JsonArray ary2;
        ary2 = jc["friendlist"];
        for(int i = 0; i < ary2.GetSize(); i ++)
        {
            FriendInfo achi;
            json::Json jnv = ary2[i];
            achi.friendid = (int)jnv["friendid"];
            achi.friendname=((char*)jnv["friendname"]);
            achi.otherid = (char*)jnv["otherid"];
            achi.friendscore = (int)jnv["friendscroe"];
            achi.maxscore = (int)jnv["maxscroe"];
            achi.goldnum = (int)jnv["goldnum"];
            achi.silvernum = (int)jnv["silvernum"];
            achi.bronzenum = (int)jnv["bronzenum"];
            m_friendslist.push_back(achi);
        }
        //通知列表
        json::JsonArray ary3;
        ary3 = jc["notifylist"];
        for(int i = 0; i < ary3.GetSize(); i ++)
        {
            NotifyInfo achi;
            json::Json jnv = ary3[i];
            achi.notifyid = (int)jnv["notifyid"];
            achi.notifytype = (int )jnv["type"];
            achi.friendId = (int )jnv["friendid"];
            achi.otherid = (char *)jnv["otherid"];
            achi.othername=((char *)jnv["othername"]);
            achi.datetime = (int)jnv["time"];
            achi.content = (char *)jnv["content"];
            m_notifyinfo.push_back(achi);
        }
        //分享显示列表
        isexist=jc["showlist"];
        if (!isexist.IsEmpty())
        {
            json::JsonArray ary4;
            ary4 = jc["showlist"];
            for(int i = 0; i < ary4.GetSize(); i ++)
            {
                ShareText achi;
                json::Json jnv = ary4[i];
                achi.type = (int)jnv["type"];
                achi.game = ((char *)jnv["game"]);
                achi.webo = ((char *)jnv["webo"]);
                achi.url = ((char *)jnv["url"]);
                achi.mark = ((char *)jnv["mark"]);
                m_listShareText.push_back(achi);
            }
        }
         //微薄显示列表
        json::JsonValue  isexist2;
        isexist2=jc["list"];
        if (!isexist2.IsEmpty())
        {
            json::JsonArray ary5;
            ary5 = jc["list"];
            for(int i = 0; i < ary5.GetSize(); i ++)
            {
                std::string  achi;
                json::Json jnv = ary5[i];
                achi = (char *)jnv["id"];
                m_friendIdlist.push_back(achi);
            }
        }
        //每日奖励列表
        json::JsonValue  isexist3;
        isexist3=jc["awardlist"];
        if (!isexist3.IsEmpty())
        {
            json::JsonArray ary6;
            ary6 = jc["awardlist"];
            for(int i = 0; i < ary6.GetSize(); i ++)
            {
                DailyLogin achi;
                json::Json jnv = ary6[i];
                achi.num = (int)jnv["num"];
                achi.sliver = (int)jnv["sliver"];
                m_listDailyLogin.push_back(achi);
            }
        }
        //星期好友排名
        isexist=jc["weeklist"];
        if (!isexist.IsEmpty())
        {
            json::JsonArray ary7;
            ary7 = jc["weeklist"];
            for(int i = 0; i < ary7.GetSize(); i ++)
            {
                WeekRankData achi;
                json::Json jnv = ary7[i];
                achi.friendid = (int)jnv["friendid"];
                achi.friendname=((char*)jnv["friendname"]);
                achi.otherid = (char*)jnv["otherid"];
                achi.friendscore = (int)jnv["friendscroe"];
                m_weekRanklist.push_back(achi);
            }
        }
        //购买钻石基本表
        
        isexist=jc["itemlist"];
        if (!isexist.IsEmpty())
        {
            json::JsonArray ary8;
            ary8 = jc["itemlist"];
            for(int i = 0; i < ary8.GetSize(); i ++)
            {
                BuyItem achi;
                json::Json jnv = ary8[i];
                achi.type = (int)jnv["type"];
                achi.itemname = ((char *)jnv["itemname"]);
                achi.itemdes = ((char *)jnv["itemdes"]);
                achi.itemid = ((char *)jnv["itemid"]);
                achi.itemprice =(char *)jnv["itemprice"];;
                achi.itemnum = (int)jnv["itemnum"];;
                achi.itemallnum = (int)jnv["itemallnum"];
                m_buyItemlist.push_back(achi);
            }
        }
        

        //周任务列表
        json::JsonArray ary9;
        ary9 = jc["missionlist"];
        for(int i = 0; i < ary9.GetSize(); i ++)
        {
            TaskData achi;
            json::Json jnv = ary9[i];
            achi.week = (int)jnv["week"];
            achi.type=(int)jnv["type"];
            achi.num=(int)jnv["num"];
            achi.description_text = (char *)jnv["description_text"];
            achi.award_num=(int)jnv["award_num"];
            achi.award_type=(int)jnv["award_type"];
            m_Tasklist.push_back(achi);
        }
        //活动列表
        isexist=jc["playlist"];
        if (!isexist.IsEmpty())
        {
            json::JsonArray ary10;
            ary10 = jc["playlist"];
            for(int i = 0; i < ary10.GetSize(); i ++)
            {
                ActivityData achi;
                json::Json jnv = ary10[i];
                achi.playid = (int)jnv["playid"];
                achi.url = (char *)jnv["url"];
                achi.diamond=(int)jnv["diamond"];
                achi.isjoin=(int)jnv["isjoin"];
                m_Activitylist.push_back(achi);
            }
        }
    }
    catch(json::JsonException *e)
    {
        printf("%s",e->what());
        return 0;
    }
    catch(...)
    {
        
    }
    return 1;
}
#pragma mark   UpdateUserInfo    06
CDatUpdateUserInfoRequest::CDatUpdateUserInfoRequest(getUserInfo info,std::vector<int> list,int time,int isstart,int type)
{
    m_Info=info;
    m_list= list;
    m_type=type;
    m_time=time;
    m_isstart=isstart;
}

int CDatUpdateUserInfoRequest::Marshal()
{
    json::Json jsonSend;
    json::Json jsonCommon;
    json::JsonArray jsonList;
    jsonSend["code"] = "06";
    jsonCommon["snnum"] = m_commField.snnum.c_str();
    jsonCommon["sysver"] = m_commField.sysver.c_str();
    jsonCommon["appver"] = m_commField.appver.c_str();
    jsonCommon["version"] = m_commField.verions.c_str();
    json::JsonValue jv = jsonCommon;
    jsonSend["commrequest"] = jv;
    jsonSend["userid"] = m_Info.UserId;
    jsonSend["curScore"] = m_Info.curScore;
    jsonSend["curSliver"] = m_Info.curSliver;
    jsonSend["curAction"] = m_Info.curAction;
    jsonSend["curex"] = m_Info.curex;
    jsonSend["maxScore"] = m_Info.maxScore;
    jsonSend["lv"] = m_Info.lv;
    jsonSend["islist"]=m_type;
    jsonSend["isstart"]=m_isstart;
    if(m_time!=0)
    {
         jsonSend["starttime"]=m_time;
    }
    for (int i=0; i<m_list.size(); i++)
    {
        json::Json jsonmoninf;
        jsonmoninf["id"] = m_list[i];
        json::JsonValue jvmoninfo = jsonmoninf;
        jsonList.Add(jvmoninfo);
    }
    json::JsonValue jvary = jsonList;
    jsonSend["list"] = jvary;
    std::string strJson;
    jsonSend.Dump(strJson);
    m_head.key = HEAD_KEY;
    m_head.uLen = strlen(strJson.c_str())  ;
    m_head.cZip = 1;
    printf("%s",strJson.c_str());
    char *pBuf = m_pData->GetBuffer(sizeof(m_head) + m_head.uLen);
    memcpy(pBuf, &m_head, sizeof(m_head));
    
    pBuf += sizeof(m_head);
    memcpy(pBuf, strJson.c_str(), m_head.uLen);
    
    return m_head.uLen; 
}


CDatUpdateUserInfoResponse::CDatUpdateUserInfoResponse()
{
    
}
int CDatUpdateUserInfoResponse::UnMarshal(json::Json &jc)
{
    try{
        
        m_friendslist.clear();//好友列表
        m_notifyinfo.clear();//通知列表
        m_Info = (int)jc["userid"];
        json::JsonValue isexist=jc["friendlist"];
        if(!isexist.IsEmpty())
        {
            //好友列表
            json::JsonArray ary2;
            ary2 = jc["friendlist"];
            for(int i = 0; i < ary2.GetSize(); i ++)
            {
                FriendInfo achi;
                json::Json jnv = ary2[i];
                achi.friendid = (int)jnv["friendid"];
                achi.friendname=((char*)jnv["friendname"]);
                achi.otherid = (char*)jnv["otherid"];
                achi.friendscore = (int)jnv["friendscroe"];
                achi.maxscore = (int)jnv["maxscroe"];
                achi.goldnum = (int)jnv["goldnum"];
                achi.silvernum = (int)jnv["silvernum"];
                achi.bronzenum = (int)jnv["bronzenum"];
                m_friendslist.push_back(achi);
            }
        }
        isexist=jc["notifylist"];
        if(!isexist.IsEmpty())
        {
            //通知列表
            json::JsonArray ary3;
            ary3 = jc["notifylist"];
            for(int i = 0; i < ary3.GetSize(); i ++)
            {
                NotifyInfo achi;
                json::Json jnv = ary3[i];
                achi.notifyid = (int)jnv["notifyid"];
                achi.notifytype = (int )jnv["type"];
                achi.friendId = (int )jnv["friendid"];
                achi.otherid = (char *)jnv["otherid"];
                achi.othername=((char *)jnv["othername"]);
                achi.datetime = (int)jnv["time"];
                achi.content = (char *)jnv["content"];
                m_notifyinfo.push_back(achi);
            }

        }
        
    }
    catch(json::JsonException *e)
    {
        printf("%s",e->what());
        return 0;
    }
    catch(...)
    {
        
    }
    return 1;
}

int CDatUpdateUserInfoResponse::GetUpdateUserInfo()
{
    return m_Info;
}
#pragma mark GET Action data  11
CDatgetActionRequest::CDatgetActionRequest(int userid)
{
    m_userid = userid;
}
int CDatgetActionRequest::Marshal()
{
    json::Json jsonSend;
    json::Json jsonCommon;
    json::JsonArray jsonList;
    
    jsonSend["code"] = "11";
    
    jsonCommon["snnum"] = m_commField.snnum.c_str();
    jsonCommon["sysver"] = m_commField.sysver.c_str();
    jsonCommon["appver"] = m_commField.appver.c_str();
    jsonCommon["version"] = m_commField.verions.c_str();
    
    json::JsonValue jv = jsonCommon;
    jsonSend["commrequest"] = jv;
    
    jsonSend["userid"] = m_userid;
    
    std::string strJson;
    
    jsonSend.Dump(strJson);
    
    m_head.key = HEAD_KEY;
    m_head.uLen = strlen(strJson.c_str()) ;
    m_head.cZip = 1;
    
    
    char *pBuf = m_pData->GetBuffer(sizeof(m_head) + m_head.uLen);
    memcpy(pBuf, &m_head, sizeof(m_head));
    
    pBuf += sizeof(m_head);
    memcpy(pBuf, strJson.c_str(), m_head.uLen);
//    printf("%s",pBuf);
    return m_head.uLen;
}


CDatgetActionResponse::CDatgetActionResponse()
{
    
}
int CDatgetActionResponse::UnMarshal(json::Json &jc)
{
    try{
        
        time = (int)jc["starttime"];
        
    }
    catch(json::JsonException *e)
    {
        printf("%s",e->what());
        return 0;
    }
    catch(...)
    {
        
    }
    return 1;

    
}
int CDatgetActionResponse::GetMonInfo()
{
    return time;
}
#pragma mark Access is unalready installed little account of the game   17
CDatInstalledGameRequest::CDatInstalledGameRequest(int userid,std::vector<std::string> list)
{
    m_list.clear();
    m_userid=userid;
    m_list=list;
}
int CDatInstalledGameRequest::Marshal()
{
    json::Json jsonSend;
    json::Json jsonCommon;
    json::JsonArray jsonList;

    jsonSend["code"] = "17";
    
    jsonCommon["snnum"] = m_commField.snnum.c_str();
    jsonCommon["sysver"] = m_commField.sysver.c_str();
    jsonCommon["appver"] = m_commField.appver.c_str();
    jsonCommon["version"] = m_commField.verions.c_str();
    
    json::JsonValue jv = jsonCommon;
    jsonSend["commrequest"] = jv;
    
    jsonSend["userid"] = m_userid;
    for(int i = 0; i <m_list.size(); i ++)
    {
        json::Json jsonmoninf;
        jsonmoninf["id"] = m_list[i].c_str();
        json::JsonValue jvmoninfo = jsonmoninf;
        jsonList.Add(jvmoninfo);
    }
    
    json::JsonValue jvary = jsonList;
    jsonSend["list"] = jvary;

    std::string strJson;
    jsonSend.Dump(strJson);
    m_head.key = HEAD_KEY;
    m_head.uLen = strlen(strJson.c_str()) ;
    m_head.cZip = 1;
    char *pBuf = m_pData->GetBuffer(sizeof(m_head) + m_head.uLen);
    memcpy(pBuf, &m_head, sizeof(m_head));
    pBuf += sizeof(m_head);
    memcpy(pBuf, strJson.c_str(), m_head.uLen);
    return m_head.uLen;
}
CDatInstalledGameResponse::CDatInstalledGameResponse()
{
    
}
int CDatInstalledGameResponse::UnMarshal(json::Json &jc)
{
    try {
        json::JsonArray ary;
        ary = jc["list"];
        for(int i = 0; i < ary.GetSize(); i ++)
        {
            std::string m_equip;
            json::Json jnv = ary[i];
            m_equip = (char *)jnv["id"];
            m_notifyinfo.push_back(m_equip);
        }
    } catch (json::JsonException *e) {
        printf("%s",e->what());
        return 0;
        
    }
    catch(...)
    {
        
    }
    return 1;
}
std::vector<std::string> *CDatInstalledGameResponse::GetMonInfo()
{
    return &m_notifyinfo;
}
#pragma mark 赠送体力   21

CDaGiveStrengthRequest::CDaGiveStrengthRequest(int useId,int friendId,int type)
{
    m_useId=useId;
    m_friendId=friendId;
    m_type=type;
}

int CDaGiveStrengthRequest::Marshal()
{
    json::Json jsonSend;
    json::Json jsonCommon;
    
    jsonSend["code"] = "21";
    
    jsonCommon["snnum"] = m_commField.snnum.c_str();
    jsonCommon["sysver"] = m_commField.sysver.c_str();
    jsonCommon["appver"] = m_commField.appver.c_str();
    jsonCommon["version"] = m_commField.verions.c_str();
    
    json::JsonValue jv = jsonCommon;
    jsonSend["commrequest"] = jv;
    jsonSend["userid"] =m_useId;
    jsonSend["friendId"] =m_friendId;
    jsonSend["type"] =m_type;
    std::string strJson;
    
    jsonSend.Dump(strJson);
    
    m_head.key = HEAD_KEY;
    m_head.uLen = strlen(strJson.c_str()) ;
    m_head.cZip = 1;
    
    
    char *pBuf = m_pData->GetBuffer(sizeof(m_head) + m_head.uLen);
    memcpy(pBuf, &m_head, sizeof(m_head));
    
    pBuf += sizeof(m_head);
    memcpy(pBuf, strJson.c_str(), m_head.uLen);
    return m_head.uLen;
}

CDatGiveStrengthResponse::CDatGiveStrengthResponse()
{


}

int CDatGiveStrengthResponse::UnMarshal(json::Json &jc)
{
    return 1;
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
