//
//  main.cpp
//  HTTPServer
//
//  Created by mac on 11/3/18.
//  Copyright © 2018 mac. All rights reserved.
//

#include <iostream>
#include "types.h"
#include "HTTP_Server.h"
#include "Request_Parser.h"

using namespace std;


int main(int argc, const char * argv[]) {

    Port server_Port;

    if(argc < 2){
       cout<<"The server port number is not specified"<<endl;
       //exit(0);
       server_Port = 7989;
    }

    else
     server_Port = atoi(argv[1]);

    HTTP_Server server;
    Port server_Port = 7799;
    if(!server.initialize_Conn(LOCALHOST,server_Port)){
        cout<<"Socket Creation Error"<<endl;
        exit(0);
    }else{
        cout<<"Socket Created Successfully"<<endl;
        while(1);
    }
     */
    Request_Parser p;
    string req = "Get /home/omar.txt HTTP1.1\r\nContent_Type:txt\r\nContent_Lenght:anything";
    vector<string> v = p.parse_Request(req);
    for(string s : v){
        cout<<s<<endl;
    }
    server.main_thread->join();
    return 0;
}
//this port number is used only for testing
//Port server_Port = 7799;
// cout<<LOCALHOST<<endl;
