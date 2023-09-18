/**
 * @file    atfs_alloc.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    18.09.2023
 * @brief   ATFS Allocator
 */

#ifndef __ATFS_ALLOC_H__
#define __ATFS_ALLOC_H__

#include "atfs.h"

/**
 * @brief Allocate `size` contiguous blocks on disk
 *
 * @param dev Block device
 * @param size Number of blocks to allocate
 * @param start Output parameter starting block
 * @return Status
 */
ATFS_Status atfs_alloc(BlockDevice *dev, u32 size, u32 *start);

/**
 * @brief Free `count` blocks starting at `block`
 *
 * @param dev Block device
 * @param block Start block
 * @param count Number of blocks to free
 */
ATFS_Status atfs_free(BlockDevice *dev, u32 block, u32 count);

/**
 * @brief Print all free areas on device for debugging purposes
 *
 * @param dev Block Device
 */
void atfs_print_free(BlockDevice *dev);

#endif /* __ATFS_ALLOC_H__ */
