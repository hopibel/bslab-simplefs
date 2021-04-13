//
// Created by hope on 4/13/21.
//

#include "File.h"

#include <unistd.h>
#include <errno.h>

File::File(const char *path, mode_t mode) {
    name = path;
    atime = time(nullptr);
    mtime = time(nullptr);
    uid = getuid();
    gid = getgid();
    this->mode = mode;
}
