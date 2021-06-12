#include "Dmap.h"
#include "myfs-structs.h"

// Calculate number of blocks required for the given container size.
unsigned Dmap::requiredBlocks(unsigned int containerBlocks) {
    // hardcoded to 64 files
    return NUM_DIR_ENTRIES;
}

void Dmap::init(unsigned int containerBlocks) {
    // TODO(hopibel): implement me
}