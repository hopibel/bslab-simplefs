#include "Superblock.h"
#include "Dmap.h"
#include "Fat.h"
#include "Root.h"
#include <cassert>
#include <cstring>
#include <type_traits>

void Superblock::init(int numBlocks) {
    containerBlocks = numBlocks;
    dmapStart = getSuperblockStart() + getSuperblockSize();
    fatStart = dmapStart + Dmap::requiredBlocks(containerBlocks);
    rootStart = fatStart + Fat::requiredBlocks(containerBlocks);
    dataStart = rootStart + Root::requiredBlocks(containerBlocks);

    // assert(getSuperblockSize() > 0);
    // assert(getDmapSize() > 0);
    // assert(getFatSize() > 0);
    // assert(getRootSize() > 0);
    // assert(getDataSize() > 0);
}

char* Superblock::serialize(char* buffer) const {
    static_assert(std::is_trivially_copyable<Superblock>::value, "Superblock not trivially copyable");

    std::memcpy(buffer, this, sizeof(*this));
    return buffer;
}