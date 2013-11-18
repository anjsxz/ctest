//
//  AJMysql.h
//  myserver
//
//  Created by anjun on 13-11-17.
//  Copyright (c) 2013年 anjun. All rights reserved.
//

#ifndef __myserver__AJMysql__
#define __myserver__AJMysql__


#include "common.h"
#include "mysql.h"

class AJMysql {
    MYSQL*m_con;
public:
    AJMysql();
    ~AJMysql();
    static AJMysql* shared();
    void finish_with_error(MYSQL *con);
   
    bool query(std::string sql,std::vector<vstring> &rows);
   
    bool execSql(std::string sql,long&n);
   void test();
  
};
#endif /* defined(__myserver__AJMysql__) */
