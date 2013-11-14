//
//  MyServer.cpp
//  myserver
//
//  Created by anjun on 13-11-14.
//  Copyright (c) 2013年 anjun. All rights reserved.
//

#include "MyServer.h"
MyServer::MyServer(int port,char* hostname):TcpServThr(port,hostname){
    con = new MyCondition;
}
MyServer::~MyServer(){
    delete con;
}
int main()
{
    MyServer  server(6001);
    //  Server::Sender* s;
    //  s = new Server::Sender(1,NULL);
    server.Init();
    server.Run();
    return 0;
}