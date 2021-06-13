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