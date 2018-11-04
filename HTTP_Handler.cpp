//
//  HTTP_Handler.cpp
//  HTTPServer
//
//  Created by mac on 11/4/18.
//  Copyright © 2018 mac. All rights reserved.
//

#include "HTTP_Handler.h"
#include "types.h"
using namespace std;


HTTP_Handler::HTTP_Handler(Port p,IP_Address ip){
    
}
time_t HTTP_Handler::get_Last_Request(){
    return time(nullptr);
}
void HTTP_Handler::shutdown(){
    
}

bool HTTP_Handler::validate_Request(){
    return false;
}
vector<string> HTTP_Handler::parse_Request(string request){
    return vector<string>();
}

string HTTP_Handler::get_Resource(string resource_Name){
    return string("bla");
}
string HTTP_Handler::get_HTTP_Response(){
      return string("bla");
}
void HTTP_Handler::send_Response(string response){
    
}

