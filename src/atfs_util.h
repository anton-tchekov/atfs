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

void atfs_write32(u8 *p, u32 v);
u32 atfs_read32(u8 *p);

#endif /* __ATFS_UTIL_H__ */
