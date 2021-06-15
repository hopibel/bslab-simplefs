#ifndef FAT_H
#define FAT_H

#include <vector>

class Fat {
public:
    static int requiredBlocks(int containerBlocks);

    // Initialize empty FAT for the given container size
    void init(int containerBlocks);

    std::vector<int> getBlockList(int firstBlock) const;
    void appendBlock(int firstBlock, int newBlock);
    // TODO: truncate?

    std::vector<char> serialize();

private:
    std::vector<int> table;

    int getLastBlock(int firstBlock) const;
};

#endif // FAT_H