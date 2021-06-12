#ifndef FAT_H
#define FAT_H

class Fat {
public:
    static int requiredBlocks(int containerBlocks);

    // Initialize empty FAT for the given container size
    void init(int containerBlocks);

private:
};

#endif // FAT_H