/**
 * @file    atfs_move.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    21.09.2023
 * @brief   ATFS Move/Rename function
 */

#ifndef __ATFS_MOVE_H__
#define __ATFS_MOVE_H__

#include "atfs.h"

/**
 * @brief Move a file or a directory from the source path
 *        to the destination path.
 *
 * @param dev Block device
 * @param dst Destination path
 * @param src Source path
 * @return Status code
 */
ATFS_Status atfs_move(BlockDevice *dev, const char *dst, const char *src);

#endif /* __ATFS_MOVE_H__ */
