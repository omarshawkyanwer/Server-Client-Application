//
//  HTTP_Handler.cpp
//  HTTPServer
//
//  Created by mac on 11/4/18.
//  Copyright © 2018 mac. All rights reserved.
//

#include "HTTP_Handler.h"
#include <iostream>
#include <cstring>
#include "types.h"
#include "Request_Parser.h"
#include <arpa/inet.h>
#include "Response_Builder.h"
#include "File_System.h"
#include <unistd.h>
#include <condition_variable>
#include <thread>
//#include <boost/filesystem.hpp>

//using namespace boost::filesystem;
using namespace std;
const HTTP_Request NO_REQUEST {
    NOREQ,{},"",NOVER,""

};
const HTTP_Response NO_RESPONSE {
        NOVER,-1,{},""

};

const bool operator ==(const struct  HTTP_Request &req1,const struct HTTP_Request &req2) {
    return (req1.type == req2.type && req1.resource == req2.resource
    && req1.headers == req2.headers && req1.version == req2.version);
}
const bool operator !=(const struct  HTTP_Request &req1,const struct HTTP_Request &req2) {
    return  !(req1==req2);
}

const bool operator ==(const struct  HTTP_Response &req1,const struct HTTP_Response &req2) {
    return  (req1.version == req2.version && req1.headers == req2.headers
    && req1.status_code == req2.status_code);
}
const bool operator !=(const struct  HTTP_Response &req1,const struct HTTP_Response &req2) {
    return  !(req1 == req2);
}


HTTP_Response bad_request(HTTP_VERSION v){
    HTTP_Response res;
    res.status_code = 400;
    res.version = v;
    return  res;
}
inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}
HTTP_Handler::HTTP_Handler(int socketfd,struct sockaddr_in address){
    this->socketfd = socketfd;
    sockaddr_in add;
    this->client_Port = add.sin_port;
    char ip_add[20];
    this->client_IP_Address = inet_ntop(AF_INET,&add.sin_addr.s_addr,ip_add,20);
    parser = new Request_Parser();
    fsys = new File_System();
    keep_alive = true;
    terminate = false;
    last_req = time(0);
}
time_t HTTP_Handler::get_Last_Request(){
    return last_req;
}

mutex recv_to_handle;
mutex handle_to_send;
//mutex handle_lock;
condition_variable pipeline1;
condition_variable pipeline2;
void HTTP_Handler::shutdown(){

    terminate = true;
    keep_alive = false;


    close(socketfd);
    pipeline1.notify_all();
    pipeline2.notify_all();
}

bool HTTP_Handler::validate_Request(string req){
    return parser->validate(req);
}
int HTTP_Handler::parse_Request(string request,struct HTTP_Request * req){

    if(validate_Request(request))
        *req =  parser->parse(request);
    else {
        return -1;
    }
    return 0;
}
string HTTP_Handler::recv_data(int len){
    bool t ;
    int bcount=0;
    do {
        int count = recv(this->socketfd, buffer + bcount, sizeof(buffer), 0);
        bcount += count;
        t = bcount >=len;
        buffer[bcount] = 0;
        if(count == 0)
            shutdown();

    }while (!t && !terminate);
    char ret_val[1024];
    strncpy(ret_val,buffer,len);
    ret_val[len] = 0;
    strcpy(buffer,buffer+len);
    return string(ret_val);
}
string HTTP_Handler::recv_request(){
    bool t = false;
    int bcount=0;
    char local_buff[204800];
    do {
        int count;
//        if(!buffer_len) {
            count = recv(this->socketfd, buffer + bcount, sizeof(buffer), 0);
            bcount += count;
            buffer[bcount] = 0;
//        }
//        else
//            bcount =count = buffer_len;

//        int i;
//        for( i = 1 ;i < bcount && !t;i++) {
//            if(buffer[i] =='\n' && buffer[i-1]=='\n')
//                t = true;
//            if(i > 1 )
//                if(buffer[i] =='\n' && buffer[i-2]=='\n' && buffer[i-1] =='\r')
//                    t = true;
//
//        }
//        if(t) {
//            buffer_len = bcount;
//            int n = bcount;
//            if(i<bcount) {
//                n = i;
//                buffer_len -= i;
//            }
//            strncpy(local_buff,buffer,n);
//            local_buff[n]=0;
//            strncpy(buffer,buffer+n,(size_t)buffer_len);
//            cout<<"buffer len is now "<<buffer_len;
//        }



        t = ends_with(string(buffer),"\n\n")|| ends_with(string(buffer),"\r\n\r\n");
        if(count == 0) {
            cout<<"shutting down"<<endl;
            shutdown();
        }

    }while (!t && !terminate);
    strcpy(local_buff,buffer);
    cout<<"ely ga blzabt "<<local_buff<<endl;

    return  string(local_buff);

}
void HTTP_Handler::recv_parse(HTTP_Request &reqst,int &status) {
    HTTP_Request req;
    while (!terminate && keep_alive) {
        string request = recv_request();
        if(terminate || !keep_alive)
            break;
        if(parser->no_request(request)) {
            cout<<request <<"no request"<<endl;
            continue;
        }

        int stat1 = this->parse_Request(request, &req);
        if (stat1 >= 0 && req.type == POST) {
            if (req.headers.count("Content-Length")) {
                HTTP_Response _5ara;
                _5ara.status_code = 200;
                string resp = get_HTTP_Response(_5ara);
                send_Response(resp);
                last_req = time(0);
                req.body = recv_data(atoi(req.headers["Content-Length"].c_str()));

            } else
                stat1 = -1;

        }
        unique_lock<mutex> lck(recv_to_handle);
        if(reqst != NO_REQUEST)
            pipeline1.wait(lck,[&reqst,this]{ return reqst==NO_REQUEST || this->terminate;});
        reqst = req;
        status = stat1;
        lck.unlock();
        if(terminate || !keep_alive)
            break;
        pipeline1.notify_one();
    }
    cout<<"ending 1"<<endl;

}
void HTTP_Handler::handle_thread(HTTP_Request & request,HTTP_Response &resp,int &status){
    HTTP_Request req;
    HTTP_Response response;
    int stat;
    while (!terminate && keep_alive) {
        unique_lock<mutex> lck(recv_to_handle);
        if(request ==NO_REQUEST)
            pipeline1.wait(lck,[&request,this]{ return request!=NO_REQUEST || terminate;});
        req = request;
        stat = status;
        request = NO_REQUEST;
        lck.unlock();
        if(terminate || !keep_alive)
            break;
         if (stat < 0)
            response = bad_request(req.version);
        else
            response = this->handle_request(req);
        unique_lock<mutex> resp_lock(handle_to_send);
        //cout<<"pipelining"<<endl;
        if(resp != NO_RESPONSE)
            pipeline2.wait(resp_lock,[&resp,this]{ return resp == NO_RESPONSE || terminate;});

        resp = response;
       // cout<<"handled"<<endl;
        resp_lock.unlock();
        if(terminate || !keep_alive)
            break;
        pipeline2.notify_one();
    }
    cout<<"ending 2"<<endl;

}
void HTTP_Handler::respond_thread(HTTP_Response &resp) {
    HTTP_Response response;
    while (!terminate && keep_alive) {
        unique_lock<mutex> lck(handle_to_send);
        if(resp==NO_RESPONSE)
            pipeline2.wait(lck,[&resp,this]{ return resp != NO_RESPONSE || terminate ;});

        response = resp;
        resp = NO_RESPONSE;
        lck.unlock();
        if(terminate || !keep_alive)
            break;
        pipeline2.notify_one();
       // cout<<"sending"<<endl;
        string respose = this->get_HTTP_Response(response);
        //cout<<respose<<endl;
        this->send_Response(respose);
        cout<<"sentttt"<<endl;
        if(response.version == HTTP_1_0
        ||!response.headers.count("Connection") || response.headers["Connection"] != "keep-alive") {
            shutdown();
            if(response.headers.count("Connection"))
                cout<<response.headers["Connection"]<<" ";
            cout<<"terminate"<<endl;
        }
    }
    cout<<"ending 3"<<endl;

}
void HTTP_Handler::pool_requests(){

    struct HTTP_Request req = NO_REQUEST;
    struct HTTP_Response response = NO_RESPONSE;
    int status = 0;
    thread *a = new thread(&HTTP_Handler::recv_parse,this,std::ref(req),std::ref(status));
    thread *b = new thread(&HTTP_Handler::handle_thread,this,std::ref(req),ref(response),ref(status));
    thread *c = new thread(&HTTP_Handler::respond_thread,this,ref(response));

    a->join();
    b->join();
    c->join();


}

int HTTP_Handler::get_Resource(string resource_Name,char *res){
    strcpy(res,resource_Name.c_str());
    return 0;
}
string vtos(HTTP_VERSION vers) {
    return (vers == HTTP_1_0)?"HTTP/1.0":"HTTP/1.1";
}
string HTTP_Handler::get_HTTP_Response(HTTP_Response response){
    Response_Builder builder;
    builder.set_HTTP_Vesion(vtos(response.version));
    builder.set_Status(response.status_code == 200);
    builder.set_headers(&response.headers);
    builder.set_Content(response.resource);
    return  builder.build_Response();

}

HTTP_Response HTTP_Handler::handle_get(HTTP_Request *re){
    string res = re->resource;
    string file = fsys->read_File(res);
    cout<<"file read"<<endl;
    HTTP_Response response;
    response.headers["Server"] = "El-Shaba7";
    if(re->headers.count("Connection")) {
        response.headers["Connection"] = re->headers["Connection"];
    }
    if(file.size() > 0) {
        response.resource = file;
        response.status_code = 200;
        response.headers["Content-Length"] = to_string(file.size());

        time_t now = time(0);
        last_req = now;
        // convert now to string form
        char* dt = ctime(&now);
        string dtime = string(dt);
        dtime.pop_back();
        cout<<"data ends with new line " <<dtime<<"."<<endl;
        response.headers["Date"] = dtime;
    } else {
        response.resource = file;
        response.status_code = 404;
    }
    response.version = re->version;
    return  response;



}


HTTP_Response HTTP_Handler::handle_post(HTTP_Request *re) {
string res = re->resource;
cout<<"posting in "<<re->body<<" "<<re->resource<<endl;
fsys->write_File(re->body,re->resource);
HTTP_Response response;
response.headers["Server"] = "El-Shaba7";
if(re->headers.count("Connection")) {
        response.headers["Connection"] = re->headers["Connection"];
}
response.status_code = 200;
    return  response;


}
HTTP_Response HTTP_Handler::handle_request( HTTP_Request req) {
    switch (req.type) {
        case GET:
            return handle_get(&req);

        case POST:
            return handle_post(&req);
        default:
            return NO_RESPONSE;

    }
}

int  HTTP_Handler::send_Response(string response){
     return send(socketfd,response.c_str(),response.size(),0);
}

HTTP_Handler::~HTTP_Handler() {
    shutdown();
    delete (parser);
    delete (fsys);
}


