#include "Superblock.h"
#include "Dmap.h"
#include "Fat.h"
#include "Root.h"

void Superblock::init(unsigned numBlocks) {
    containerBlocks = numBlocks;
    dmapStart = 1; // superblock is always 0
    fatStart = dmapStart + Dmap::requiredBlocks(containerBlocks);
    rootStart = fatStart + Fat::requiredBlocks(containerBlocks);
    dataStart = rootStart + Root::requiredBlocks(containerBlocks);
    // TODO: assert dataStart < containerBlocks
}