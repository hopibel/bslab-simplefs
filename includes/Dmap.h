#ifndef DMAP_H
#define DMAP_H

#include <vector>

class Dmap {
public:
    static int requiredBlocks(int containerBlocks);

    // Initialize empty DMAP for the given container size
    void init(int containerBlocks, int metadataBlocks);

    int findFreeBlock();
    bool isFree(int block) const;
    void markUsed(int block);
    void markFree(int block);

    std::vector<unsigned char> serialize();

private:
    std::vector<bool> flags;
    int num_flags;
};

#endif // DMAP_H