//
//  MyServer.h
//  myserver
//
//  Created by anjun on 13-11-14.
//  Copyright (c) 2013年 anjun. All rights reserved.
//

#ifndef __myserver__MyServer__
#define __myserver__MyServer__
#include "TcpServThr.h"
#include "MySync.h"
#include <iostream>
class MyServer:TcpServThr{
protected:
    MyCondition *con;
public:
    MyServer(int port,char *hostname = NULL);
  virtual  ~MyServer();
    
};
#endif /* defined(__myserver__MyServer__) */
