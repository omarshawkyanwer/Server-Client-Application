//
//  Request_Parser.cpp
//  HTTPServer
//
//  Created by mac on 11/4/18.
//  Copyright © 2018 mac. All rights reserved.
//

#include <stdio.h>
#include<iostream>
#include "Request_Parser.h"
#include <vector>
#include <string>


#include "types.h"
#include <regex>


using namespace std;

//default constructor
Request_Parser::Request_Parser(){}


struct HTTP_Request Request_Parser::parse(string req){

     std::smatch m;
      regex e ("(GET|POST|PUT|HEAD|DELETE) ([^ ]+) HTTP\/1\.(0|1)\r?\n((\s*[^ ^:]+\s*:\s*[^\n^\r]+\r?\n)*(\r?\n)*((.*\r?\n)*))");
    regex_search(req,m,e);
    vector<string> parts;
    for (auto i:m) {
    cout<<parts.size()<<" "<<i<<"..\n";
    parts.push_back(i);
    }
    cout<<m.size()<<"....\n";
    struct HTTP_Request request;
    request.type = (parts[1]=="GET")? GET:POST;
    request.resource = parts[2];
    request.version = (parts[3]=="0")?HTTP_1_0:HTTP_1_1;
        string headers = parts[4];
      smatch h;
        //regex regx("\s*([^ ^:]+)\s*:\s*([^\n]+)\n");
        regex regx(" *([^ ^:]+) *: *([^\n^\r]+) *\r?\n");
        while(regex_search(headers,h,regx)){
        vector<string> vals;
        for (auto i:h) {
            vals.push_back(i);
        }
        request.headers[vals[1]] = vals[2];
        headers =  h.suffix().str();
        cout<<vals[1]<<".\."<<vals[2]<<endl;

        }
        request.body = headers;
        cout<<"parsing ended\n";
        return request;
}
bool Request_Parser::validate(string req) {
      regex e ("(GET|POST|PUT|HEAD|DELETE) +([^ ]+) +HTTP\/1\.(0|1)\r?\n( *[^ :]+: +[^\n^\r]+\r?\n)*(\r?\n)?((.+\r?\n)*)\r?\n");
    smatch m;
    bool b = regex_match(req,m,e);
    if(!b) {
        cout << "bad request\n" << req << endl;

    }
    return  b;
}
bool Request_Parser::no_request(string req){
    regex e ("( *\r?\n)*");
    smatch m;
    bool b = regex_match(req,m,e);
    return b;

}

string Request_Parser::sep = string({13,10,0});

