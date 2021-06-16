#include "Fat.h"
#include "myfs-structs.h"

#include <cstring>
#include <iterator>
#include <vector>

// Calculate number of blocks required for the given container size.
int Fat::requiredBlocks(int containerBlocks) {
    const int entries_per_block = BLOCK_SIZE / sizeof(int);
    const int blocks_needed = (containerBlocks + entries_per_block - 1) / entries_per_block;

    return blocks_needed;
}

void Fat::init(int containerBlocks) {
    table.resize(containerBlocks, 0);
}

// Return vector of blockIDs for the file located at firstBlock.
std::vector<int> Fat::getBlockList(int firstBlock) const {
    std::vector<int> blocks{firstBlock};
    for (int block = firstBlock; table[block] != END_OF_CLUSTER; block = table[block]) {
        blocks.push_back(block);
    }

    return blocks;
}

// Append newBlock to file's chain.
void Fat::appendBlock(int firstBlock, int newBlock) {
    const int lastBlock = getLastBlock(firstBlock);
    table[lastBlock] = newBlock;
    table[newBlock] = END_OF_CLUSTER;
}

std::vector<char> Fat::serialize() {
    std::vector<char> bytes;
    bytes.resize(table.size() * sizeof(uint32_t), 0);
    memcpy(bytes.data(), table.data(), bytes.size());

    return bytes;
}

void Fat::deserialize(std::vector<char> bytes, int containerBlocks) {
    if (bytes.size() / sizeof(int) != (std::size_t) containerBlocks) {
        throw std::invalid_argument(
            "expected " +
            std::to_string(containerBlocks * sizeof(int)) +
            " bytes. received " +
            std::to_string(bytes.size())
        );
    }

    // allocate memory
    table.resize(containerBlocks, 0);
    // copy bytes into table
    memcpy(table.data(), bytes.data(), bytes.size());
}

int Fat::getLastBlock(int firstBlock) const {
    int block = firstBlock;
    while (table[block] != END_OF_CLUSTER) {
        block = table[block];
    }

    return block;
}