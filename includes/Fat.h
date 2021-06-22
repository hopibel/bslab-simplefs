#ifndef FAT_H
#define FAT_H

#include <cstdint>
#include <vector>

class Fat {
public:
    static int requiredBlocks(int containerBlocks);

    // Initialize empty FAT for the given container size
    void init(uint32_t containerBlocks);

    std::vector<uint32_t> getBlockList(uint32_t firstBlock) const;
    void allocateBlock(uint32_t firstBlock);
    void appendBlock(uint32_t firstBlock, uint32_t newBlock);
    // TODO: truncate?
    void truncate(uint32_t block);
    std::vector<char> serialize();
    void deserialize(std::vector<char> bytes, uint32_t containerBlocks);

private:
    std::vector<uint32_t> table;

public:    uint32_t getLastBlock(uint32_t firstBlock) const;




};

#endif // FAT_H