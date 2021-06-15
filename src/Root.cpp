#include "Root.h"
#include "myfs-structs.h"

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
    // TODO: implement
    return std::vector<char> {};
}

// TODO: implement
void Root::deserialize(std::vector<char> bytes, int containerBlocks) {

}