#include "atfs_alloc.h"
#include "atfs_util.h"
#include <string.h>

#if 0
static ATFS_Status _chain(BlockDevice *dev, u32 block, u32 *cur_size, u32 *next)
{
	u8 buf[dev->BlockSize];
	ATFS_Status status;
	if((status = dev->Read(block, 1, buf)))
	{
		return status;
	}

	*next = atfs_read32(buf + ATFS_OFFSET_FREE_NEXT);
	*cur_size = atfs_read32(buf + ATFS_OFFSET_FREE_SIZE);
	return ATFS_STATUS_OK;
}

static ATFS_Status _find(BlockDevice *dev, u32 cur, u32 req_size)
{
	//prev = NONE;
	//_chain(dev, ATFS_SECTOR_BOOT, &cur_size, &next);


}


static ATFS_Status _perfect(BlockDevice *dev, u32 prev, u32 next)
{
	/* Make previous area point to next area */
	u8 buf[dev->BlockSize];
	ATFS_Status status;
	if((status = dev->Read(prev, 1, buf)))
	{
		return status;
	}

	atfs_write32(buf + ATFS_OFFSET_FREE_NEXT, next);
	if((status = dev->Write(prev, 1, buf)))
	{
		return status;
	}

	return ATFS_STATUS_OK;
}

static ATFS_Status _split(BlockDevice *dev,
	u32 prev, u32 cur, u32 next, u32 cur_size, u32 req_size)
{
	/* Split area */
	u8 buf[dev->BlockSize];
	ATFS_Status status;
	u32 new_start = cur + req_size;
	u32 new_size = cur_size - req_size;

	/* Make previous area point to second part */
	if((status = dev->Read(prev, 1, buf)))
	{
		return status;
	}

	atfs_write32(buf + ATFS_OFFSET_FREE_NEXT, new_start);
	if((status = dev->Write(prev, 1, buf)))
	{
		return status;
	}

	/* Make second part point to next area */
	memset(buf, 0, dev->BlockSize);
	atfs_write32(buf + ATFS_OFFSET_FREE_SIZE, new_size);
	atfs_write32(buf + ATFS_OFFSET_FREE_NEXT, next);
	if((status = dev->Write(new_start, 1, buf)))
	{
		return status;
	}

	return ATFS_STATUS_OK;
}

ATFS_Status fs_alloc(BlockDevice *dev, u32 req_size, u32 *start)
{
	u32 cur_size;
	u32 prev;
	u32 cur;
	u32 next;

	if(cur_size < req_size)
	{
		/* We have searched the entire chain */
		return ATFS_STATUS_NO_SPACE_LEFT_ON_DEVICE;
	}
	else if(cur_size == req_size)
	{
		_perfect(dev, prev, next);
	}
	else /* cur_size > req_size */
	{
		_split(dev, prev, cur, next, cur_size, req_size);
	}

	*start = cur;
	return ATFS_STATUS_OK;
}


ATFS_Status fs_free(BlockDevice *dev, u32 block, u32 count)
{
	/* TODO */
	return ATFS_STATUS_OK;
	(void)dev, (void)block, (void)count;
}
#endif
