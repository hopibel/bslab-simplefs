#ifndef ROOT_H
#define ROOT_H

#include "OnDiskFile.h"

#include <vector>

class Root {
public:
    static int requiredBlocks(int containerBlocks);

    // Initialize empty Root file directory for the given container size
    void init(int containerBlocks);

    std::vector<char> serialize();
    void deserialize(std::vector<char> bytes, int containerBlocks);

private:
    std::vector<OnDiskFile>  RootDir { std::vector<OnDiskFile>(64)};
};

#endif // ROOT_H