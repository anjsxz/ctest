//
//  AJMysql.cpp
//  myserver
//
//  Created by anjun on 13-11-17.
//  Copyright (c) 2013年 anjun. All rights reserved.
//

#include "AJMysql.h"
static AJMysql* _shared;
AJMysql* AJMysql::shared(){
    if (_shared == NULL) {
       _shared = new AJMysql();

    }
    return _shared;
}
AJMysql::AJMysql(){
     m_con = mysql_init(NULL);
    if (m_con == NULL)
    {
        finish_with_error(m_con);
    }
    
    if (mysql_real_connect(m_con, MYSQL_HOST, MYSQL_USER, MYSQL_PASS,
                           MYSQL_DATABASE, 0, NULL, 0) == NULL)
    {
       finish_with_error(m_con);
    }
}
AJMysql::~AJMysql(){
    mysql_close(m_con);
}
bool AJMysql::execSql(std::string sql,long&n){
    if (mysql_query(m_con, sql.c_str())) {
        finish_with_error(m_con);
        return false;
    }
    n = mysql_insert_id(m_con);
    return true;
}

bool AJMysql::query(std::string sql,std::vector<vstring> &rows){
    std::vector<std::string> v;
    if (mysql_query(m_con,sql.c_str()))
    {
        finish_with_error(m_con);
        return false;
    }
    
    MYSQL_RES *result = mysql_store_result(m_con);
    
    if (result == NULL)
    {
        finish_with_error(m_con);
        return false;
    }

    int   num_fields = mysql_num_fields(result);
    
    MYSQL_ROW row = nullptr;
    
    while ((row = mysql_fetch_row(result)))
    {
        for(int j = 0; j < num_fields; j++)
        {
        char t[20] ={0};
        sprintf(t,"%s",row[j] ? row[j] : "NULL");
            v.push_back(t);
        }
        rows.push_back(v);
    }
    
    mysql_free_result(result);
    return true;
}
void AJMysql::finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

