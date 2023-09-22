/**
 * @file    atfs_delete.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    21.09.2023
 */

#include "atfs_delete.h"

ATFS_Status _dir_entry_delete(BlockDevice *dev,
	const char *name, u32 block, u32 size)
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
			if(!strcmp(entry_name, name))
			{
				// atfs_free(dev, entry_start, entry_size);
				memset(entry_name, 0, ATFS_DIR_ENTRY_SIZE);
				dev->Write(block, 1, buf);
				return ATFS_STATUS_OK;
			}
		}
	}

	return ATFS_STATUS_NO_SUCH_FILE_OR_DIRECTORY;
}

ATFS_Status atfs_delete(BlockDevice *dev, const char *path)
{
	/* TODO: This is work in progress */

	return ATFS_STATUS_NOT_IMPLEMENTED;
	(void)dev;
	(void)path;
}
