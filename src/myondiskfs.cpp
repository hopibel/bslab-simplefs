//
// Created by Oliver Waldhorst on 20.03.20.
// Copyright © 2017-2020 Oliver Waldhorst. All rights reserved.
//

#include "myondiskfs.h"
#include "myfs-structs.h"
#include <algorithm>
#include <asm-generic/errno-base.h>
#include <chrono>
#include <cstdint>
#include <stdexcept>

// For documentation of FUSE methods see https://libfuse.github.io/doxygen/structfuse__operations.html

#undef DEBUG

// TODO: Comment lines to reduce debug messages
#define DEBUG
#define DEBUG_METHODS
#define DEBUG_RETURN_VALUES

#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "macros.h"
#include "myfs.h"
#include "myfs-info.h"
#include "blockdevice.h"

/// @brief Constructor of the on-disk file system class.
///
/// You may add your own constructor code here.
MyOnDiskFS::MyOnDiskFS() : MyFS() {
    // create a block device object
    this->blockDevice= new BlockDevice(BLOCK_SIZE);

    // TODO: [PART 2] Add your constructor code here
}

/// @brief Destructor of the on-disk file system class.
///
/// You may add your own destructor code here.
MyOnDiskFS::~MyOnDiskFS() {
    // free block device object
    delete this->blockDevice;

    // TODO: [PART 2] Add your cleanup code here

}

/// @brief Create a new file.
///
/// Create a new file with given name and permissions.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] mode Permissions for file access.
/// \param [in] dev Can be ignored.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseMknod(const char *path, mode_t mode, dev_t dev) {
    LOGM();

    // [PART 2] Implement this!

    if (root.isFull()) {
        RETURN(-ENOSPC);
    }
    if (std::string(path).length() > NAME_LENGTH){
        RETURN(-ENAMETOOLONG);
    }
    if (root.hasFile(path)) {
        RETURN(-EEXIST);
    }

    root.mknod(path+1, mode);
    LOGF("Created %s with mode: %o", path, mode);

    RETURN(0);
}

/// @brief Delete a file.
///
/// Delete a file with given name from the file system.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseUnlink(const char *path) {
    LOGM();

    // TODO: [PART 2] Implement this!

    RETURN(0);
}

/// @brief Rename a file.
///
/// Rename the file with with a given name to a new name.
/// Note that if a file with the new name already exists it is replaced (i.e., removed
/// before renaming the file.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] newpath  New name of the file, starting with "/".
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseRename(const char *path, const char *newpath) {
    LOGM();

    // TODO: [PART 2] Implement this!

    RETURN(0);
}

/// @brief Get file meta data.
///
/// Get the metadata of a file (user & group id, modification times, permissions, ...).
/// \param [in] path Name of the file, starting with "/".
/// \param [out] statbuf Structure containing the meta data, for details type "man 2 stat" in a terminal.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseGetattr(const char *path, struct stat *statbuf) {
    LOGM();

    // [PART 2] Implement this!

    LOGF( "\tAttributes of %s requested\n", path );

    if ( strcmp( path, "/" ) == 0 ) {
        statbuf->st_uid = getuid(); // The owner of the file/directory is the user who mounted the filesystem
        statbuf->st_gid = getgid(); // The group of the file/directory is the same as the group of the user who mounted the filesystem
        statbuf->st_atime = time(nullptr); // The last "a"ccess of the file/directory is right now
        statbuf->st_mtime = time(nullptr); // The last "m"odification of the file/directory is right now
        statbuf->st_mode = S_IFDIR | 0755;
        statbuf->st_nlink = 2; // Why "two" hardlinks instead of "one"? The answer is here: http://unix.stackexchange.com/a/101536
        RETURN(0);
    }

    int ret = 0;
    if (root.hasFile(path+1)) {
        // gespeicherte Attribute zurückgeben
        OnDiskFile& file = root.getFile(path+1);
        *statbuf = file.getStat();
        LOGF("Permissions: %o", statbuf->st_mode);
        // statbuf->st_mode = S_IFREG | file.getMode();
    } else {
        ret = -ENOENT;
    }

    RETURN(ret);

}

/// @brief Change file permissions.
///
/// Set new permissions for a file.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] mode New mode of the file.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseChmod(const char *path, mode_t mode) {
    LOGM();

    // TODO: [PART 2] Implement this!

    RETURN(0);
}

/// @brief Change the owner of a file.
///
/// Change the user and group identifier in the meta data of a file.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] uid New user id.
/// \param [in] gid New group id.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseChown(const char *path, uid_t uid, gid_t gid) {
    LOGM();

    // TODO: [PART 2] Implement this!

    RETURN(0);
}

/// @brief Open a file.
///
/// Open a file for reading or writing. This includes checking the permissions of the current user and incrementing the
/// open file count.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [out] fileInfo Can be ignored in Part 1
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseOpen(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();

    // [PART 2] Implement this!

    LOGF("Opening %s", path);

    // zu viele geöffnete Dateien
    if (openFileCount >= NUM_OPEN_FILES) {
        LOGF("Too many open files (limit %d)", NUM_OPEN_FILES);
        RETURN(-EMFILE);
    }

    if (root.hasFile(path+1)) {
        auto& fileMeta = root.getFile(path+1);
        fileInfo->fh = generateFilehandle();
        auto& openFile = openFiles[fileInfo->fh];
        openFile.isFree = false;
        openFile.blockList = fat.getBlockList(fileMeta.getFirstBlock());

        ++openFileCount;
    } else {
        LOG("File not found");
        RETURN(-ENOENT);
    }

    RETURN(0);
}

/// @brief Read from a file.
///
/// Read a given number of bytes from a file starting form a given position.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// Note that the file content is an array of bytes, not a string. I.e., it is not (!) necessarily terminated by '\0'
/// and may contain an arbitrary number of '\0'at any position. Thus, you should not use strlen(), strcpy(), strcmp(),
/// ... on both the file content and buf, but explicitly store the length of the file and all buffers somewhere and use
/// memcpy(), memcmp(), ... to process the content.
/// \param [in] path Name of the file, starting with "/".
/// \param [out] buf The data read from the file is stored in this array. You can assume that the size of buffer is at
/// least 'size'
/// \param [in] size Number of bytes to read
/// \param [in] offset Starting position in the file, i.e., number of the first byte to read relative to the first byte of
/// the file
/// \param [in] fileInfo Can be ignored in Part 1
/// \return The Number of bytes read on success. This may be less than size if the file does not contain sufficient bytes.
/// -ERRNO on failure.
int MyOnDiskFS::fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();

    // TODO: [PART 2] Implement this!

    RETURN(0);
}

/// @brief Write to a file.
///
/// Write a given number of bytes to a file starting at a given position.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// Note that the file content is an array of bytes, not a string. I.e., it is not (!) necessarily terminated by '\0'
/// and may contain an arbitrary number of '\0'at any position. Thus, you should not use strlen(), strcpy(), strcmp(),
/// ... on both the file content and buf, but explicitly store the length of the file and all buffers somewhere and use
/// memcpy(), memcmp(), ... to process the content.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] buf An array containing the bytes that should be written.
/// \param [in] size Number of bytes to write.
/// \param [in] offset Starting position in the file, i.e., number of the first byte to read relative to the first byte of
/// the file.
/// \param [in] fileInfo Can be ignored in Part 1 .
/// \return Number of bytes written on success, -ERRNO on failure.
int MyOnDiskFS::fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();

    // TODO: [PART 2] Implement this!
    // TODO: test writing more than 3 and >3 blocks

    // get buffer and metadata objects
    OpenFile& fbuf = openFiles[fileInfo->fh];
    OnDiskFile& fmeta = root.getFile(path + 1);

    // find block
    uint32_t blockListIndex = offset / BLOCK_SIZE;

    // flush current buffer if different block
    if (blockListIndex >= fbuf.blockList.size() || fbuf.blockList[blockListIndex] != fbuf.blockNo) {
        if (fbuf.dirty) {
            fbuf.dirty = false;
            blockDevice->write(fbuf.blockNo, fbuf.buffer.data());
        }
    }

    // allocate new blocks if necessary
    uint32_t lastBlockIndex = (offset + size - 1) / BLOCK_SIZE;
    if (lastBlockIndex >= fbuf.blockList.size()) {
        auto blocksNeeded = lastBlockIndex - fbuf.blockList.size() + 1;
        std::vector<uint32_t> newBlocks;
        try {
            newBlocks = dmap.findNFreeBlocks(blocksNeeded+1);
        } catch (const std::runtime_error&) {
            return -ENOSPC;
        }

        buffer.fill(0);
        // empty file edge case: firstBlock = END_OF_CLUSTER
        auto it = newBlocks.begin();
        if (fmeta.getFirstBlock() == END_OF_CLUSTER) {
            blockDevice->write(*it, buffer.data());
            fmeta.setFirstBlock(*it);
            dmap.markUsed(*it);
            fat.allocateBlock(*it);
            ++it;
        }
        for (; it != newBlocks.end(); ++it) {
            blockDevice->write(*it, buffer.data());
            dmap.markUsed(*it);
            fat.appendBlock(fmeta.getFirstBlock(), *it);
        }
        // update cached blockList
        fbuf.blockList = fat.getBlockList(fmeta.getFirstBlock());
    }

    auto it = fbuf.blockList.begin() + blockListIndex;
    const char* head = buf;
    auto remainingBytes = size;
    // write first block
    {
        std::size_t blockOffset = offset % BLOCK_SIZE;
        off_t firstSize = std::min(size, BLOCK_SIZE - blockOffset);

        blockDevice->read(*it, fbuf.buffer.data());
        fbuf.blockNo = *it;
        std::copy_n(head, firstSize, fbuf.buffer.data() + blockOffset);
        fbuf.dirty = true;

        ++it;
        head += firstSize;
        remainingBytes -= firstSize;
    }
    // write middle blocks (full blocks)
    // flush previous block
    if (remainingBytes > 0) {
        blockDevice->write(fbuf.blockNo, fbuf.buffer.data());
        fbuf.dirty = false;
    }
    while (remainingBytes >= BLOCK_SIZE) {
        blockDevice->read(*it, fbuf.buffer.data());
        fbuf.blockNo = *it;
        std::copy_n(head, BLOCK_SIZE, fbuf.buffer.data());
        blockDevice->write(*it, fbuf.buffer.data());
        fbuf.dirty = false;

        ++it;
        head += BLOCK_SIZE;
        remainingBytes -= BLOCK_SIZE;
    }
    // write last block (size less than a full block)
    if (remainingBytes > 0) {
        blockDevice->read(*it, fbuf.buffer.data());
        fbuf.blockNo = *it;
        std::copy_n(head, remainingBytes, fbuf.buffer.data());
        fbuf.dirty = true;
        // don't write yet in case of multiple writes
    }

    // metadata
    fmeta.setMtime();
    fmeta.setCtime();
    fmeta.setSize(std::max(fmeta.getSize(), offset + static_cast<off_t>(size)));

    // saves filesystem structures
    writeMetadata();

    RETURN(static_cast<int>(size));
}

/// @brief Close a file.
///
/// \param [in] path Name of the file, starting with "/".
/// \param [in] File handel for the file set by fuseOpen.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseRelease(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();

    // [PART 2] Implement this!

    // write cached block
    if (fileInfo->fh >= 0 && fileInfo->fh < NUM_OPEN_FILES) {
        OpenFile& opened = openFiles[fileInfo->fh];
        blockDevice->write(opened.blockNo, opened.buffer.data());

        openFileCount--;
        opened.isFree = true;
    } else {
        RETURN(-ENOENT);
    }

    RETURN(0);
}

/// @brief Truncate a file.
///
/// Set the size of a file to the new size. If the new size is smaller than the old size, spare bytes are removed. If
/// the new size is larger than the old size, the new bytes may be random.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] newSize New size of the file.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseTruncate(const char *path, off_t newSize) {
    LOGM();

    // TODO: [PART 2] Implement this!

    RETURN(0);
}

/// @brief Truncate a file.
///
/// Set the size of a file to the new size. If the new size is smaller than the old size, spare bytes are removed. If
/// the new size is larger than the old size, the new bytes may be random. This function is called for files that are
/// open.
/// You do not have to check file permissions, but can assume that it is always ok to access the file.
/// \param [in] path Name of the file, starting with "/".
/// \param [in] newSize New size of the file.
/// \param [in] fileInfo Can be ignored in Part 1.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseTruncate(const char *path, off_t newSize, struct fuse_file_info *fileInfo) {
    LOGM();

    // TODO: [PART 2] Implement this!

    RETURN(0);
}

/// @brief Read a directory.
///
/// Read the content of the (only) directory.
/// You do not have to check file permissions, but can assume that it is always ok to access the directory.
/// \param [in] path Path of the directory. Should be "/" in our case.
/// \param [out] buf A buffer for storing the directory entries.
/// \param [in] filler A function for putting entries into the buffer.
/// \param [in] offset Can be ignored.
/// \param [in] fileInfo Can be ignored.
/// \return 0 on success, -ERRNO on failure.
int MyOnDiskFS::fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();

    // [PART 2] Implement this!

    LOGF( "--> Getting The List of Files of %s", path );

    filler( buf, ".", nullptr, 0 ); // Current Directory
    filler( buf, "..", nullptr, 0 ); // Parent Directory

    if (std::string(path) == "/") // If the user is trying to show the files/directories of the root directory list all files
    {
        for (std::string filename : root.getFileList()) {
            // LOGF("%s", filename.c_str());
            filler(buf, filename.c_str(), nullptr, 0);
        }
    }

    RETURN(0);
}

/// Initialize a file system.
///
/// This function is called when the file system is mounted. You may add some initializing code here.
/// \param [in] conn Can be ignored.
/// \return 0.
void* MyOnDiskFS::fuseInit(struct fuse_conn_info *conn) {
    // Open logfile
    this->logFile= fopen(((MyFsInfo *) fuse_get_context()->private_data)->logFile, "w+");
    if(this->logFile == NULL) {
        fprintf(stderr, "ERROR: Cannot open logfile %s\n", ((MyFsInfo *) fuse_get_context()->private_data)->logFile);
    } else {
        // turn of logfile buffering
        setvbuf(this->logFile, NULL, _IOLBF, 0);

        LOG("Starting logging...\n");

        LOG("Using on-disk mode");

        LOGF("Container file name: %s", ((MyFsInfo *) fuse_get_context()->private_data)->contFile);

        int ret= this->blockDevice->open(((MyFsInfo *) fuse_get_context()->private_data)->contFile);

        if(ret >= 0) {
            LOG("Container file does exist, reading");

            // TODO: [PART 2] Read existing structures form file

            // TODO: return error codes
            readMetadata();

        } else if(ret == -ENOENT) {
            LOG("Container file does not exist, creating a new one");

            ret = this->blockDevice->create(((MyFsInfo *) fuse_get_context()->private_data)->contFile);

            if (ret >= 0) {
                // TODO: [PART 2] Create empty structures in file
                LOG("Writing empty filesystem structures");

                // Preallocate container by writing empty block at the end
                buffer.fill(0); // zero-initialized dummy block
                ret = blockDevice->write(CONTAINER_BLOCKS-1, buffer.data());

                // Create structures, allocate memory, etc
                superblock.init(CONTAINER_BLOCKS);
                dmap.init(CONTAINER_BLOCKS, superblock.getDataStart());
                fat.init(CONTAINER_BLOCKS);
                root.init(CONTAINER_BLOCKS);

                if (ret >= 0) {
                    // Write structures to disk
                    ret = blockDevice->write(superblock.getSuperblockStart(), superblock.serialize(buffer.data()));
                    if (ret < 0) {
                        LOG("Failed to write superblock");
                    }
                    writeMetadata();
                } else {
                    LOG("Failed to preallocate container");
                }
            }
        }

        if(ret < 0) {
            LOGF("ERROR: Access to container file failed with error %d", ret);
        }
    }

    return 0;
}

/// @brief Clean up a file system.
///
/// This function is called when the file system is unmounted. You may add some cleanup code here.
void MyOnDiskFS::fuseDestroy() {
    LOGM();

    // [PART 2] Implement this!
    writeMetadata();
    blockDevice->close();
}

// TODO: [PART 2] You may add your own additional methods here!

// overload to show available space
int MyOnDiskFS::fuseStatfs(const char *path, struct statvfs *statInfo) {
    LOGM();

    statInfo->f_bsize = BLOCK_SIZE;
    statInfo->f_frsize = BLOCK_SIZE;
    statInfo->f_blocks = superblock.getContainerSize();
    statInfo->f_bfree = dmap.countFreeBlocks();
    statInfo->f_bavail = statInfo->f_bfree;

    return 0;
}

void MyOnDiskFS::readMetadata() {
    LOGM();
    superblock.deserialize(
        readFromDisk(superblock.getSuperblockStart(), superblock.getSuperblockSize())
    );
    dmap.deserialize(
        readFromDisk(superblock.getDmapStart(), superblock.getDmapSize()),
        superblock.getContainerSize()
    );
    fat.deserialize(
        readFromDisk(superblock.getFatStart(), superblock.getFatSize()),
        superblock.getContainerSize()
    );
    root.deserialize(
        readFromDisk(superblock.getRootStart(), superblock.getRootSize()),
        superblock.getContainerSize()
    );
}

// Read `count` blocks into a char vector starting from `startBlock`
std::vector<char> MyOnDiskFS::readFromDisk(int startBlock, int count) {
    LOGM();
    std::vector<char> bytes;
    for (int offset = 0; offset < count; ++offset) {
        // read a block
        blockDevice->read(startBlock + offset, buffer.data());
        // append to bytes vector
        std::copy(buffer.begin(), buffer.end(), std::back_inserter(bytes));
    }

    LOGF("read %ld bytes", bytes.size());

    return bytes;
}

void MyOnDiskFS::writeMetadata() {
    // skip if it has been less than 1000ms since last write
    auto begin = lastMetadataWrite;
    auto end = std::chrono::system_clock::now();
    auto msSinceLastWrite = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    lastMetadataWrite = end;
    if (msSinceLastWrite < 1000) {
        return;
    }

    // superblock is written on creation and never changes
    dumpToDisk(dmap.serialize(), superblock.getDmapStart());
    dumpToDisk(fat.serialize(), superblock.getFatStart());
    dumpToDisk(root.serialize(), superblock.getRootStart());
}

void MyOnDiskFS::dumpToDisk(std::vector<char> bytes, int startBlock) const {
    LOGM();
    // calc blocks, rounded up
    auto blocksNeeded = (bytes.size() + BLOCK_SIZE - 1) / BLOCK_SIZE;

    // Zero-pad to be divisible by BLOCK_SIZE
    if (bytes.size() % BLOCK_SIZE != 0) {
        bytes.resize(blocksNeeded * BLOCK_SIZE, 0);
    }

    // write blocks to disk
    for (std::size_t offset = 0; offset < blocksNeeded; ++offset) {
        blockDevice->write(startBlock + offset, bytes.data() + offset * BLOCK_SIZE);
    }

    LOGF("wrote %ld bytes", bytes.size());
}

uint32_t MyOnDiskFS::generateFilehandle() {
    for (uint32_t i = 0; i < openFiles.size(); ++i) {
        if (openFiles[i].isFree) {
            return i;
        }
    }
    throw std::runtime_error("Ran out of free filehandles!");
}

// DO NOT EDIT ANYTHING BELOW THIS LINE!!!

/// @brief Set the static instance of the file system.
///
/// Do not edit this method!
void MyOnDiskFS::SetInstance() {
    MyFS::_instance= new MyOnDiskFS();
}
