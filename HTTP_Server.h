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
    //start listenning on port p
    void run_Conn(Port p);
    //to exit the HTTP server and make it down
    void shutDown();
protected:
    void handle_Request(string req);
};


#endif /* HTTP_Server_h */
