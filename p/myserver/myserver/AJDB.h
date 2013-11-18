//
//  AJDB.h
//  myserver
//
//  Created by anjun on 13-11-17.
//  Copyright (c) 2013年 anjun. All rights reserved.
//

#ifndef __myserver__AJDB__
#define __myserver__AJDB__
#include "AJMysql.h"
#include "CDatPacket.h"
#include "common.h"
class AJDB:public AJMysql{
public:
    static int userRegister(ReqLogin&user);
    static int userLogin(int userid,LoginInfo&login);
};

#endif /* defined(__myserver__AJDB__) */
