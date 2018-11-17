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
#include "types.h"
#include <string>
#include "HTTP_Handler.h"
using namespace std;



class Request_Parser{
public:
    static string sep;
    Request_Parser();
    struct HTTP_Request parse(string req);
    bool validate(string req);
};

#endif /* Request_Parser_h */
