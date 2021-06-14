#include "Root.h"
#include "myfs-structs.h"

#include <vector>

// Calculate number of blocks required for the given container size.
int Root::requiredBlocks(int containerBlocks) {
    // hardcoded to 64 files
    return NUM_DIR_ENTRIES;
}

void Root::init(int containerBlocks) {
    // TODO: implement me
}

std::vector<unsigned char> Root::serialize() {
    // TODO: implement
    return std::vector<unsigned char> {};
}