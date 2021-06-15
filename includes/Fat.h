#ifndef FAT_H
#define FAT_H

#include <vector>

class Fat {
public:
    static int requiredBlocks(int containerBlocks);

    // Initialize empty FAT for the given container size
    void init(int containerBlocks);

    std::vector<char> serialize();

private:
};

#endif // FAT_H