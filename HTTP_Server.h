//
//  HTTP_Server.h
//  HTTPServer
//
//  Created by mac on 11/3/18.
//  Copyright © 2018 mac. All rights reserved.
//

#ifndef HTTP_Server_h
#define HTTP_Server_h
#include <string>
#include "types.h"
using namespace std;


class HTTP_Server{
public:
    HTTP_Server();
    //initialize a connection with it's socket
    bool initialize_Conn(IP_Address a,Port p);
    //start listenning on socket client_Socket
    void shutDown();

};


#endif /* HTTP_Server_h */
