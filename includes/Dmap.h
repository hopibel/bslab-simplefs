#ifndef DMAP_H
#define DMAP_H

#include <vector>

class Dmap {
public:
    static int requiredBlocks(int containerBlocks);

    // Initialize empty DMAP for the given container size
    void init(int containerBlocks, int metadata);
    // TODO: serialize/deserialize

    int findFreeBlock();
    bool isFree(int block) const;
    void markUsed(int block);
    void markFree(int block);

private:
    std::vector<unsigned> flags;
    int num_flags;
};

#endif // DMAP_H