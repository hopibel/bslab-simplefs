#ifndef DMAP_H
#define DMAP_H

class Dmap {
public:
    static int requiredBlocks(int containerBlocks);

    // Initialize empty DMAP for the given container size
    void init(int containerBlocks);

private:
};

#endif // DMAP_H