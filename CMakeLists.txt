cmake_minimum_required(VERSION 3.5)
project(Server_Client_Application)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
include_directories(.)

add_executable(Server_Client_Application
        HTTP_Handler.cpp
        HTTP_Handler.h
        HTTP_Server.cpp
        HTTP_Server.h
        main.cpp
        Request_Parser.cpp
        Request_Parser.h
        Request_Parser.hpp
        File_System.cpp
        File_System.h
        Response_Builder.cpp
        Response_Builder.h
        types.h)
