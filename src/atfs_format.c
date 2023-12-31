/**
 * @file    atfs_format.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    18.09.2023
 */

#include "atfs_format.h"
#include "atfs_util.h"
#include <string.h>

static ATFS_Status _setup_boot_block(BlockDevice *dev)
{
	u8 buf[dev->BlockSize];
	memset(buf, 0, dev->BlockSize);

	memcpy(buf + ATFS_OFFSET_SIGNATURE, ATFS_SIGNATURE, sizeof(ATFS_SIGNATURE));
	atfs_write32(buf + ATFS_OFFSET_REVISION, ATFS_REVISION);
	atfs_write32(buf + ATFS_OFFSET_FREE_NEXT, ATFS_INITIAL_ROOT_SIZE + 1);
	atfs_write32(buf + ATFS_OFFSET_FREE_SIZE, 0);
	atfs_write32(buf + ATFS_OFFSET_ROOT_BLOCK, 1);
	atfs_write32(buf + ATFS_OFFSET_ROOT_SIZE, ATFS_INITIAL_ROOT_SIZE);
	return dev->Write(0, 1, buf);
}

static ATFS_Status _setup_root_block(BlockDevice *dev)
{
	u8 buf[dev->BlockSize];
	u32 i;

	/* TODO: Not really satisfied with having to use a buffer just to
		zero blocks. Maybe add a dev->Zero() operation? */
	memset(buf, 0, dev->BlockSize);
	for(i = 1; i < 1 + ATFS_INITIAL_ROOT_SIZE; ++i)
	{
		PROPAGATE(dev->Write(i, 1, buf));
	}

	return ATFS_STATUS_OK;
}

static ATFS_Status _setup_free_list(BlockDevice *dev)
{
	u8 buf[dev->BlockSize];
	memset(buf, 0, dev->BlockSize);

	/* Free space is total size minus the initial size of the root directory
		and minus 1 for the bootsector */
	u32 free_size = dev->BlockCount - ATFS_INITIAL_ROOT_SIZE - 1;

	/* There is no next free area */
	atfs_write32(buf + ATFS_OFFSET_FREE_NEXT, 0);

	/* Whole remaining space is marked as free */
	atfs_write32(buf + ATFS_OFFSET_FREE_SIZE, free_size);

	/* Free space starts after the root directory */
	return dev->Write(ATFS_INITIAL_ROOT_SIZE + 1, 1, buf);
}

ATFS_Status atfs_format(BlockDevice *dev)
{
	PROPAGATE(_setup_boot_block(dev));
	PROPAGATE(_setup_root_block(dev));
	PROPAGATE(_setup_free_list(dev));
	return ATFS_STATUS_OK;
}
