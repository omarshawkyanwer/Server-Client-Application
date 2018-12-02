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
#include "Response_Builder.h"
#include <regex>

using namespace std;

int main(int argc, const char * argv[]) {
    /*string req = "POST hacks.txt HTTP/1.0\r\nHOST: ana\r\nhacks fakes\r\n";
    regex e ("(GET|POST|PUT|HEAD|DELETE) +([^ ]+) +HTTP/1\.(0|1)\r?\n([^ ^:]+: [^\n^\r]+\r?\n)*(.*\r?\n)*");
    smatch m;
    bool b = regex_match(req,m,e);
    if(!b) {
        cout << "bad request\n" << req << endl;
    }
    return 0;*/
    Port server_Port;

    if(argc < 2) {
       cout<<"The server port number is not specified"<<endl;
       server_Port = 8000;
    }

    else
     server_Port = atoi(argv[1]);

    HTTP_Server server;
    //server_Port = 7799;
    if(!server.initialize_Conn(LOCALHOST,server_Port)){
        cout<<"Socket Creation Error"<<endl;
        exit(0);
    }else{
        cout<<"Socket Created Successfully"<<endl;
        //while (1);
    }
    server.main_thread->join();

    return 0;
}
//this port number is used only for testing
//Port server_Port = 7799;
// cout<<LOCALHOST<<endl;
