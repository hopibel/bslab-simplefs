#include "Fat.h"
#include "myfs-structs.h"

// Calculate number of blocks required for the given container size.
int Fat::requiredBlocks(int containerBlocks) {
    const int entries_per_block = BLOCK_SIZE / sizeof(int);
    const int blocks_needed = (containerBlocks + entries_per_block - 1) / entries_per_block;

    return blocks_needed;
}

void Fat::init(int containerBlocks) {
    // TODO: implement me
}