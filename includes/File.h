//
// Created by hope on 4/13/21.
//

#ifndef MYFS_FILE_H
#define MYFS_FILE_H

#include <string>
#include <vector>

class File {
public:
    File(const char *path, mode_t mode);
    std::string getName(){return name;}
    std::size_t getSize() { return data.size(); }
    time_t getAtime(){return atime;}
    time_t getMtime(){return mtime;}
    time_t getCtime(){return ctime;}
    uid_t  getUserId(){return uid;}
    gid_t  getGroupId(){return gid;}
    mode_t getMode(){return mode;}
    void setAtime();
    void setMtime();
    void setCtime();
    void setUserId(uid_t t);
    void setGroupId(gid_t t);
    void setMode(mode_t m);
    void setName(const char *p);

    int write(const char *buf, size_t size, off_t offset);
    int read(char *buf, size_t size, off_t offset);
    void truncate(off_t newSize);

private:
    std::string name; // path ohne '/'
    std::vector<char> data;
    time_t atime;
    time_t mtime;
    time_t ctime;
    uid_t uid;
    gid_t gid;
    mode_t mode;

};


#endif //MYFS_FILE_H
