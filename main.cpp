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

using namespace std;


int main(int argc, const char * argv[]) {
    if(argc < 2){
       cout<<"The server port number is not specified"<<endl;
       exit(0);
    }
    Port server_Port = atoi(argv[1]);
    HTTP_Server server;
    if(!server.initialize_Conn(LOCALHOST,server_Port)){
        cout<<"Socket Creation Error"<<endl;
        exit(0);
    }else{
        cout<<"Socket Created Successfully"<<endl;
    }
    return 0;
}
//this port number is used only for testing
//Port server_Port = 7799;
//cout<<LOCALHOST<<endl;
