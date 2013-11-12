
#include "CDatPacket.h"

USING_NS_CC;

#include <pthread.h>


//#define User_SDK_91
#define HANDLER_VERUP_MSG 100
#define HANDLER_ANALYTICS_EVENT 200

#define SOURCE_TAG     "2"

#define APP_LANGUAGE  1 // 1 for English, 2 for Chinese
#define SCREEN_MIN_X 8
#define SCREEN_MIN_Y 8

#define ios_game
#define IPADORIPHONE  1  ///1琛ㄧずIpad else 0 琛ㄧず iphone



#define wood_row 7   //
#define wood_column 7  //

#define exchange_rate 4  //

#define exchange_time  0.15f 
#define elimi_time    0.0f 
#define move_time     0.2f 

#define TRANSTIME 0.3f

#define MAX_BATFRAME  12


#define BATX_TAG 1111
#define FIRE_TAG 1112
#define MOVE_ACTION 1113
#define FIRE_ACTION 1114
#define ELIMI_WOOD_TAG 1117
#define UPGRADE_WOOD_TAG 1118
#define DOUBLE_HITS_TAG 1119
#define HIGH_SCORE_TAG  1120
#define SCORE_PROCESS_TAG 1121
#define SPIRIT_ACTION 1122
#define DOUBLE_ACTION 1123
#define SLOW_ACTION 1124
#define LIFE_ACTION 1125
#define RESET_LIFE_ACTION 1126
#define LIFE_MENU_TAG 1127
#define SHAKE_SCREEN_ACTION 1128
#define START_FIRE_ACTION 1129
#define ELIMI_WOOD_BACK 1130
#define ELIMI_WOOD_FRONT 1131
#define SCORE_PROCESS1_TAG 1132
#define Wood_ELIMINATEWOODNUM 1133
#define BATX_PLIST 1200
#define WATER_FALL_EFFECT 1201
#define BOW_E_TAG 1202
#define BOW_S_TAG 1203
#define BOW_W_TAG 1204
#define BOW_N_TAG 1205
#define BOW_STAR_PLIST1 1206
#define BOW_STAR_PLIST2 1207
#define BOW_STAR_PLIST3 1208
#define BOW_STAR_PLIST4 1209
#define FIRE_BALL_EFFECT 1210
#define FALL_STAR_EFFECT 1211
#define BORDER_FIRE_EFFECT 1212
#define UNMUTE_ALL_SOUNDS 1213
#define WOOD_NUMBER_TAG 1214
#define LEVEL_UP_TAG 1215
#define BRUTAL_FIRE_TAG 1216
#define WOOD_PRESENTATION  1217
#define WOOD_PRESENTATION_ACTION  1218
#define EXTERN_EMLIN_TAG   1219
#define red_wood 0
#define green_wood 1
#define yellow_wood 2
#define blue_wood 3
#define violet_wood 4
#define white_wood 5
#define black_wood 6

#define normal_wood 0
#define strong1_wood 1
#define lock1_wood 2
#define strong2_wood 3
#define lock2_wood 4
#define elimi_row_wood 5
#define elimi_col_wood 6
#define elimi_cross_wood 7
#define elimi_3x3_wood 8
#define elimi_color_wood 9
#define water_fall_wood 10
#define normal_spirit_wood 11
#define super_spirit_wood 12
#define double_score_wood 13
#define slow_fire_wood 14

#define hard_wood 1 //
#define spirit_wood 2 //
#define encourage_wood 3 //
#define upgrade_wood1 4 //
#define upgrade_wood2 5 //
#define upgrade_wood3 6 //

#define spirit_0 0
#define spirit_1 1
#define spirit_2 2
#define spirit_3 3
#define spirit_4 4
#define spirit_5 5
#define super_spirit_1 6
#define super_spirit_2 7
#define super_spirit_3 8
#define super_spirit_4 9
#define super_spirit_5 10
#define double_score_1 11
#define double_score_2 12
#define double_score_3 13
#define double_score_4 14
#define double_score_5 15
#define slow_fire_1 16
#define slow_fire_2 17
#define slow_fire_3 18
#define slow_fire_4 19
#define slow_fire_5 20

#define SHOP_LINGQI_TAG 230
#define SHOP_WARNING_TAG  234

#define BUYLAYER_LINGQI_TAG 231
#define CULTURELAYER_TAG   232
#define QUESTIONUI_QUESTION_TAG   233


#define BACK_TO_MENU_WARNING 3001
#define LEVEL_UPGRADE_NOTIFICATION 3002
#define BACK_DELETE 3003
#define REPLAY_OK   3004



#define MYFONTNAME "Blod" //"Arial"
#define BLODFONT "Blod"//"blod"
#define FONT_BLACK_ZH  "Blod"
#pragma mark ------------���-------
#define WOODREFRESH_A(x,y)   1000-10*(x)*(y)
#define WOODREFRESH_B(x)   1000-(x)-10
#define WOODREFRESH_B1(x,y)  (1000-(x))*(y)
#define WOODREFRESH_C(x,y,z)   1000-(x)-(y)-(z)
#define WOOD_FIRE_TIME(x)    100.0/(50+(x))    

#define WOOD_SCORE_1(x)   250*((x)-2)
#define WOOD_SCORE_2(x,y)   (500+100*(x))*y
#define WOOD_SCORE_3(x)   300*(x)
#define WOOD_SCORE_4(x)   0
#define WOOD_SCORE_5   1000
#define WOOD_SCORE_6   2000
#define WOOD_SCORE_7   3000
#define WOOD_SCORE_8   5000
#define WOOD_LV_ADDNUM(x,y)    (x)+(y) 
#define WOOD_EXTERN_SCORE(x)  100*(x)
#define WOOD_EXTERN_SCORE1(x)  500*(x)
#define WOOD_EXTERN_SCORE2(x)  1000*(x)
#define WOOD_EXTERN_SCORE3(x)  2000*(x)
#define WOOD_EXTERN_SCORE4(x)  5000*(x)
#define WOOD3_DOUBLE_HIT_SCORE(x) 300*(x)
#define WOOD4_DOUBLE_HIT_SCORE(x) 500*(x)
//fire up speed
#define FIRE_UP_SPEED1(x) 0.1*wood_row*wood_height/120.0
#define FIRE_UP_SPEED2(x) 0.1*wood_row*wood_height/(60-x/2.0)
#define FIRE_UP_SPEED3(x) 0.1*wood_row*wood_height/(50-x/4.0)
#define FIRE_UP_SPEED4(x) 0.1*wood_row*wood_height/(40.0)
#define FIRE_UP_SPEED5 wood_row*wood_height/30.0
#define FIRE_DOWN_SPEED wood_row*wood_height/120.0

#define COMBOINALL  8 //

#define SPEEDADDTIMEDELAY (60*8)    //
#define NOTIFICATION_UPDATESPD  "notifi_updateSpd" //
#define GETEXFORMULA(x)    ((x)/100000)+1
#define MESSAGE_LOGINLAYER "getmessage"


#define MESSAGE_REGISTER "getregister"
#define MESSAGE_UPDATEUSERINFO  "updateuserinfo"
#define MESSAGE_UPDATMONINFO  "updateuMoninfo"
#define MESSAGE_ACHIEVE  "updateuAchieve"
#define MESSAGE_SPDBEGINTIME  "updatespeedbetime"
#define MESSAGE_GETACHIEVE "netgetachieve"
#define MESSAGE_GETUSERINFO "netgetuserinfo"
#define MESSAGE_HOMECHANGE  "homechange"

#define MESSAGE_APPDELEGATE_NET  "appdelenet"
#define BUY_ADVERTISEMENT  "advertisement"//判断充值界面的的广告是否购买
#define WEEKTASK_SCORE_TIME "weaktask_score_time"//取的最高分时间
#define WEEKTASK_SCORE "weektask_score" //周任务最高分

#define HEADXML     "RE"
#define UserIDXML   "myuserId"  //用户登录的userid
#define FACEBOOKUPTETIME  "facebookupdatetime"   //更新facebook好友使得时间
#define BANBENHAO  "append"   //系统版本号设置
#define ISFACEBOOK "isfacebook" //是否登入facebook
#define FACEBOOKID "facebook_otherid"

#define MY_BANNER_UNIT_ID @"a152255b024b21a"
#define BANNER_UNIT_ID "a152255b024b21a"
#define GAMEOVER_ADDSCORE_TIME 2

#define HELLOLWORD_TAG    300000

#define FULL_ACTION_NUM     5

#ifndef _STU_H_
#define _STU_H_

#define WINSIZE  CCDirector::sharedDirector()->getWinSize()
#define SCALEX WINSIZE.width/320
#define SCALEY WINSIZE.height/480
typedef struct tag_stuPhoneNum
{
	std::string str_name;
	std::string str_phone;
}stuPhoneNum;

typedef struct SetData
{
    int Sound;//�虫�
    int Music;//����充�
    int Culture;
}SetData;

struct MemoryStruct {
    char *memory;
    size_t size;
};
#endif
extern int g_time;
extern int g_userid;
extern int g_speedLastTimer;//行动力剩余时间
extern int g_speedBeginTimer;
extern getUserInfo g_stuMyUser;  //用户表
extern int g_sqLastTime;
extern int m_week;//
extern int m_day;//
extern int m_number;//
extern int curDevice;


extern int g_bOf;
extern std::string g_curUserName;
extern int _mgold;
extern float g_slowRate;
extern int wood_width;
extern int wood_height;

extern pthread_mutex_t     m_dbMutex;


//select * from (select * from MyHobby order by goTime) order by id desc
