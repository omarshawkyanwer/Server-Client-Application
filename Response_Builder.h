//
//  Response_Builder.h
//  HTTPServer
//
//  Created by mac on 11/7/18.
//  Copyright © 2018 mac. All rights reserved.
//

#ifndef Response_Builder_h
#define Response_Builder_h
#include <string>
using namespace std;

class Response_Builder{
private:
    string status;
    string HTTP_Version;
    string content_Type;
    string content_Length;
    string respone_Body;
public:
    Response_Builder();
    void set_Status(bool state);
    void set_HTTP_Vesion(string version);
    void set_Content(string && content);
    void set_Content_Type(string type);
    string build_Response();
};

#endif /* Response_Builder_h */
