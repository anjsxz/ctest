#pragma once


#include "json.h"
#include "CMemBuffer.h"




#define HEAD_KEY 1215

#pragma pack(1)
typedef struct tagDAT_HEAD
{
    unsigned int key;
    unsigned int uLen;
    char cZip;
}DAT_HEAD;
#pragma pack()

typedef struct tagCommRequest
{
    std::string snnum;
    std::string sysver;
    std::string appver;
    std::string verions;
}CommRequest;


typedef struct tagCommResponse
{
    int retcode;
    int systime;//系统时间
    std::string msg;//返回的信息
    std::string code;//请求的命令
}CommResponse;


typedef struct tagReqLogin   /////2
{
    std::string username;//用户名
    std::string otherid ;//facebook id
    std::string othername;//facebook name
    int sex ;//性别
    std::string lang;//语言
    std::string email;//邮箱
    std::string pic;//图片链接地址
    int time;
}ReqLogin;

typedef struct tagFriendInfo   ////3
{
    int friendid;//好友id
    std::string otherid;//facebook好友 的id
    std::string friendname;//facebook好友 的名字
    int friendscore;//好友 的分数
    int maxscore;//好友历史的分数
    int goldnum;//金牌数量
    int silvernum;//银牌数量
    int bronzenum;//铜牌数量
    std::string time;//玩家向好友赠送行动力的时间
    std::string time2;//玩家向好友索取行动力的时间

}FriendInfo;


typedef struct tagNotifyInfo /////4
{
    int notifyid;//消息的id
    int notifytype;//0 表示好友送你一个体力  1 表示好友向你索取一个体力
    int friendId;//好友id
    std::string otherid;//facebook好友的id
    std::string othername;//facebook好友的名字
    int datetime;//消息发送的时间
    std::string content;//消息发送的内容
}NotifyInfo;
typedef struct taggetUserInfo  ////10
{
    std::string User;//
    int UserId ;//玩家  id
    std::string OtherId;////玩家facebook的id
    std::string OtherName;//玩家facebook的名字
    std::string Email;//玩家的邮箱地址
    int Age;//玩家的年龄
    int Sex;//玩家的性别
    int curScore ;//玩家当前最高分
    int curSliver;//玩家的钻石个数
    int curAction;//玩家行动力的个数
    int curex;//玩家当前的经验
    int maxScore ;//玩家历史最高分
    int lv;//玩家等级lv
    int ani;//(当前工程没有用到)
    int goldnum;//金牌数量
    int silvernum;//银牌数量
    int bronzenum;//铜牌数量
}getUserInfo;

typedef struct tagGetItemData     ////13
{
    int itemid;//道具的使用id
    int type;//道具的类型
    std::string des;//道具的描述
    std::string name;//道具的名称
    int itemlv;//道具的解锁等级
    int price;//道具的价格
    int num;//玩家拥有该道具的数量
}GetItemData;

typedef struct tagGetlvEx
{
    int lv;//等级
    int levupex;//当前等级升级所需的经验
    int type;//类型
    int bonuscore;//当前等级的额外奖励分数的百分比
    int reward;//升级后所得到的奖励
    int getEx;//每一盘所获取的经验(现在没有用到)
}GetlvEx;

typedef struct tagDailyLogin/////24
{
    int num;//第几天
    int sliver;//奖励几颗钻石
}DailyLogin;

typedef struct tagShareText/////25
{
    int type;//分享的内容
    std::string game;//分享的文字显示
    std::string webo;//分享出去的文字内容
    std::string url;//分享出去的链接
    std::string mark;//文字注释
}ShareText;


typedef struct tagBuyItem//
{
    int type;//充值的类型
    std::string itemname;//充值物品的名字
    std::string itemdes;//充值物品的描述
    std::string itemid;//充值物品的的id 
    std::string itemprice;//充值物品的的价格
    int itemnum;//充值过后可以获得的物品的数量
    int itemallnum;//可以充值的次数
}BuyItem;

typedef struct tagPicUrl//
{
    std::string picname;//facebook  头像的名字
    std::string data;//facebook  头像的数据
    std::string url;//facebook  头像的url的链接
    
}PicUrl;

typedef struct tagTaskData//
{
    int week;//星期参数
    int type;//任务类型
    int num;//任务数量
    std::string description_text;//任务描述
    int award_type;//奖励类型
    int award_num;//奖励数量
    int completeness;//任务完成度
    int isgetaward;//是否领取奖励
}TaskData;

typedef struct tagActivityData//
{
    int playid;//活动编号
    std::string url;//图片地址
    int diamond;//花费钻石
//    int picData;
    int isjoin;//需要参加
}ActivityData;

typedef struct tagWeekRankData//
{
    int friendid;//好友id
    std::string friendname;// 好友名字
    std::string otherid;//微薄id
    int friendscore;//好友得分
}WeekRankData;

typedef struct tagUplist{
    int levupex;
    int lv;
    int  percent;
    int rule;
    int sliver;
    int type;
}Uplist;//升级列表
typedef struct tagInstalledGame{
    std::string id;
}InstalledGame;
//////////////////////////////////
class CDatResponse
{
public:
	CDatResponse();
	~CDatResponse();
    virtual int Marshal() = 0;
    int ccDeflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out);
    
    DAT_HEAD m_head;
    CommResponse m_commField;
//    std::string m_strcode;
    CMemBuffer *m_pData;
    char* m_pSend;
    int m_pSendLen;
};

class CDatRequest
{

public:
	CDatRequest();
	~CDatRequest();
    virtual	int				 UnMarshal(json::Json &jc) = 0;
   
    DAT_HEAD				m_head;
    CommRequest m_commField;
    
};

//static    std::string a2u(const char *inbuf);
#pragma mark Regist 1
/////////////////////////////////////////regist  1
class CDatRegistRequest:public CDatRequest
{
public:
    CDatRegistRequest(json::Json&jc);
    int UnMarshal(json::Json &jc);
    ReqLogin *GetUserInfo();
    int m_UserID;
    
private:
    ReqLogin m_reqRegist;
};

class CDatRegistResponse:public CDatResponse
{
    CDatRegistRequest*m_req;
public:
    CDatRegistResponse(CDatRegistRequest*req);
    int Marshal();
    
};



#pragma mark Login  2
/////////////////////////////////////////////////login   2


class CDatLoginRequest:public CDatRequest
{
public:
    CDatLoginRequest(json::Json&jc);
    int UnMarshal(json::Json &jc);
    int m_userid;//用户id
    int m_datetime;//比赛截止时间
    getUserInfo m_userinfo;//用户详细信息
    int m_starttime;//行动力起始时间
    int m_day;//是不是当天的第一次登陆
    int m_week;//是不是当天的第一次登陆
    int m_number;//当前一周的连续登陆天数
    std::vector<GetlvEx>        m_exinfo;//升级数值列表
    std::vector<FriendInfo>     m_friendslist;//好友列表
    std::vector<NotifyInfo>     m_notifyinfo;//通知列表
    std::vector<ShareText>      m_listShareText;//分享显示列表
    std::vector<std::string>    m_friendIdlist;//安装游戏的列表
    std::vector<DailyLogin>     m_listDailyLogin;//奖励列表
    std::vector<WeekRankData>   m_weekRanklist;//星期好友排名
    std::vector<BuyItem>    m_buyItemlist;//购买基本表
    std::vector<TaskData>       m_Tasklist;//周任务列表
    std::vector<ActivityData>   m_Activitylist;//周任务列表
    std::string m_appver;
};

class CDatLoginResponse:public CDatResponse
{
    CDatLoginRequest*m_req;
public:
    CDatLoginResponse(CDatLoginRequest*req);
    CDatLoginResponse(ReqLogin reqLogin,std::string source,std::vector<std::string> list,int m_onid=0);
    int Marshal();
private:
    ReqLogin m_reqLogin;
    std::string m_source;
    std::vector<Uplist> m_uplist;
    std::vector<FriendInfo>m_friendlist;
    std::vector<TaskData>m_missionlist;
    int m_userId;
    
    
};


#pragma mark 更新用户信息  06
class CDatUpdateUserInfoRequest:public CDatRequest
{
    int  m_Info;
public:
    CDatUpdateUserInfoRequest(json::Json&jc);
    int UnMarshal(json::Json &jc);
    int GetUpdateUserInfo();
    std::vector<FriendInfo>     m_friendslist;//好友列表
    std::vector<NotifyInfo>     m_notifyinfo;//通知列表
    
};
class CDatUpdateUserInfoResponse:public CDatResponse
{

    CDatUpdateUserInfoRequest*m_req;
    std::vector<FriendInfo>m_friendslist;
    std::vector<NotifyInfo>m_notifyinfo;
    
    getUserInfo m_Info;//更新用户信息
    std::vector<int> m_list;//道具列表
    int m_time;//更新行动力的开始时间
    int m_isstart;//是否开始游戏 统计游戏次数   0:不是   1是
    int m_type;//是否需要返回list  0:不需要   1需要
public:
    CDatUpdateUserInfoResponse(CDatUpdateUserInfoRequest*req);
    int Marshal();
};

#pragma mark ---------获取行动力的开始时间（11）------- 

class CDatgetActionRequest:public CDatRequest
{
public:
    CDatgetActionRequest(json::Json &jc);
    int UnMarshal(json::Json &jc);
    int GetMonInfo();
private:
    int time;
    
};
class CDatgetActionResponse:public CDatResponse
{
    CDatgetActionRequest*m_req;
public:
    CDatgetActionResponse(CDatgetActionRequest*req);
    int Marshal();

};


#pragma mark Access is already installed little account of the game   17
class CDatInstalledGameRequest:public CDatRequest
{
public:
    CDatInstalledGameRequest(json::Json &jc);
    int UnMarshal(json::Json &jc);
    std::vector<std::string> *GetMonInfo();
private:
    std::vector<std::string> m_notifyinfo;
};
class CDatInstalledGameResponse:public CDatResponse
{
    CDatInstalledGameRequest*m_req;
    int m_userid;
    std::vector<InstalledGame> m_list;
public:
    CDatInstalledGameResponse(CDatInstalledGameRequest*req);
    int Marshal();


};

#pragma mark-----------向好友索取或者赠送行动时通知服务器形成一个message（21）-------
class CDatGiveStrengthRequest:public CDatRequest
{
public:
    CDatGiveStrengthRequest(json::Json &jc);
    int UnMarshal(json::Json &jc);
};
class CDatGiveStrengthResponse:public CDatResponse
{
    CDatGiveStrengthRequest*m_req;
    int m_useId;
    int m_friendId;
    int m_type;
public:
    CDatGiveStrengthResponse(CDatGiveStrengthRequest*req);
    int Marshal();
   
};

static char Hex(const char pos);
static std::string toUTF8(const char* text);
extern int timeStart;
extern int g_sysTimer;


