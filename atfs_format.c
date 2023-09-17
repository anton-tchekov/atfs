#include "atfs_format.h"

#define ATFS_INITIAL_ROOT_SIZE 1

static Status _setup_boot_block(BlockDevice *dev)
{
	u8 buf[ATFS_BLOCK_SIZE];
	memcpy(buf + ATFS_OFFSET_SIGNATURE, ATFS_SIGNATURE, sizeof(ATFS_SIGNATURE));
	write_le_32(buf + ATFS_OFFSET_REVISION, ATFS_REVISION);
	write_le_32(buf + ATFS_OFFSET_FREE_NEXT, ATFS_INITIAL_ROOT_SIZE + 1);
	write_le_32(buf + ATFS_OFFSET_FREE_SIZE, 0);
	write_le_32(buf + ATFS_OFFSET_ROOT_BLOCK, 1);
	write_le_32(buf + ATFS_OFFSET_ROOT_SIZE, ATFS_INITIAL_ROOT_SIZE);
	RETURN_IF(dev->Write(0, 1, buf));
}

static Status _setup_root_block(BlockDevice *dev)
{
	u8 buf[dev->BlockSize];

	/* Zero the first byte of the root directory */
	buf[0] = '\0';
	RETURN_IF(dev->Write(1, 1, buf));
}

static Status _setup_free_list(BlockDevice *dev)
{
	u8 buf[dev->];

	/* Free space is total size minus the initial size of the root directory
		and minus 1 for the bootsector */
	u32 free_size = dev->BlockCount - ATFS_INITIAL_ROOT_SIZE + 1;

	/* There is no next free area */
	write_le_32(buf + ATFS_FREE_OFFSET_NEXT, 0);

	/* Whole remaining space is marked as free */
	write_le_32(buf + ATFS_FREE_OFFSET_SIZE, size);

	/* Free space starts after the root directory */
	RETURN_IF(dev->Write(ATFS_INITIAL_ROOT_SIZE + 1, 1, buf));
}

Status atfs_format(BlockDevice *dev)
{
	RETURN_IF(_setup_bootsector(dev));
	RETURN_IF(_setup_root_block(dev));
	RETURN_IF(_setup_freelist(dev));
	return STATUS_OK;
}

