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
    void deserialize(char* buffer);

    std::string getName() const { return name; }
    off_t getSize() const;
    uint32_t getFirstBlock() const {return firstBlock;}
    struct stat getStat() const { return structstat; }

    void setName(const char* newName);
    void setName(const std::string newName);
    void setFirstBlock(uint32_t f);

    void setAtime();
    void setMtime();
    void setCtime();
    void setUserId(uid_t t);
    void setGroupId(gid_t t);
    void setMode(mode_t m);
    void setSize(off_t s);

private:
    std::string name;
    uint32_t firstBlock;
    struct stat structstat;
};

#endif //MYFS_ONDISKFILE_H
