#include "Dmap.h"
#include "myfs-structs.h"

// Calculate number of blocks required for the given container size.
int Dmap::requiredBlocks(int containerBlocks) {
    // ceiling of int division: https://stackoverflow.com/a/2745086/10272400
    // (x + y - 1) / y
    const int bytes_needed = (containerBlocks + 7) / 8;
    const int blocks_needed = (bytes_needed + BLOCK_SIZE - 1) / BLOCK_SIZE;

    return blocks_needed;
}

void Dmap::init(int containerBlocks) {
    // TODO(hopibel): implement me
}