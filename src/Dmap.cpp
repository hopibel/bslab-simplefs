#include "Dmap.h"
#include "blockdevice.h"
#include "myfs-structs.h"

#include <algorithm>
#include <errno.h>

// Calculate number of blocks required for the given container size.
int Dmap::requiredBlocks(int containerBlocks) {
    // ceiling of int division: https://stackoverflow.com/a/2745086/10272400
    // (x + y - 1) / y
    const int bytes_needed = (containerBlocks + 7) / 8;
    const int blocks_needed = (bytes_needed + BLOCK_SIZE - 1) / BLOCK_SIZE;

    return blocks_needed;
}

// Initialize an empty DMAP in memory
// index counts bits from least to most significant
// metadata is number of blocks reserved for filesystem structures
void Dmap::init(int containerBlocks, int metadataBlocks) {
    // save containerBlocks in case DMAP size isn't divisible by 8
    // and needs padding
    num_flags = containerBlocks;

    // mark metadata blocks as used
    flags.resize(metadataBlocks, true);
    flags.resize(num_flags, false);
}

// Returns block id of a free block or -ENOSPC if there are none
int Dmap::findFreeBlock() {
    // TODO: cache locations of free blocks (contiguous regions)
    auto firstFree = std::find(flags.begin(), flags.end(), false);
    if (firstFree == flags.end()) {
        // no free block found
        return -ENOSPC;
    } else {
        return firstFree - flags.begin();
    }
}

bool Dmap::isFree(int block) const {
    return flags[block] == false;
}

// set bit
void Dmap::markUsed(int block) {
    flags[block] = true;
}

// unset bit
void Dmap::markFree(int block) {
    flags[block] = false;
}

// bits indexed little endian order
std::vector<unsigned char> Dmap::serialize() {
    std::vector<unsigned char> bytes;

    unsigned char byte = 0;
    for (std::size_t i = 0; i < flags.size(); ++i) {
        byte |= flags[i] << (i % 8);
        if (i % 8 == 7) {
            bytes.push_back(byte);
            byte = 0;
        }
    }

    return bytes;
}