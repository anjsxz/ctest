//
//  AJDB.cpp
//  myserver
//
//  Created by anjun on 13-11-17.
//  Copyright (c) 2013年 anjun. All rights reserved.
//

#include "AJDB.h"
int AJDB::userRegister(ReqLogin &user){
    long n=0;
    bool b=  AJMysql::shared()->execSql("insert user values()",n);
    if (b) {
        char str_n[11]={0};
        sprintf(str_n,"%ld",n);
        char s_name[20]={0};
        sprintf(s_name,"Player-%ld",n);
        std::string username=  s_name;
        user.username= username;
        user.othername= username;
        long update_n=0;
        std::string sql="update user set action=5,diamond=20, username = \'"+username+"\',othername= \'"+username+" \' where userid="+str_n;
        printf("\nsql:\n%s\n",sql.c_str());
       bool b2= AJMysql::shared()->execSql(sql,update_n);
        if (b2)
         return (int)n;
        return -1;
    }
    return -1;
}
int AJDB::userLogin(int userid,LoginInfo&login){
    std::vector<vstring> rows;
    char sql[200]={0};
    std::vector<std::string> row;
    sprintf(sql,"select userid,datetime,starttime,username,otherid,othername,email,age,sex,score,diamond,action,highscore,lv,curex,goldnum,silvernum,bronzenum,day,week,num from user where userid=%d",userid);
    printf("\nquery:sql\n%s\n",sql);
    AJMysql::shared()->query(sql,rows);
    if (rows.size()>0) {
        row =rows[0];
        login.userinfo.userid   =   row[0].compare("NULL")==0?0:atoi(row[0].c_str());
        login.userinfo.datetime =   row[1].compare("NULL")==0?0:atoi(row[1].c_str());
        login.userinfo.starttime =  row[2].compare("NULL")==0?0:atoi(row[2].c_str());
        login.userinfo.username =   row[3].compare("NULL")==0?"":row[3].c_str();
        login.userinfo.otherid =    row[4].compare("NULL")==0?"":row[4].c_str();
        login.userinfo.othername =  row[5].compare("NULL")==0?"":row[5].c_str();
        login.userinfo.email =      row[6].compare("NULL")==0?"":row[6].c_str();
        login.userinfo.age =        row[7].compare("NULL")==0?0:atoi(row[7].c_str());
        login.userinfo.sex =        row[8].compare("NULL")==0?0:atoi(row[8].c_str());;
        login.userinfo.score =      row[9].compare("NULL")==0?0:atoi(row[9].c_str());
        login.userinfo.diamond =    row[10].compare("NULL")==0?0:atoi(row[10].c_str());
        login.userinfo.action =     row[11].compare("NULL")==0?0:atoi(row[11].c_str());
        login.userinfo.highscore =  row[12].compare("NULL")==0?0:atoi(row[12].c_str());
        login.userinfo.lv =         row[13].compare("NULL")==0?0:atoi(row[13].c_str());
        login.userinfo.curex =      row[14].compare("NULL")==0?0:atoi(row[14].c_str());
        login.userinfo.goldnum =    row[15].compare("NULL")==0?0:atoi(row[15].c_str());
        login.userinfo.silvernum =  row[16].compare("NULL")==0?0:atoi(row[16].c_str());
        login.userinfo.bronzenum =  row[17].compare("NULL")==0?0:atoi(row[17].c_str());
        login.userinfo.day =        row[18].compare("NULL")==0?0:atoi(row[18].c_str());
        login.userinfo.week =       row[19].compare("NULL")==0?0:atoi(row[19].c_str());
        login.userinfo.num =        row[20].compare("NULL")==0?0:atoi(row[20].c_str());
    }
    //////////
    rows.clear();
    AJMysql::shared()->query("select * from uplist",rows);
    for (int i=0; i<rows.size(); i++) {
        row =rows[i];
        GetlvEx lvex;
        lvex.lv   =   row[0].compare("NULL")==0?0:atoi(row[0].c_str());
        lvex.levupex   =   row[1].compare("NULL")==0?0:atoi(row[1].c_str());
        lvex.type   =   row[2].compare("NULL")==0?0:atoi(row[2].c_str());
        lvex.bonuscore   =   row[3].compare("NULL")==0?0:atoi(row[3].c_str());
        lvex.reward   =   row[4].compare("NULL")==0?0:atoi(row[4].c_str());
        lvex.getEx   =   row[5].compare("NULL")==0?0:atoi(row[5].c_str());
        login.uplist.push_back(lvex);
    }
    //////////
    rows.clear();
    sprintf(sql,"select *  from get_friendlist where friendid=%d",userid);
    printf("\nquery:sql\n%s\n",sql);
    AJMysql::shared()->query(sql,rows);
    for (int i=0; i<rows.size(); i++) {
        row =rows[i];
        FriendInfo o;
        o.friendid   =   row[0].compare("NULL")==0?0:atoi(row[0].c_str());
        o.otherid   =   row[1].compare("NULL")==0?"":row[1].c_str();
        o.friendname   =   row[2].compare("NULL")==0?"":row[2].c_str();
        o.friendscore   =   row[3].compare("NULL")==0?0:atoi(row[3].c_str());
        o.maxscore   =   row[4].compare("NULL")==0?0:atoi(row[4].c_str());
        o.goldnum   =   row[5].compare("NULL")==0?0:atoi(row[5].c_str());
        login.friendslist.push_back(o);
    }
    //////////
    rows.clear();
    sprintf(sql,"select *  from getmessage_list where friendid=%d",userid);
    printf("\nquery:sql\n%s\n",sql);
    AJMysql::shared()->query(sql,rows);
    for (int i=0; i<rows.size(); i++) {
        row =rows[i];
        NotifyInfo o;
        o.notifyid   =   row[0].compare("NULL")==0?0:atoi(row[0].c_str());
        o.notifytype   =   row[1].compare("NULL")==0?0:atoi(row[1].c_str());
        o.friendId   =   row[2].compare("NULL")==0?0:atoi(row[2].c_str());
        o.otherid   =   row[3].compare("NULL")==0?"":row[3].c_str();
        o.othername   =   row[4].compare("NULL")==0?"":row[4].c_str();
        o.datetime   =   row[5].compare("NULL")==0?0:atoi(row[5].c_str());
        login.notifylist.push_back(o);
    }
    //////////
    rows.clear();
    sprintf(sql,"select *  from share_data");
    printf("\nquery:sql\n%s\n",sql);
    AJMysql::shared()->query(sql,rows);
    for (int i=0; i<rows.size(); i++) {
        row =rows[i];
        ShareText o;
        o.type   =   row[0].compare("NULL")==0?0:atoi(row[0].c_str());
        o.game   =   row[1].compare("NULL")==0?"":row[1].c_str();
        o.webo   =   row[2].compare("NULL")==0?"":row[2].c_str();
        o.url   =   row[3].compare("NULL")==0?"":row[3].c_str();
        o.mark   =   row[4].compare("NULL")==0?"":row[4].c_str();
        
        login.showlist.push_back(o);
    }
    //////////
    rows.clear();
    sprintf(sql,"select otherid  from ungame_data");
    printf("\nquery:sql\n%s\n",sql);
    AJMysql::shared()->query(sql,rows);
    for (int i=0; i<rows.size(); i++) {
        row =rows[i];
        InstalledGame o;
        o.id   =   row[0].compare("NULL")==0?"":row[0].c_str();
        login.list.push_back(o);
    }
    //////////
//    rows.clear();
//    sprintf(sql,"select *  from daily_login ");
//    printf("\nquery:sql\n%s\n",sql);
//    AJMysql::shared()->query(sql,rows);
//    for (int i=0; i<rows.size(); i++) {
//        row =rows[i];
//        DailyLogin o;
//        o.num   =   row[0].compare("NULL")==0?0:atoi(row[0].c_str());
//        o.sliver   =   row[1].compare("NULL")==0?0:atoi(row[1].c_str());
//   
//        login.awardlist.push_back(o);
//    }
    //////////
    rows.clear();
    sprintf(sql,"select *  from week_rank_data where friendid=%d",userid);
    printf("\nquery:sql\n%s\n",sql);
    AJMysql::shared()->query(sql,rows);
    for (int i=0; i<rows.size(); i++) {
        row =rows[i];
        WeekRankData o;
        o.friendid   =   row[0].compare("NULL")==0?0:atoi(row[0].c_str());
        o.friendname   =   row[1].compare("NULL")==0?"":row[1].c_str();
        o.otherid   =   row[2].compare("NULL")==0?"":row[2].c_str();
        o.friendscore   =   row[3].compare("NULL")==0?0:atoi(row[3].c_str());
        login.weeklist.push_back(o);
    }
    //////////
    rows.clear();
    sprintf(sql,"select *  from buyitem_data");
    printf("\nquery:sql\n%s\n",sql);
    AJMysql::shared()->query(sql,rows);
    for (int i=0; i<rows.size(); i++) {
        row =rows[i];
        BuyItem o;
        o.type   =   row[0].compare("NULL")==0?0:atoi(row[0].c_str());
        o.itemname   =   row[1].compare("NULL")==0?"":row[1].c_str();
        o.itemdes   =   row[2].compare("NULL")==0?"":row[2].c_str();
        o.itemid   =   row[3].compare("NULL")==0?"":row[3].c_str();
        o.itemprice   =   row[4].compare("NULL")==0?"":row[4].c_str();
        o.itemnum   =   row[5].compare("NULL")==0?0:atoi(row[5].c_str());
        o.itemallnum   =   row[5].compare("NULL")==0?0:atoi(row[5].c_str());
        login.itemlist.push_back(o);
    }
    //////////
    rows.clear();
    sprintf(sql,"select *  from missionlist");
    printf("\nquery:sql\n%s\n",sql);
    AJMysql::shared()->query(sql,rows);
    for (int i=0; i<rows.size(); i++) {
        row =rows[i];
        TaskData o;
        o.week   =   row[0].compare("NULL")==0?0:atoi(row[0].c_str());
        o.type   =   row[1].compare("NULL")==0?0:atoi(row[1].c_str());
        o.num   =   row[2].compare("NULL")==0?0:atoi(row[2].c_str());
        o.description_text   =   row[3].compare("NULL")==0?"":row[3].c_str();
        o.award_num   =   row[4].compare("NULL")==0?0:atoi(row[4].c_str()) ;
        o.award_type   =   row[5].compare("NULL")==0?0:atoi(row[5].c_str());
        login.missionlist.push_back(o);
    }
    //////////
    rows.clear();
    sprintf(sql,"select *  from missionlist");
    printf("\nquery:sql\n%s\n",sql);
    AJMysql::shared()->query(sql,rows);
    for (int i=0; i<rows.size(); i++) {
        row =rows[i];
        ActivityData o;
        o.playid   =   row[0].compare("NULL")==0?0:atoi(row[0].c_str());
        o.url   =   row[1].compare("NULL")==0?"":row[1].c_str();
        o.diamond   =   row[2].compare("NULL")==0?0:atoi(row[2].c_str());
        o.isjoin   =   row[3].compare("NULL")==0?0:atoi(row[3].c_str());
        login.playlist.push_back(o);
    }
    return -1;
}