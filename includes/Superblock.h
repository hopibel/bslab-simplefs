#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

class Superblock {
public:
    void init(unsigned numBlocks);
    // char* serialize(char* buffer);

    // Start block
    unsigned getDmapStart() const { return dmapStart; }
    unsigned getFatStart() const { return fatStart; }
    unsigned getRootStart() const { return rootStart; }
    unsigned getDataStart() const { return dataStart; }

    // Size in blocks
    unsigned getDmapSize() const { return fatStart - dmapStart; }
    unsigned getFatSize() const { return rootStart - fatStart; }
    unsigned getRootSize() const { return dataStart - rootStart; }
    unsigned getDataSize() const { return containerBlocks - dataStart; }
    unsigned getContainerSize() const { return containerBlocks; }

private:
    unsigned containerBlocks;
    unsigned dmapStart, fatStart, rootStart, dataStart;
};

#endif // SUPERBLOCK_H