//
//  Request_Parser.cpp
//  HTTPServer
//
//  Created by mac on 11/4/18.
//  Copyright © 2018 mac. All rights reserved.
//

#include <stdio.h>
#include "Request_Parser.h"
#include <vector>
#include <string>
#include <sstream>
using namespace std;

//default constructor
Request_Parser::Request_Parser(){}

vector<string> Request_Parser::parse_Request(string req){
    vector<string> result;
    string first_Line;
    istringstream request(req);
    while (getline(request, first_Line)) {
        istringstream first_Line_Stream(first_Line);
        string first_Line_field;
        while (getline(first_Line_Stream,first_Line_field,' ')) {
            result.push_back(first_Line_field);
        }
        break;
    }
    return result;
}
