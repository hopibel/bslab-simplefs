#include "Superblock.h"
#include "Dmap.h"
#include "Fat.h"
#include "Root.h"
#include "myfs-structs.h"

#include <cstring>
#include <stdexcept>
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

    std::memcpy(buffer, this, sizeof(Superblock));

    return buffer;
}

void Superblock::deserialize(std::vector<char> bytes) {
    if (bytes.size() != BLOCK_SIZE) {
        throw std::invalid_argument("expected " + std::to_string(BLOCK_SIZE) + " bytes. received " + std::to_string(bytes.size()));
    }
    std::memcpy(this, bytes.data(), sizeof(Superblock));
}