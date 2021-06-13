//
// Created by user on 13.06.21.
//
#include "OnDiskFile.h"
#include <string.h>

OnDiskFile::OnDiskFile() {

    this->path = std::string(NAME_LENGTH+1,'0');
    this->first_block =0;
}
char* OnDiskFile::serialize(char *buffer) {

    memcpy(buffer,this, sizeof(*this));

}
off_t OnDiskFile::getSize() {
    return this->structstat.st_size;
}
void OnDiskFile::setAtime(){
    this->structstat.st_atim.tv_sec = time(nullptr);
}
void OnDiskFile::setMtime(){
    this->structstat.st_mtim.tv_sec = time(nullptr);
}
void OnDiskFile::setCtime(){
    this->structstat.st_ctim.tv_sec = time(nullptr);
}
void OnDiskFile::setUserId(uid_t t){
    this->structstat.st_uid =t;
}
void OnDiskFile::setGroupId(gid_t t) {
    this->structstat.st_gid =t;
}
void OnDiskFile::setMode(mode_t m) {
    this->structstat.st_mode = m;
}
void OnDiskFile::setFirst_block(uint32_t f) {
    this->first_block = f;
}
void OnDiskFile::setSize(off_t s) {
    this->structstat.st_size = s;
}