#include "atfs_alloc.h"
#include "atfs_util.h"
#include <string.h>

#ifdef ATFS_DEBUG
#include <stdio.h>
#include <inttypes.h>
#endif /* ATFS_DEBUG */

DeviceStatus read_for_modify(BlockDevice *dev, u32 block, u8 *buf)
{
	DeviceStatus status = DEVICE_STATUS_OK;
	if(block == ATFS_SECTOR_BOOT)
	{
		/* Read contents to not overwrite bootsector (yikes) */
		status = dev->Read(block, 1, buf);
	}
	else
	{
		/* Can be safely zeroed */
		memset(buf, 0, dev->BlockSize);
	}

	return status;
}

ATFS_Status atfs_alloc(BlockDevice *dev, u32 req_size, u32 *start)
{
	u8 buf[dev->BlockSize];
	DeviceStatus status;
	u32 cur = ATFS_SECTOR_BOOT;
	u32 prev = 0;
	u32 next;
	u32 cur_size;
	do
	{
		PROPAGATE(dev->Read(cur, 1, buf));
		next = atfs_read32(buf + ATFS_OFFSET_FREE_NEXT);
		cur_size = atfs_read32(buf + ATFS_OFFSET_FREE_SIZE);
		if(cur_size >= req_size)
		{
			/* First fit: Suitable area found */
			break;
		}

		prev = cur;
		cur = next;
	}
	while(cur);

	if(cur_size < req_size)
	{
		return ATFS_STATUS_NO_SPACE_LEFT_ON_DEVICE;
	}
	else if(cur_size == req_size)
	{
		/* Make previous area point to next area */
		PROPAGATE(read_for_modify(dev, prev, buf));
		atfs_write32(buf + ATFS_OFFSET_FREE_NEXT, next);
		PROPAGATE(dev->Write(prev, 1, buf));
	}
	else /* cur_size > req_size */
	{
		/* Split area */
		u32 new_start = cur + req_size;
		u32 new_size = cur_size - req_size;

		/* Make previous area point to second part */
		PROPAGATE(read_for_modify(dev, prev, buf));
		atfs_write32(buf + ATFS_OFFSET_FREE_NEXT, new_start);
		PROPAGATE(dev->Write(prev, 1, buf));

		/* Make second part point to next area */
		memset(buf, 0, dev->BlockSize);
		atfs_write32(buf + ATFS_OFFSET_FREE_SIZE, new_size);
		atfs_write32(buf + ATFS_OFFSET_FREE_NEXT, next);
		PROPAGATE(dev->Write(new_start, 1, buf));
	}

	*start = cur;
	return ATFS_STATUS_OK;
}

ATFS_Status atfs_free(BlockDevice *dev, u32 block, u32 count)
{
	u8 buf[dev->BlockSize];
	DeviceStatus status;
	u32 prev = ATFS_SECTOR_BOOT;
	u32 next;
	u32 prev_size;
	u32 next_size;
	u32 next_next;

	do
	{
		PROPAGATE(dev->Read(prev, 1, buf));
		next = atfs_read32(buf + ATFS_OFFSET_FREE_NEXT);
		prev_size = atfs_read32(buf + ATFS_OFFSET_FREE_SIZE);
		if(next > block)
		{
			break;
		}

		prev = next;
	}
	while(prev);

	int merge_with_prev = prev + prev_size == block;
	int merge_with_next = block + count == next;

	if(merge_with_next)
	{
		PROPAGATE(dev->Read(next, 1, buf));
		next_size = atfs_read32(buf + ATFS_OFFSET_FREE_SIZE);
		next_next = atfs_read32(buf + ATFS_OFFSET_FREE_NEXT);
	}

	if(merge_with_prev && merge_with_next)
	{
		PROPAGATE(read_for_modify(dev, prev, buf));
		atfs_write32(buf + ATFS_OFFSET_FREE_NEXT, next_next);
		atfs_write32(buf + ATFS_OFFSET_FREE_SIZE, prev_size + count + next_size);
		PROPAGATE(dev->Write(prev, 1, buf));
	}
	else if(merge_with_prev)
	{
		PROPAGATE(read_for_modify(dev, prev, buf));
		atfs_write32(buf + ATFS_OFFSET_FREE_SIZE, prev_size + count);
		PROPAGATE(status = dev->Write(prev, 1, buf));
	}
	else if(merge_with_next)
	{
		memset(buf, 0, dev->BlockSize);
		atfs_write32(buf + ATFS_OFFSET_FREE_NEXT, next_next);
		atfs_write32(buf + ATFS_OFFSET_FREE_SIZE, count + next_size);
		PROPAGATE(dev->Write(block, 1, buf));

		PROPAGATE(read_for_modify(dev, prev, buf));
		atfs_write32(buf + ATFS_OFFSET_FREE_NEXT, block);
		PROPAGATE(status = dev->Write(prev, 1, buf));
	}
	else
	{
		memset(buf, 0, dev->BlockSize);
		atfs_write32(buf + ATFS_OFFSET_FREE_NEXT, next);
		atfs_write32(buf + ATFS_OFFSET_FREE_SIZE, count);
		PROPAGATE(dev->Write(block, 1, buf));

		PROPAGATE(read_for_modify(dev, prev, buf));
		atfs_write32(buf + ATFS_OFFSET_FREE_NEXT, block);
		PROPAGATE(dev->Write(prev, 1, buf));
	}

	return ATFS_STATUS_OK;
}

#ifdef ATFS_DEBUG

void atfs_print_free(BlockDevice *dev)
{
	u8 buf[dev->BlockSize];
	u32 cur = ATFS_SECTOR_BOOT;
	u32 i = 0;
	u32 prev = 0;
	u32 next = 1;
	u32 cur_size = 0;
	do
	{
		DeviceStatus status = dev->Read(cur, 1, buf);
		if(status)
		{
			printf("%s\n", dev_status_string(status));
			return;
		}

		next = atfs_read32(buf + ATFS_OFFSET_FREE_NEXT);
		cur_size = atfs_read32(buf + ATFS_OFFSET_FREE_SIZE);

		printf("Free Area %d:\n"
			"Start: %"PRIu32"\n"
			"Size:  %"PRIu32"\n"
			"Prev:  %"PRIu32"\n"
			"Next:  %"PRIu32"\n\n",
				i, cur, cur_size, prev, next);

		prev = cur;
		cur = next;
		++i;
	}
	while(cur);
}

#endif /* ATFS_DEBUG */
