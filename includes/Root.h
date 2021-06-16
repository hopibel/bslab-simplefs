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

    bool has(std::string path) const;
    OnDiskFile& getFile(std::string path);
    std::vector<std::string> getFileList() const;

private:
    std::vector<OnDiskFile> files {std::vector<OnDiskFile>(NUM_DIR_ENTRIES)};
};

#endif // ROOT_H