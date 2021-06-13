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
    std::string getPath(){return path;}
    off_t getSize();
    uint32_t getFirst_block(){return first_block;}
    void setAtime();
    void setMtime();
    void setCtime();
    void setUserId(uid_t t);
    void setGroupId(gid_t t);
    void setMode(mode_t m);
    void setFirst_block(uint32_t f);
    void setSize(off_t s);

    ~OnDiskFile(){
        //delete(structstat);
    }
private:
    std::string path;
    struct stat structstat;
    uint32_t first_block;





};

#endif //MYFS_ONDISKFILE_H
