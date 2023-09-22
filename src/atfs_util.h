/**
 * @file    atfs_util.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    18.09.2023
 * @brief   ATFS Utility Function
 */

#ifndef __ATFS_UTIL_H__
#define __ATFS_UTIL_H__

#include "types.h"

/**
 * @brief Write a unsigned 32-bit integer into a buffer
 *
 * @param p Pointer into buffer
 * @param v 32-bit integer value to write
 */
void atfs_write32(u8 *p, u32 v);

/**
 * @brief Read a unsigned 32-bit integer from a buffer
 *
 * @param p Pointer into buffer
 * @return 32-bit integer that was read
 */
u32 atfs_read32(const u8 *p);

#endif /* __ATFS_UTIL_H__ */
