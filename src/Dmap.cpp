#include "Dmap.h"
#include "blockdevice.h"
#include "myfs-structs.h"

#include <algorithm>
#include <cstdint>
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
    dataStart = metadataBlocks;

    // mark metadata blocks as used
    flags.resize(metadataBlocks, true);
    flags.resize(num_flags, false);

    // cache index of next free block
    nextBlock = dataStart % containerBlocks;
}

uint32_t Dmap::countFreeBlocks() const {
    uint32_t n = 0;
    for (bool b : flags) {
        n += static_cast<int>(!b);
    }
    return n;
}

// Returns block id of a free block or -ENOSPC if there are none
int Dmap::findFreeBlock() {
    // nextBlock contains the (approximate) next free block.
    // avoids scanning from the beginning every single time
    int ret = 0;
    auto firstFree = std::find(flags.begin() + nextBlock, flags.end(), false);
    if (firstFree == flags.end()) {
        // no free block found. wrap around to the beginning
        firstFree = std::find(flags.begin(), flags.begin() + nextBlock, false);
        if (firstFree == flags.begin() + nextBlock) {
            // still no free block found. out of space!
            return -ENOSPC;
        } else {
            ret = firstFree - flags.begin();
        }
    } else {
        ret = firstFree - flags.begin();
    }

    nextBlock = std::max((ret + 1) % flags.size(), static_cast<std::size_t>(dataStart));

    return ret;
}

std::vector<uint32_t> Dmap::findNFreeBlocks(uint32_t n) {
    // nextBlock contains the (approximate) next free block.
    // avoids scanning from the beginning every single time
    std::vector<uint32_t> freeBlocks;
    auto nextBlockOld = nextBlock;

    // scan from nextBlock to end
    uint32_t block;
    for (block = nextBlockOld; block < flags.size() && freeBlocks.size() < n; ++block) {
        if (isFree(block)) {
            freeBlocks.push_back(block);
        }
    }

    // update nextBlock
    if (freeBlocks.size() == n) {
        nextBlock = std::max(block % flags.size(), static_cast<std::size_t>(dataStart));
    }

    // scan from beginning to old nextBlock
    for (block = 0; block < nextBlockOld && freeBlocks.size() < n; ++block) {
        if (isFree(block)) {
            freeBlocks.push_back(block);
        }
    }
    if (freeBlocks.size() < n) {
        throw std::runtime_error("Couldn't find " + std::to_string(n) + " free blocks");
    }

    // update nextBlock
    if (block > 0) {
        nextBlock = std::max(block % flags.size(), static_cast<std::size_t>(dataStart));
    }

    return freeBlocks;
}

bool Dmap::isFree(uint32_t block) const {
    return flags[block] == false;
}

// set bit
void Dmap::markUsed(uint32_t block) {
    flags[block] = true;
}

// unset bit
void Dmap::markFree(uint32_t block) {
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

void Dmap::deserialize(std::vector<char> bytes, uint32_t containerBlocks, uint32_t dataStart) {
    if (bytes.size() / BLOCK_SIZE != (std::size_t) Dmap::requiredBlocks(containerBlocks)) {
        throw std::invalid_argument(
            "expected " +
            std::to_string(Dmap::requiredBlocks(containerBlocks) * BLOCK_SIZE) +
            " bytes. received " +
            std::to_string(bytes.size())
        );
    }

    this->dataStart = dataStart;

    // read containerBlocks bits
    bool nextBlockCached = false;
    for (std::size_t i = 0; i < bytes.size() && i * 8 < (std::size_t) containerBlocks; ++i) {
        // unpack bits into 8 flags starting from lowest
        for (int bit = 0; bit < 8; ++bit) {
            flags.push_back(bytes[i] & (1 << bit));
        }
        // cache next free block id
        if (!nextBlockCached && bytes[i] != static_cast<char>(0xFF)) {
            nextBlock = flags.size() - 8;
            nextBlockCached = true;
        }
    }

    // discard extra bits, if any
    flags.resize(containerBlocks);
}