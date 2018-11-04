//
//  Request_Parser.h
//  HTTPServer
//
//  Created by mac on 11/4/18.
//  Copyright © 2018 mac. All rights reserved.
//

#ifndef Request_Parser_h
#define Request_Parser_h
#include <vector>
#include <string>

using namespace std;



class Request_Parser{
public:
    Request_parser();
    vector<string> parser_Request(string req);
};

#endif /* Request_Parser_h */
