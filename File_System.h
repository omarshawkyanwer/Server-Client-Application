//
//  File_System.h
//  Try
//
//  Created by mac on 11/17/18.
//  Copyright © 2018 mac. All rights reserved.
//

#ifndef File_System_h
#define File_System_h
#include <map>
#include <string>
#include <mutex>
using namespace std;
class File_System{
private:
    map<string,mutex *> file_Name_Lock;
    mutex map_Lock;
    void add_File_To_Map(string file_Name);
public:
    File_System();
    string read_File(string file_Name);
    void write_File(string new_File_Content,string new_File_Path);
};


#endif /* File_System_h */
