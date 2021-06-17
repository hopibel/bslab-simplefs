#ifndef DMAP_H
#define DMAP_H

#include <cstdint>
#include <vector>

class Dmap {
public:
    static int requiredBlocks(int containerBlocks);

    // Initialize empty DMAP for the given container size
    void init(int containerBlocks, int metadataBlocks);

    int findFreeBlock() const;
    std::vector<uint32_t> findNFreeBlocks(uint32_t n) const;
    bool isFree(uint32_t block) const;
    void markUsed(uint32_t block);
    void markFree(uint32_t block);

    std::vector<char> serialize();
    void deserialize(std::vector<char> bytes, uint32_t containerBlocks);

private:
    std::vector<bool> flags;
    int num_flags;
};

#endif // DMAP_H