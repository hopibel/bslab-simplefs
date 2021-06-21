//
// Created by user on 13.06.21.
//
#include "OnDiskFile.h"
#include "myfs-structs.h"

#include <algorithm>
#include <unistd.h>
#include <string.h>

OnDiskFile::OnDiskFile(std::string name, mode_t mode) {
    this->name = name;
    sstat.st_atim.tv_sec = time(nullptr);
    sstat.st_mtim.tv_sec = time(nullptr);
    sstat.st_ctim.tv_sec = time(nullptr);
    sstat.st_uid = getuid();
    sstat.st_gid = getgid();
    sstat.st_mode = mode;

    sstat.st_size = 0;
    sstat.st_blocks = 0;
    sstat.st_nlink = 1;
}

// On-disk format:
// (index, description)
// 0: Name length
// 1-255: up to 255 char name without null-terminator. Padded to 255 bytes with 0
// 256-259: int blockId of first data block (aka FAT index)
// 260-403: stat struct with file metadata
// 404-511: padding
char* OnDiskFile::serialize(char *buffer) {
    // geht nicht. std::string enthält ein pointer
    // memcpy(buffer, this, sizeof(*this));

    char* bPtr = buffer;

    // write name length. truncate to 255 just in case
    uint8_t nameLen = name.length() > NAME_LENGTH ? 255 : name.length();
    bPtr[0] = nameLen;
    bPtr += sizeof(nameLen);

    // write name
    memcpy(bPtr, name.data(), nameLen);
    bPtr += nameLen;

    // write firstBlock
    memcpy(bPtr, &firstBlock, sizeof(firstBlock));
    bPtr += sizeof(firstBlock);

    // write structstat
    memcpy(bPtr, &sstat, sizeof(sstat));

    return buffer;
}

void OnDiskFile::deserialize(char *buffer) {
    // read name length
    uint8_t nameLen = (uint8_t) buffer[0];
    buffer += sizeof(nameLen);

    // read name
    name = std::string(buffer, nameLen);
    buffer += nameLen;

    // read firstBlock
    firstBlock = *((uint32_t*) buffer);
    buffer += sizeof(firstBlock);

    // read structstat
    memcpy(&sstat, buffer, sizeof(sstat));
}

void OnDiskFile::setName(const char* p) {
    std::string filename = p;
    if (filename[0]=='/') {
        filename.substr(1, std::string::npos);
    }
    this->name = filename;
}
void OnDiskFile::setName(const std::string newName) {
    name = newName;
}

off_t OnDiskFile::getSize() const {
    return this->sstat.st_size;
}
void OnDiskFile::setAtime(){
    this->sstat.st_atim.tv_sec = time(nullptr);
}
void OnDiskFile::setMtime(){
    this->sstat.st_mtim.tv_sec = time(nullptr);
}
void OnDiskFile::setCtime(){
    this->sstat.st_ctim.tv_sec = time(nullptr);
}
void OnDiskFile::setUserId(uid_t t){
    this->sstat.st_uid =t;
}
void OnDiskFile::setGroupId(gid_t t) {
    this->sstat.st_gid =t;
}
void OnDiskFile::setMode(mode_t m) {
    this->sstat.st_mode = m;
}
void OnDiskFile::setFirstBlock(uint32_t f) {
    this->firstBlock = f;
}
void OnDiskFile::setSize(off_t s) {
    this->sstat.st_size = s;
}