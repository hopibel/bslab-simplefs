#ifndef FAT_H
#define FAT_H

class Fat {
public:
    static unsigned requiredBlocks(unsigned containerBlocks);

    // Initialize empty FAT for the given container size
    void init(unsigned containerBlocks);

private:
};

#endif // FAT_H