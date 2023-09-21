/**
 * @file    atfs_delete.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    21.09.2023
 * @brief   ATFS Delete function
 */

#ifndef __ATFS_DELETE_H__
#define __ATFS_DELETE_H__

#include "atfs.h"

/**
 * @brief Delete a file or a directory recursively
 *
 * @param dev Block device
 * @param path Path to delete
 * @return Status code
 */
ATFS_Status atfs_delete(BlockDevice *dev, const char *path);

#endif /* __ATFS_DELETE_H__ */
