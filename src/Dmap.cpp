#include "Dmap.h"
#include "blockdevice.h"
#include "myfs-structs.h"

#include <algorithm>
#include <errno.h>
#include <stdexcept>
#include <string>

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
int Dmap::findFreeBlock() const {
    // TODO: cache locations of free blocks (contiguous regions)
    auto firstFree = std::find(flags.begin(), flags.end(), false);
    if (firstFree == flags.end()) {
        // no free block found
        return -ENOSPC;
    } else {
        return firstFree - flags.begin();
    }
}

std::vector<int> Dmap::findNFreeBlocks(int n) const {
    std::vector<int> blocks;
    for (int i = 0; i < n; ++i) {
        int block = findFreeBlock();
        if (block < 0) {
            throw std::runtime_error("Couldn't find " + std::to_string(n) + " free blocks");
        } else {
            blocks.push_back(block);
        }
    }

    return blocks;
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
std::vector<char> Dmap::serialize() {
    std::vector<char> bytes;

    char byte = 0;
    for (std::size_t i = 0; i < flags.size(); ++i) {
        byte |= flags[i] << (i % 8);
        if (i % 8 == 7) {
            bytes.push_back(byte);
            byte = 0;
        }
    }

    return bytes;
}

void Dmap::deserialize(std::vector<char> bytes, int containerBlocks) {
    if (bytes.size() / BLOCK_SIZE != (std::size_t) Dmap::requiredBlocks(containerBlocks)) {
        throw std::invalid_argument(
            "expected " +
            std::to_string(Dmap::requiredBlocks(containerBlocks) * BLOCK_SIZE) +
            " bytes. received " +
            std::to_string(bytes.size())
        );
    }

    // read containerBlocks bits
    for (std::size_t i = 0; i < bytes.size() && i * 8 < (std::size_t) containerBlocks; ++i) {
        // unpack bits into 8 flags starting from lowest
        for (int bit = 0; bit < 8; ++bit) {
            flags.push_back(bytes[i] & (1 << bit));
        }
    }

    // discard extra bits, if any
    flags.resize(containerBlocks);
}