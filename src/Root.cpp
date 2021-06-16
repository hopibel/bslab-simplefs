#include "Root.h"
#include "OnDiskFile.h"
#include "myfs-structs.h"

#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

// Calculate number of blocks required for the given container size.
int Root::requiredBlocks(int containerBlocks) {
    // hardcoded to 64 files
    return NUM_DIR_ENTRIES;
}

void Root::init(int containerBlocks) {
    // TODO: implement
}

std::vector<char> Root::serialize() {
    // allocate buffer NUM_DIR_ENTRIES * BLOCK_SIZE bytes
    std::vector<char> bytes(files.size() * BLOCK_SIZE, 0);

    // write one file per block
    char* buf = bytes.data();
    for (OnDiskFile& file : files) {
        file.serialize(buf);
        buf += BLOCK_SIZE;
    }

    return bytes;
}

void Root::deserialize(std::vector<char> bytes, int containerBlocks) {
    if (bytes.size() != NUM_DIR_ENTRIES * BLOCK_SIZE) {
        throw std::invalid_argument("expected " +
            std::to_string(NUM_DIR_ENTRIES * BLOCK_SIZE) + 
            "bytes. received " + 
            std::to_string(bytes.size())
        );
    }

    char* buf = bytes.data();
    for (auto& file : files) {
        file.deserialize(buf);
        buf += BLOCK_SIZE;
    }
}

bool Root::has(std::string path) const {
    for (auto& file : files) {
        if (file.getName() == path) {
            return true;
        }
    }
    return false;
}

OnDiskFile& Root::getFile(std::string path) {
    for (auto& file : files) {
        if (file.getName() == path) {
            return file;
        }
    }
    throw std::out_of_range("Couldn't find " + path + " in directory");
}

std::vector<std::string> Root::getFileList() const {
    std::vector<std::string> fileList;
    for (auto& file : files) {
        fileList.push_back(file.getName());
    }

    return fileList;
}