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

    std::size_t getSize() { return data.size(); }

private:
    std::string name; // path
    std::vector<char> data;
    time_t atime;
    time_t mtime;
    uid_t uid;
    gid_t gid;
    mode_t mode;
};


#endif //MYFS_FILE_H
