/**
 * @file    atfs_file.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    21.09.2023
 * @brief   File create, open, read and write functions
 */

#ifndef __ATFS_FILE_H__
#define __ATFS_FILE_H__

#include "atfs.h"

ATFS_Status atfs_fcreate(BlockDevice *dev, const char *path,
	ATFS_FileType type, u32 capacity);

ATFS_Status atfs_fopen(BlockDevice *dev, const char *path, ATFS_File *file);

ATFS_Status atfs_fread(ATFS_File *file, u32 block, u32 count, void *buf);

ATFS_Status atfs_fwrite(ATFS_File *file, u32 block, u32 count, const void *buf);

#endif /* __ATFS_FILE_H__ */
