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
class File_System;
struct HTTP_Request{

    HTTP_Request_type type;
    map<string,string> headers;
    string resource;
    HTTP_VERSION version;
    string body;
};
struct HTTP_Response {
        HTTP_VERSION version;
        int status_code;
        map<string,string> headers;
        string resource;


};
const bool operator ==(const struct  HTTP_Request &req1,const struct HTTP_Request &req2);

const bool operator !=(const struct  HTTP_Request &req1,const struct HTTP_Request &req2);
const bool operator ==(const struct  HTTP_Response &req1,const struct HTTP_Response &req2);

const bool operator !=(const struct  HTTP_Response &req1,const struct HTTP_Response &req2);
class HTTP_Handler{
private:

public:

    HTTP_Handler(int socketfd,struct sockaddr_in address);
    time_t get_Last_Request();
    void shutdown();
    friend class HTTP_Server;
    ~HTTP_Handler();
protected:
    bool keep_alive;
    bool terminate;
    time_t last_req;
    Request_Parser *parser;
    char buffer[204800];
    int buffer_len;
    Port client_Port;
    IP_Address client_IP_Address;
    File_System *fsys;
    int socketfd;
    bool validate_Request(string req);
    int  parse_Request(string request,struct HTTP_Request * req);
    //at least aquire lock on a certin resource
    HTTP_Response handle_request( HTTP_Request req);
    int get_Resource(string resource_Name,char *res);
    string  get_HTTP_Response(HTTP_Response response);
    HTTP_Response handle_get(HTTP_Request *re);
    HTTP_Response handle_post(HTTP_Request *re);
    string recv_data(int len);
    string recv_request();
    void pool_requests();
    int send_Response(string response);
    void recv_parse(HTTP_Request &request,int &status);
    void handle_thread(HTTP_Request & request,HTTP_Response &resp,int &status);
    void respond_thread(HTTP_Response &resp);
};


#endif /* HTTP_Handler_h */
