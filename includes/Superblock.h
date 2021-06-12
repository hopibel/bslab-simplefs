#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

class Superblock {
public:
    void init(int numBlocks);
    char* serialize(char* buffer) const;

    // Start block
    int getSuperblockStart() const { return 0; }
    int getDmapStart() const { return dmapStart; }
    int getFatStart() const { return fatStart; }
    int getRootStart() const { return rootStart; }
    int getDataStart() const { return dataStart; }

    // Size in blocks
    int getSuperblockSize() const { return 1; }
    int getDmapSize() const { return fatStart - dmapStart; }
    int getFatSize() const { return rootStart - fatStart; }
    int getRootSize() const { return dataStart - rootStart; }
    int getDataSize() const { return containerBlocks - dataStart; }
    int getContainerSize() const { return containerBlocks; }

private:
    int containerBlocks;
    int dmapStart, fatStart, rootStart, dataStart;
};

#endif // SUPERBLOCK_H