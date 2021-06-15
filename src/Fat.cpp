#include "Fat.h"
#include "myfs-structs.h"

#include <vector>

// Calculate number of blocks required for the given container size.
int Fat::requiredBlocks(int containerBlocks) {
    const int entries_per_block = BLOCK_SIZE / sizeof(int);
    const int blocks_needed = (containerBlocks + entries_per_block - 1) / entries_per_block;

    return blocks_needed;
}

void Fat::init(int containerBlocks) {
    // TODO: implement me
}

std::vector<char> Fat::serialize() {
    // TODO: implement
    return std::vector<char> {};
}