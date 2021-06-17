#ifndef FAT_H
#define FAT_H

#include <cstdint>
#include <vector>

class Fat {
public:
    static int requiredBlocks(int containerBlocks);

    // Initialize empty FAT for the given container size
    void init(int containerBlocks);

    std::vector<uint32_t> getBlockList(uint32_t firstBlock) const;
    void appendBlock(int firstBlock, int newBlock);
    // TODO: truncate?

    std::vector<char> serialize();
    void deserialize(std::vector<char> bytes, int containerBlocks);

private:
    std::vector<int> table;

    int getLastBlock(int firstBlock) const;
};

#endif // FAT_H