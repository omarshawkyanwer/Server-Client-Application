//
//  File_System.cpp
//  Try
//
//  Created by mac on 11/17/18.
//  Copyright © 2018 mac. All rights reserved.
//

#include <stdio.h>
#include "File_System.h"
#include <mutex>
#include <map>
#include <fstream>
#include <string>
#include <unistd.h>

using namespace std;

File_System::File_System(){}

void File_System::add_File_To_Map(string file_Path_With_Name){
    lock_guard<mutex> lock_Map(map_Lock);
    if(file_Name_Lock.find(file_Path_With_Name) == file_Name_Lock.end()){
        mutex * new_File_Mutex = new mutex();
        file_Name_Lock.emplace(make_pair(file_Path_With_Name, new_File_Mutex));
    }
}

void File_System::write_File(string new_File_Content, string file_Path_With_Name){
    add_File_To_Map(file_Path_With_Name);
    lock_guard<mutex> lock_File(*file_Name_Lock[file_Path_With_Name]);
    ofstream file;
    file.open (file_Path_With_Name);
    for_each(new_File_Content.begin(), new_File_Content.end(), [&file](char& c){
        file<<c;
    });
    file.close();
}

string File_System::read_File(string file_Name){
    //if file exists
    if(access( file_Name.c_str(), F_OK ) != -1){
        add_File_To_Map(file_Name);
        lock_guard<mutex> lock_File(*file_Name_Lock[file_Name]);
        fstream file_To_Read(file_Name);
        file_To_Read.seekg (0, file_To_Read.end);
        int length = file_To_Read.tellg();
        file_To_Read.seekg (0, file_To_Read.beg);
        char * buffer = new char [length];
        file_To_Read.read(buffer,length);
        string content;
        for(int i = 0 ; i < length ; ++i)
            content.push_back(buffer[i]);
        delete [] buffer;
        file_To_Read.close();
        return content;
    }else{
        return string();
    }
}


