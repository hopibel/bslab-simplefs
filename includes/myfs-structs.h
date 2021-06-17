//
//  myfs-structs.h
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#ifndef myfs_structs_h
#define myfs_structs_h

#include <cstdint>

#define NAME_LENGTH 255
#define BLOCK_SIZE 512
#define NUM_DIR_ENTRIES 64
#define NUM_OPEN_FILES 64

// TODO: Add structures of your file system here
const uint32_t CONTAINER_SIZE_MB = 64; // container size in Megabytes
const uint32_t CONTAINER_BLOCKS = CONTAINER_SIZE_MB * (1024 * 1024 / BLOCK_SIZE);
const uint32_t FREE = 0;
const uint32_t END_OF_CLUSTER = 0xFFFFFFFF;

#endif /* myfs_structs_h */
