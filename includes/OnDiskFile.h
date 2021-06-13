//
// Created by user on 13.06.21.
//

#ifndef MYFS_ONDISKFILE_H
#define MYFS_ONDISKFILE_H
#include <sys/stat.h>
#include <string>
#include "blockdevice.h"
#include "myfs-structs.h"

class OnDiskFile {
public:
    OnDiskFile();
    char* serialize(char* buffer);
    char* deserialize(char* buffer);
    ~OnDiskFile(){
        //delete(statbuf);
    }
private:
    std::string path;
    struct stat statbuf;
    uint32_t first_block;





};

#endif //MYFS_ONDISKFILE_H
