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

    friend int mains(int argc, const char * argv[]) ;

protected:
    thread *main_thread;

    void waitForRequests();
   // friend void habalo(HTTP_Server *s);
    mutex handlers_mutex;
    vector<thread*> request_handlers;

    mutex listening_mutex;
    bool listening;

    IP_Address server_ip;
    Port server_port;
    int serverSocket;
    int max_queued;
    void handle_Request(int socketfd,struct sockaddr_in address);
};


#endif /* HTTP_Server_h */
