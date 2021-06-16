#ifndef ROOT_H
#define ROOT_H

#include "OnDiskFile.h"

#include <sys/types.h>
#include <vector>

class Root {
public:
    static int requiredBlocks(int containerBlocks);

    // Initialize empty Root file directory for the given container size
    void init(int containerBlocks);

    std::vector<char> serialize();
    void deserialize(std::vector<char> bytes, int containerBlocks);

    bool isFull() const;
    bool hasFile(std::string path) const;
    OnDiskFile& getFile(std::string path);
    std::vector<std::string> getFileList() const;

    OnDiskFile& mknod(std::string path, mode_t mode);

private:
    std::vector<OnDiskFile> files {std::vector<OnDiskFile>(NUM_DIR_ENTRIES)};
};

#endif // ROOT_H