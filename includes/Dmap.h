#ifndef DMAP_H
#define DMAP_H

class Dmap {
public:
    static unsigned requiredBlocks(unsigned containerBlocks);

    // Initialize empty DMAP for the given container size
    void init(unsigned containerBlocks);

private:
};

#endif // DMAP_H