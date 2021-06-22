#include "Fat.h"
#include "myfs-structs.h"

#include <cstdint>
#include <cstring>
#include <iterator>
#include <vector>

// Calculate number of blocks required for the given container size.
int Fat::requiredBlocks(int containerBlocks) {
    const int entries_per_block = BLOCK_SIZE / sizeof(int);
    const int blocks_needed = (containerBlocks + entries_per_block - 1) / entries_per_block;

    return blocks_needed;
}

void Fat::init(uint32_t containerBlocks) {
    table.resize(containerBlocks, FREE);
}

// Return vector of blockIDs for the file located at firstBlock.
std::vector<uint32_t> Fat::getBlockList(uint32_t firstBlock) const {
    std::vector<uint32_t> blocks;
    for (auto block = firstBlock; block != END_OF_CLUSTER; block = table[block]) {
        blocks.push_back(block);
    }
    return blocks;
}

// Allocate the first block of a file
void Fat::allocateBlock(uint32_t firstBlock) {
    table[firstBlock] = END_OF_CLUSTER;
}

// Append newBlock to file's chain.
void Fat::appendBlock(uint32_t firstBlock, uint32_t newBlock) {
    const uint32_t lastBlock = getLastBlock(firstBlock);
    table[lastBlock] = newBlock;
    table[newBlock] = END_OF_CLUSTER;
}

std::vector<char> Fat::serialize() {
    std::vector<char> bytes;
    bytes.resize(table.size() * sizeof(uint32_t), 0);
    memcpy(bytes.data(), table.data(), bytes.size());

    return bytes;
}

void Fat::deserialize(std::vector<char> bytes, uint32_t containerBlocks) {
    if (bytes.size() / sizeof(uint32_t) != (std::size_t) containerBlocks) {
        throw std::invalid_argument(
            "expected " +
            std::to_string(containerBlocks * sizeof(uint32_t)) +
            " bytes. received " +
            std::to_string(bytes.size())
        );
    }

    // allocate memory
    table.resize(containerBlocks, 0);
    // copy bytes into table
    memcpy(table.data(), bytes.data(), bytes.size());
}

uint32_t Fat::getLastBlock(uint32_t firstBlock) const {
    auto block = firstBlock;
    while (table[block] != END_OF_CLUSTER) {
        block = table[block];
    }

    return block;
}// Truncate a chain to the given block
void Fat::truncate(uint32_t block) {
    table[block] = END_OF_CLUSTER;
}