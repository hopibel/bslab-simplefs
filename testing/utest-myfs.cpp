//
//  test-myfs.cpp
//  testing
//
//  Created by Oliver Waldhorst on 15.12.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#include "catch.hpp"


#include "tools.hpp"
#include "myfs.h"
#include "myinmemoryfs.h"

#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <unistd.h>
#include <memory>

#define FILENAME "file"

// TODO: Implement your helper functions here!

TEST_CASE("T-1.11", "[Part_1]") {
    printf("Testcase 1.11: fuseRename\n");

    std::unique_ptr<MyInMemoryFS> memfs(new MyInMemoryFS());
    struct stat* statbuf = new struct stat();

    REQUIRE(memfs->fuseMknod("/foo.txt", 0644, 0) == 0);
    REQUIRE(memfs->fuseRename("/foo.txt", "/bar.txt") == 0);
    REQUIRE(memfs->fuseGetattr("/bar.txt", statbuf) == 0);
    REQUIRE(memfs->fuseGetattr("/foo.txt", statbuf) == -ENOENT);
}

TEST_CASE("T-1.12", "[Part_1]") {
    printf("Testcase 1.12: Filename length over 255\n");

    std::unique_ptr<MyInMemoryFS> memfs(new MyInMemoryFS());
    struct stat* statbuf = new struct stat();

    REQUIRE(memfs->fuseMknod("/foofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoo.txt",
        0644, 0) == -ENAMETOOLONG);
    REQUIRE(memfs->fuseGetattr("/foofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoofoo.txt",
        statbuf) == -ENOENT);
}