#include "Dmap.h"
#include "myfs-structs.h"

#include <errno.h>

// Calculate number of blocks required for the given container size.
int Dmap::requiredBlocks(int containerBlocks) {
    // ceiling of int division: https://stackoverflow.com/a/2745086/10272400
    // (x + y - 1) / y
    const int bytes_needed = (containerBlocks + 7) / 8;
    const int blocks_needed = (bytes_needed + BLOCK_SIZE - 1) / BLOCK_SIZE;

    return blocks_needed;
}

// index counts bits from least to most significant
// metadata is number of blocks reserved for filesystem structures
void Dmap::init(int containerBlocks, int metadata) {
    // save containerBlocks because DMAP size isn't divisible by 8
    // and needs to be padded to full byte at the end
    num_flags = containerBlocks;

    // mark metadata blocks as used
    const int full_ints = metadata / 32;
    const int rem = metadata % 32;
    flags.resize(full_ints, 1);
    if (rem > 0) {
        flags.push_back(0xFFFFFFFF ^ (0xFFFFFFFF << rem));
    }

    // mark rest as free
    const int needed = (containerBlocks + 31) / 32;
    flags.resize(needed, 0);
}

// Returns block id of a free block or -ENOSPC if there are none
int Dmap::findFreeBlock() {
    // TODO: cache locations of free blocks (contiguous regions)
    for (std::size_t i = 0; i < flags.size(); ++i) {
        // check 32 bits at once
        auto flag = flags[i];
        if (flag < 0xFFFFFFFF) {
            // find offset within the char
            for (int j = 0; j < 32; ++j) {
                if ((flag & (1 << j)) == 0) {
                    // check if block id is valid (and not padding)
                    int block = i * 32 + j;
                    if (block < num_flags) {
                        return i * 32 + j;
                    } else {
                        return -ENOSPC;
                    }
                }
            }
        }
    }
    // no free block found
    return -ENOSPC;
}

bool Dmap::isFree(int block) const {
    int index = block / 32;
    int bit_offset = block % 32;
    return (flags[index] & (1 << bit_offset)) == 0;
}

// set bit
void Dmap::markUsed(int block) {
    int index = block / 32;
    int bit_offset = block % 32;
    unsigned int mask = 1 << bit_offset;
    flags[index] |= mask;
}

// unset bit
void Dmap::markFree(int block) {
    int index = block / 32;
    int bit_offset = block % 32;
    unsigned int mask = ~(1 << bit_offset);
    flags[index] &= mask;
}