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
#include <map>
using namespace std;
class Request_Parser;
struct HTTP_Request{

    HTTP_Request_type type;
    map<string,string> headers;
    string resource;
    HTTP_VERSION version;
};
struct HTTP_Response {
        HTTP_VERSION version;
        int status_code;
        map<string,string> headers;
        string resource;

};
class HTTP_Handler{
private:

public:

    HTTP_Handler(int socketfd,struct sockaddr_in address);
    time_t get_Last_Request();
    void shutdown();
    friend class HTTP_Server;

protected:
    Request_Parser *parser;
    char buffer[2048];
    Port client_Port;
    IP_Address client_IP_Address;
    int socketfd;
    bool validate_Request(string req);
    int  parse_Request(string request,struct HTTP_Request * req);
    //at least aquire lock on a certin resource
    int get_Resource(string resource_Name,char *res);
    string  get_HTTP_Response(int status,string resource);
    void pool_requests();
    int send_Response(string response);
};


#endif /* HTTP_Handler_h */
