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
#include "myondiskfs.h"

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <memory>
#include <cstring>

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

TEST_CASE("T-1.13", "[Part_1]") {
    printf("Testcase 1.13: fuseMknod\n");

    std::unique_ptr<MyInMemoryFS> memfs(new MyInMemoryFS());
    struct stat* statbuf = new struct stat();

    REQUIRE(memfs->fuseMknod("/foo.txt", 0644, 0) == 0);
    REQUIRE(memfs->fuseGetattr("/foo.txt", statbuf) == 0);

}

TEST_CASE("T-1.14", "[Part_1]") {
    printf("Testcase 1.14: fuseWrite/Read\n");
    size_t size =512;
    std::unique_ptr<MyInMemoryFS> memfs(new MyInMemoryFS());
    struct fuse_file_info* stat = new struct fuse_file_info();
    char* r= new char[size];
    memset(r, 0, size);
    char* w= new char[size];
    char* w2= new char[2*size];
    memset(w, 1, size);
    memset(w2,2,2*size);
    REQUIRE(memfs->fuseMknod("/foo.txt", 0644, 0) == 0);
    REQUIRE(memfs->fuseWrite("/foo.txt",r,size,0,stat) == size);
    REQUIRE(memfs->fuseRead("/foo.txt",w,size,0,stat)==size);
    REQUIRE(memcmp(r,w,size)==0);
    REQUIRE(memfs->fuseRead("/foo.txt",w2,size*2,0,stat)==size);
    REQUIRE(memfs->fuseRead("/foo.txt",w,size/2,0,stat)==size/2);
    REQUIRE(memfs->fuseWrite("/foo.txt",r,size,size+1, stat) == size);
    REQUIRE(memfs->fuseRead("/foo.txt",w2,size*2,0,stat)==size*2);
    REQUIRE(memcmp(r,w,size)==0);
    delete []  r;
    delete []  w;
    delete []  w2;

}

TEST_CASE("T-1.15", "[Part_1]") {
    printf("Testcase 1.15: fusetruncate\n");
    size_t size = 512;
    std::unique_ptr<MyInMemoryFS> memfs(new MyInMemoryFS());
    auto *stat = new struct fuse_file_info();
    auto *r = new char[size];
    memset(r,0, size);
    auto *w = new char[size];
    memset(w,0, size);
    memfs->fuseMknod("/foo.txt", 0644, 0);
    REQUIRE(memfs->fuseWrite("/foo.txt",r,size,0,stat) == size);
    REQUIRE(memfs->fuseTruncate("/foo.txt",size/2,stat)==0);
    REQUIRE(memfs->fuseRead("/foo.txt",w,size,0,stat)==size/2);
    REQUIRE(memfs->fuseTruncate("/bar.txt",size,stat)<0);
    REQUIRE(memfs->fuseRead("/foo.txt",w,size,0,stat)==size/2);
    REQUIRE(memfs->fuseTruncate("/foo.txt",size,stat)==0);
    REQUIRE(memfs->fuseRead("/foo.txt",w,size,0,stat)==size);
    REQUIRE(memcmp(r,w,size/2)==0);
    delete []  r;
    delete []  w;
    delete stat;
}

TEST_CASE("T1.16","[Part_1]"){
    printf("Testcase 1.16: fuseChown\n");
    uid_t uid=1001;
    gid_t gid=1001;
    std::unique_ptr<MyInMemoryFS> memfs(new MyInMemoryFS());
    struct stat* statbuf = new struct stat();
    memfs->fuseMknod("/foo.txt", 0644, 0);
    REQUIRE(memfs->fuseChown("/foo.txt",uid,gid)==0);
    memfs->fuseGetattr("/foo.txt", statbuf);
    REQUIRE(statbuf->st_gid==gid);
    REQUIRE(statbuf->st_uid==uid);


}

TEST_CASE("T1.17","[Part_1]"){
    printf("Testcase 1.17: fuseMode\n");
    mode_t mode = 0666;

    std::unique_ptr<MyInMemoryFS> memfs(new MyInMemoryFS());
    struct stat* statbuf = new struct stat();
    memfs->fuseMknod("/foo.txt", 0644, 0);
    REQUIRE(memfs->fuseChmod("/foo.txt",mode)==0);
    memfs->fuseGetattr("/foo.txt", statbuf);
    REQUIRE(statbuf->st_mode==(S_IFREG |  mode));

}

TEST_CASE("T-1.18", "[Part_1]") {
    printf("Testcase 1.18: Write file with the same name\n");

    std::unique_ptr<MyInMemoryFS> memfs(new MyInMemoryFS());

    REQUIRE(memfs->fuseMknod("/foo.txt", 0644, 0) == 0);
    REQUIRE(memfs->fuseMknod("/foo.txt", 0644, 0) == -EEXIST);
    REQUIRE(memfs->fuseUnlink("/foo.txt") == 0);
    REQUIRE(memfs->fuseUnlink("/foo.txt") == -ENOENT);

}

TEST_CASE("T-1.19", "[Part_1]") {
    printf("Testcase 1.19: Unlink file\n");

    std::unique_ptr<MyInMemoryFS> memfs(new MyInMemoryFS());

    REQUIRE(memfs->fuseMknod("/foo.txt", 0644, 0) == 0);
    REQUIRE(memfs->fuseUnlink("/foo.txt") == 0);
    REQUIRE(memfs->fuseUnlink("/foo.txt") == -ENOENT);
}