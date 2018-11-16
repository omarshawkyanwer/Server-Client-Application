//
//  HTTP_Handler.cpp
//  HTTPServer
//
//  Created by mac on 11/4/18.
//  Copyright © 2018 mac. All rights reserved.
//

#include "HTTP_Handler.h"
#include <iostream>
#include<cstring>
#include "types.h"
#include "Request_Parser.h"
       #include <arpa/inet.h>
using namespace std;


HTTP_Handler::HTTP_Handler(int socketfd,struct sockaddr_in address){
    this->socketfd = socketfd;
    sockaddr_in add;
    this->client_Port = add.sin_port;
    char ip_add[20];
    this->client_IP_Address = inet_ntop(AF_INET,&add.sin_addr.s_addr,ip_add,20);
    parser = new Request_Parser();

}
time_t HTTP_Handler::get_Last_Request(){
    return time(nullptr);
}
void HTTP_Handler::shutdown(){

}

bool HTTP_Handler::validate_Request(string req){
    return parser->validate(req);
}
int HTTP_Handler::parse_Request(string request,struct HTTP_Request * req){

    if(validate_Request(request))
        *req =  parser->parse(request);
    else {
        return 400;
    }
    return 0;
}
void HTTP_Handler::pool_requests(){

    char file[2048];
    struct HTTP_Request req;
    do {
        int bcount = recv(this->socketfd,buffer,sizeof(buffer),0);
        int stat1 = this->parse_Request(string(buffer),&req);
        int status = this->get_Resource(req.resource,file);
        cout<<".."<<stat1 << status<<string(file)<<endl;
        //string resp = this->get_HTTP_Response(status,string(file));
        this->send_Response(string(file));

    }while(false);
    cout<<"request handled successfully\n";

}

int HTTP_Handler::get_Resource(string resource_Name,char *res){
    strcpy(res,resource_Name.c_str());
    return 0;
}
string HTTP_Handler::get_HTTP_Response(int status,string resource){
//      stringstream ss;

}
int  HTTP_Handler::send_Response(string response){
     return send(socketfd,response.c_str(),response.size(),0);
}

