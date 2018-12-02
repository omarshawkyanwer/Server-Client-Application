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
    is_alive = true;
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
    is_alive = true;
    living_clients = 0;
    memset(used,0, sizeof(used));
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    main_thread = new thread(&HTTP_Server::waitForRequests,this);
    monitor_thread = new thread(&HTTP_Server::monitor_connections,this);
    return true;
}

void HTTP_Server::shutDown(){
    pause_Conn();
    thread *t;
    HTTP_Handler *h;
    is_alive = false;
    for (i : request_handlers) {

     h = i;
     h->shutdown();
     t->join();
     //delete(h);
    }
    main_thread->join();
    monitor_thread->join();

}
//move semantics is used not to copy the request string : it may a large one.
void HTTP_Server::handle_Request(int socketfd,struct sockaddr_in address){
    cout <<"socket "<<socketfd <<" "<<address.sin_port<<endl;
    HTTP_Handler *hand = new HTTP_Handler(socketfd,address);
    int i = 0;
    unique_lock<mutex> lck(handlers_mutex);
    while (used[i])
        i++;
    request_handlers[i] = hand;
    used[i] = 1;
    living_clients++;
    if(living_clients == 50)
        pause_Conn();
    lck.unlock();

    hand->pool_requests();


    //delete hand;

    cout<<"request handled successfully"<<endl;
        lck.lock();

        request_handlers[i] = NULL;
        used[i] = 0;
        living_clients--;
        if(living_clients == 49)
            run_Conn();
        lck.unlock();

}

void HTTP_Server::monitor_connections() {

    int ind = 0;
    while(is_alive) {

        time_t now = time(0);
        unique_lock<mutex> lck(handlers_mutex);
        if(!used[ind])
        {
            ind++;
            ind%=50;
            lck.unlock();
            continue;
        }
        HTTP_Handler* elem = request_handlers[ind];
        lck.unlock();
        if(elem == NULL)
            continue;
        double secs = (now - elem->get_Last_Request())/CLOCKS_PER_SEC;
        if(secs > 10.0) {
            elem->shutdown();
            lck.lock();
            used[ind] = 0;
            living_clients--;
            if(living_clients == 49)
                run_Conn();
            lck.unlock();
        }
        ind++;
        ind%=50;

    }
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
while(is_alive) {

    if(!listen(serverSocket,max_queued)) {
        while(listening) {
            int newSocket = accept(serverSocket,(struct sockaddr*)&their_addr,&sin_size);
            if(!listening) {
                close(newSocket);
                continue;
            }
                new thread(&HTTP_Server::handle_Request,this,newSocket,their_addr);


        }
        lck.lock();
        lck.unlock();
    }

}


}
HTTP_Server::~HTTP_Server() {

    delete main_thread;
    delete monitor_thread;
}
