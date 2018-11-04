    //
//  HTTP_Server.cpp
//  HTTPServer
//
//  Created by mac on 11/4/18.
//  Copyright © 2018 mac. All rights reserved.
//

#include <stdio.h>
#include "HTTP_Server.h"
#include "types.h"
#include "HTTP_Handler.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

//default constructor
HTTP_Server::HTTP_Server(){}

bool HTTP_Server::initialize_Conn(IP_Address ip_Address,Port port_Number){
    int serverSocket;
    struct sockaddr_in serverAddr;
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0)
        return false;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port_Number);
    serverAddr.sin_addr.s_addr = inet_addr(ip_Address.c_str());
    int bind_Socket = bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if(bind_Socket < 0 )
        return false;
    if(listen(serverSocket, MAXCONN) < 0)
        return false;
    return true;
}

void HTTP_Server::shutDown(){
    exit(0);
}
//move semantics is used not to copy the request string : it may a large one.
void HTTP_Server::handle_Request(Port p,IP_Address ip,string && req){
    HTTP_Handler request_handler(p,ip,move(req));
}
