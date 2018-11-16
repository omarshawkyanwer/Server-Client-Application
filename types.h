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
using namespace std;

typedef in_port_t Port;
typedef string IP_Address;
typedef uint32_t ip_t;

enum HTTP_Request_type {
GET,
POST,
HEAD,
DELETE,
PUT
};
enum HTTP_VERSION {
HTTP_1_0,
HTTP_1_1
};

#endif /* types_h */
