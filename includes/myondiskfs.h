//
// Created by Oliver Waldhorst on 20.03.20.
// Copyright © 2017-2020 Oliver Waldhorst. All rights reserved.
//

#ifndef MYFS_MYONDISKFS_H
#define MYFS_MYONDISKFS_H

#include "myfs.h"
#include "myfs-structs.h"

#include "Superblock.h"
#include "Dmap.h"
#include "Fat.h"
#include "Root.h"

#include <array>
#include <chrono>

/// @brief On-disk implementation of a simple file system.
class MyOnDiskFS : public MyFS {
protected:
    // BlockDevice blockDevice;

public:
    static MyOnDiskFS *Instance();

    // TODO: [PART 1] Add attributes of your file system here
    Superblock superblock;
    Dmap dmap;
    Fat fat;
    Root root;

    // use std::array so we can use array.fill(0) to reset it
    std::array<char, BLOCK_SIZE> buffer;

    struct OpenFile {
        std::array<char, BLOCK_SIZE> buffer;
        uint32_t blockNo = END_OF_CLUSTER; // cached block
        std::vector<uint32_t> blockList;

        bool isFree = true;
        bool dirty = false;
    };
    uint32_t openFileCount = 0;
    std::array<OpenFile, NUM_OPEN_FILES> openFiles;

    std::chrono::system_clock::time_point lastMetadataWrite = std::chrono::system_clock::now();

    MyOnDiskFS();
    ~MyOnDiskFS();

    static void SetInstance();

    // --- Methods called by FUSE ---
    // For Documentation see https://libfuse.github.io/doxygen/structfuse__operations.html
    virtual int fuseGetattr(const char *path, struct stat *statbuf);
    virtual int fuseMknod(const char *path, mode_t mode, dev_t dev);
    virtual int fuseUnlink(const char *path);
    virtual int fuseRename(const char *path, const char *newpath);
    virtual int fuseChmod(const char *path, mode_t mode);
    virtual int fuseChown(const char *path, uid_t uid, gid_t gid);
    virtual int fuseTruncate(const char *path, off_t newSize);
    virtual int fuseOpen(const char *path, struct fuse_file_info *fileInfo);
    virtual int fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
    virtual int fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
    virtual int fuseRelease(const char *path, struct fuse_file_info *fileInfo);
    virtual void* fuseInit(struct fuse_conn_info *conn);
    virtual int fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo);
    virtual int fuseTruncate(const char *path, off_t offset, struct fuse_file_info *fileInfo);
    virtual void fuseDestroy();

    virtual int fuseStatfs(const char *path, struct statvfs *statInfo);

    // TODO: Add methods of your file system here
private:
    void readMetadata();
    void syncToDisk();

    std::vector<char> readFromDisk(int startBlock, int count);
    void dumpToDisk(std::vector<char> bytes, int startBlock) const;

    uint32_t generateFilehandle();
    void cacheBlock(uint32_t blockNo, uint32_t fh);
    void flushCache(uint32_t fh);
};

#endif //MYFS_MYONDISKFS_H
