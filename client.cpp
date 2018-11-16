#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <vector>
#include <string>
#include <iostream>
#include<fstream>


std::vector<std::string> read_file();

int main(int argc , char * argv []){

    // check number of arguments :
    if (argc != 3){
        //error
    }
    char * server_ip = argv[1];
    int port_number = atoi(argv[2]);
    int socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if (socket_fd<0){
        printf("Error creating the socket");
        // print error message and make a function for error print :'D
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number);
    
    if (connect(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){

        //connection failed
    }

    // send/recieve : ezay h3rf howa 3ayez eih ? :'D    

    std::vector<std::string> requests = read_file();

    std::cout << requests.size() << std::endl;

    for (int i =0 ; i<requests.size();i++){
        std::cout<<requests[i] <<std::endl;
    }
        
}

std::vector<std::string> read_file()
{
    // Open the File
    std::ifstream in("requests.txt");
    //file = fopen(path,'r');
    std::string line;
    std::vector<std::string> requests; // Empty on creation
    while (std::getline(in,line)){
        if (line.size()>0){
            requests.push_back(line);
        }
    }
    return requests;
}


void get(std::string){

}

void post(std::string){

}

