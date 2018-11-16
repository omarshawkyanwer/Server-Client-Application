    //
//  HTTP_Server.cpp
//  HTTPServer
//
//  Created by mac on 11/4/18.
//  Copyright © 2018 mac. All rights reserved.
//

#include <cstdio>
#include<iostream>
#include <cstdlib>
#include <cstring>
#include "HTTP_Server.h"
#include "types.h"
#include "HTTP_Handler.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <thread>         // std::thread
#include <mutex>

using namespace std;

//default constructor
HTTP_Server::HTTP_Server(){
    listening = true;
    max_queued = MAXCONN;
}

bool HTTP_Server::initialize_Conn(IP_Address ip_Address,Port port_Number){
    int serverSocket;
    struct sockaddr_in serverAddr,remoteAddr;

    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0)
        return false;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port_Number);
    serverAddr.sin_addr.s_addr = inet_addr(ip_Address.c_str());
    int bind_Socket = bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    this->serverSocket = serverSocket;
    if(bind_Socket < 0 )
    {
            return false;
    }
    int optval = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    main_thread = new thread(&HTTP_Server::waitForRequests,this);

    return true;
}

void HTTP_Server::shutDown(){
    exit(0);
}
//move semantics is used not to copy the request string : it may a large one.
void HTTP_Server::handle_Request(int socketfd,struct sockaddr_in address){
    cout <<"socket "<<socketfd <<" "<<address.sin_port<<endl;
    HTTP_Handler *hand = new HTTP_Handler(socketfd,address);
    hand->pool_requests();


}
void HTTP_Server::run_Conn() {

    listening = true;

}
void HTTP_Server::pause_Conn(){

    listening = false;
}

void HTTP_Server::waitForRequests(){
unique_lock<mutex> lck(listening_mutex);
    lck.unlock();
    socklen_t sin_size = sizeof(struct sockaddr_in);
    struct sockaddr_in their_addr;
while(1) {

    if(!listen(serverSocket,max_queued)) {
        while(listening) {
            int newSocket = accept(serverSocket,(struct sockaddr*)&their_addr,&sin_size);
          //  printf("..\n");
            if(listening)
               thread *new_thread = new thread(&HTTP_Server::handle_Request,this,newSocket,their_addr);
                //handle_Request(newSocket,their_addr);
            else
                close(newSocket);

        }
        lck.lock();
        lck.unlock();
    }

}


}
