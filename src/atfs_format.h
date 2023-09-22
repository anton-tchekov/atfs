/**
 * @file    atfs_format.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    18.09.2023
 * @brief   ATFS Disk Format
 */

#ifndef __ATFS_FORMAT_H__
#define __ATFS_FORMAT_H__

#include "atfs.h"

/**
 * @brief Format a block device with the ATFS file system
 *
 * @param dev Block device
 * @return Status code
 */
ATFS_Status atfs_format(BlockDevice *dev);

#endif /* __ATFS_FORMAT_H__ */
