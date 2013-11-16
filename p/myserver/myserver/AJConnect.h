//
//  AJConnect.h
//  myserver
//
//  Created by anjun on 13-11-15.
//  Copyright (c) 2013年 anjun. All rights reserved.
//

#ifndef __myserver__AJConnect__
#define __myserver__AJConnect__

#include "stdafx.h"
#include  "CDatPacket.h"
class AJConnect{
    
 public:
    
    CDatResponse* m_res;
    CDatRequest* m_req;
    AJConnect(CDatRequest*,CDatResponse*);
};
#endif /* defined(__myserver__AJConnect__) */
