//
//  Response_Builder.cpp
//  HTTPServer
//
//  Created by mac on 11/7/18.
//  Copyright © 2018 mac. All rights reserved.
//

#include <stdio.h>
#include "Response_Builder.h"
#include <string>
#include <iostream>
#include <map>
#include "types.h"
using namespace std;



/*
 rest of the file extensions to be hard-coded at any any
 time but i have tried only those to prove the concept
 
 */
map<string,string> content_Types = {
    {"txt","text/plain"},
    {"html","text/html"}
};


//default constructor
Response_Builder::Response_Builder(){}



/*
 if state = true --> 200 OK
 else NOTFOUND
 */
void Response_Builder::set_Status(bool state){
    if(state)
        status = OK;
    else
        status = NOTFOUND;
}

//set the http version using move semantics
void Response_Builder::set_HTTP_Vesion(string version){
    HTTP_Version = version;
}

/*
 set both request body (content) and also the content length
 */
void Response_Builder::set_Content(string && content){
    content_Length = to_string(content.length());
    respone_Body = move(content);
}

void Response_Builder::set_Content_Type(string type){
    content_Type = content_Types[type];
}

string Response_Builder::build_Response(){
    string respone = HTTP_Version + " " + status + "\r\n";
    //if there is a content body
    if(atoi(content_Length.c_str()) > 0){
        respone += "Content-Length: " + content_Length + "\r\n"
        + "Content-Type: " + content_Type + "\r\n"
        + "\n"
        + respone_Body;
    }
    return respone;
}
