/**
 * @file    atfs_move.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    21.09.2023
 */

#include "atfs_move.h"
#include "atfs_util.h"
#include <string.h>

ATFS_Status _dir_entry_rename(BlockDevice *dev,
	const char *dst, const char *src, u32 block, u32 size)
{
	u8 buf[dev->BlockSize];
	u32 end, offset;
	for(end = block + size; block < end; ++block)
	{
		dev->Read(block, 1, buf);
		for(offset = 0; offset < dev->BlockSize;
			offset += ATFS_DIR_ENTRY_SIZE)
		{
			char *entry_name =
				(char *)(buf + offset + ATFS_DIR_ENTRY_OFFSET_NAME);
			if(!strcmp(entry_name, src))
			{
				strncpy(entry_name, dst, ATFS_MAX_FILE_NAME_LENGTH);
				dev->Write(block, 1, buf);
				return ATFS_STATUS_OK;
			}
		}
	}

	return ATFS_STATUS_NO_SUCH_FILE_OR_DIRECTORY;
}

ATFS_Status atfs_move(BlockDevice *dev, const char *dst, const char *src)
{
	/* TODO: This is work in progress */

	return ATFS_STATUS_NOT_IMPLEMENTED;
	(void)dev;
	(void)dst;
	(void)src;
}
