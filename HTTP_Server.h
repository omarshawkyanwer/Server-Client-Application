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
#include "HTTP_Handler.h"
#include <thread>
#include <mutex>
#include <vector>
using namespace std;


class HTTP_Server{
public:
    HTTP_Server();
    //initialize a connection with it's socket
    bool initialize_Conn(IP_Address a,Port p);

    //start listenning on port p
    void run_Conn();
    //to exit the HTTP server and make it down
    void pause_Conn();

    void shutDown();
    friend int main(int argc, const char * argv[]) ;
    ~HTTP_Server();

protected:
    thread *main_thread;
    thread *monitor_thread;

    void waitForRequests();
    mutex handlers_mutex;
    bool is_alive;
    //map<thread*,HTTP_Handler *> request_handlers;
    HTTP_Handler * request_handlers[50];
    bool used[50];
    int living_clients;
    mutex listening_mutex;
    bool listening;
    void monitor_connections();


    IP_Address server_ip;
    Port server_port;
    int serverSocket;
    int max_queued;
    void handle_Request(int socketfd,struct sockaddr_in address);

};


#endif /* HTTP_Server_h */
