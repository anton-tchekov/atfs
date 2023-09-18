#ifndef __ATFS_UTIL_H__
#define __ATFS_UTIL_H__

#include "types.h"

void atfs_write32(u8 *p, u32 v);
u32 atfs_read32(u8 *p);

#endif /* __ATFS_UTIL_H__ */
