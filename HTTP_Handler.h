//
//  HTTP_Handler.h
//  HTTPServer
//
//  Created by mac on 11/3/18.
//  Copyright © 2018 mac. All rights reserved.
//

#ifndef HTTP_Handler_h
#define HTTP_Handler_h
#include <ctime>
#include <string>
#include <vector>
#include "types.h"
using namespace std;

class HTTP_Handler{
private:
    Port client_Port;
    IP_Address client_IP_Address;
public:
    HTTP_Handler(int client_Socket);
    time_t get_Last_Request();
    void shutdown();
    void run();
protected:
    bool validate_Request();
    vector<string> parse_Request(string request);
    //at least aquire lock on a certin resource
    string get_Resource(string resource_Name);
    string get_HTTP_Response();
    void send_Response(string response);
};

#endif /* HTTP_Handler_h */
