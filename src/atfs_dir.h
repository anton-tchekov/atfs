#ifndef __ATFS_DIR_H__
#define __ATFS_DIR_H__

#include "atfs.h"

#if 0

typedef struct
{
	u32 type;
	u32 addr;
	u32 cSize;
	u32 bSize;
	char name[48];
} DirEntry;

void create_directory_entry(BlockDevice *dev, u32 chunkAddr, u32 sizeInChunks,
	DirEntry entry);

DirEntry get_adress_by_path(BlockDevice *dev, char* path, DirEntry currDir);

#endif

#endif /* __ATFS_DIR_H__ */
