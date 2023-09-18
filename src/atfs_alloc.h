#ifndef __ATFS_ALLOC_H__
#define __ATFS_ALLOC_H__

#include "atfs.h"

/**
 * @brief Allocate `size` contiguous blocks on disk
 *
 * @param size Number of blocks to allocate
 * @param start Output parameter starting block
 * @return Status
 */
ATFS_Status fs_alloc(BlockDevice *dev, u32 size, u32 *start);

/**
 * @brief Free `count` blocks starting at `block`
 *
 * @param block Start block
 * @param count Number of blocks to free
 */
ATFS_Status fs_free(BlockDevice *dev, u32 block, u32 count);

#endif /* __ATFS_ALLOC_H__ */