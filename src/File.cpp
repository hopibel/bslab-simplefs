//
// Created by hope on 4/13/21.
//

#include "File.h"

#include <algorithm>
#include <unistd.h>
#include <errno.h>

File::File(const char *path, mode_t mod){
    name = path + 1;
    atime = time(nullptr);
    mtime = time(nullptr);
    ctime = time(nullptr);
    uid = getuid();
    gid = getgid();
    this->mode = mod;
}
void File::setAtime(){
    this->atime=time(nullptr);
}
void File::setMtime(){
    this->mtime=time(nullptr);
}
void File::setCtime(){
    this->ctime=time(nullptr);
}
void File::setUserId(uid_t t){
    this->uid= t;
}
void File::setGroupId(gid_t t){
    this->gid= t;
}
void File::setMode(mode_t m){
    this->mode = m;
}
void File::setName(const char* p){
    std::string filename = p;
    if(filename[0]=='/'){
        filename.substr(1, std::string::npos);
    }
    this->name = filename;
}

int File::write(const char *buf, size_t size, off_t offset) {
    // update mtime
    setMtime();
    // update ctime
    setCtime();

    // resize if writing past end of file
    if (offset + size > getSize()) {
        data.resize(offset + size, 0); // fill empty with null bytes
    }
    std::copy_n(buf, size, data.begin() + offset);
    return size;
}
int File::read( char *buf, size_t size, off_t offset) {
    // update Accesstime
    setAtime();

    if(offset > getSize()){
    // Offset größer als File
        return 0;
    }
    if(offset + size <= getSize() ){

        std::copy(data.begin() + offset, data.begin() + offset + size, buf);
        return size;
    }
    else{
        std::copy(data.begin() + offset,data.end(),buf);
        return (getSize() - offset);
    }
}
void File::truncate(off_t newSize) {
    // update mtime
    setMtime();
    // update ctime
    setCtime();
    data.resize(newSize,0);
}