#ifndef DMAP_H
#define DMAP_H

#include <cstdint>
#include <vector>

class Dmap {
public:
    static int requiredBlocks(int containerBlocks);

    // Initialize empty DMAP for the given container size
    void init(int containerBlocks, int metadataBlocks);

    uint32_t countFreeBlocks() const;
    int findFreeBlock();
    std::vector<uint32_t> findNFreeBlocks(uint32_t n);
    bool isFree(uint32_t block) const;
    void markUsed(uint32_t block);
    void markFree(uint32_t block);

    std::vector<char> serialize();
    void deserialize(std::vector<char> bytes, uint32_t containerBlocks, uint32_t dataStart);

private:
    std::vector<bool> flags;
    int num_flags;
    uint32_t dataStart;
    uint32_t nextBlock = 0;
};

#endif // DMAP_H