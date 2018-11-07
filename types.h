//
//  types.h
//  HTTPServer
//
//  Created by mac on 11/4/18.
//  Copyright © 2018 mac. All rights reserved.
//

#ifndef types_h
#define types_h
#include <string>
#include <netinet/in.h>


#define MAXCONN 50
#define LOCALHOST "127.0.0.1"
#define OK "200 OK"
#define NOTFOUND "404 Not Found"
#define HTTP_ONE "HTTP/1.0"
#define HTTP_ONE_ONE "HTTP/1.1"

using namespace std;

typedef in_port_t Port;
typedef string IP_Address;




#endif /* types_h */
