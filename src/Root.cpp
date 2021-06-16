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
    std::vector<char> bytes(RootDir.size() * BLOCK_SIZE, 0);

    // write one file per block
    char* buf = bytes.data();
    for (OnDiskFile& file : RootDir) {
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
    for (auto& file : RootDir) {
        file.deserialize(buf);
        buf += BLOCK_SIZE;
    }
}