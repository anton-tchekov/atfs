/**
 * @file    ramdisk.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    22.09.2023
 */

#include "ramdisk.h"
#include <string.h>

#define RAMFS_BLOCK_SIZE   512
#define RAMFS_BLOCK_COUNT  512

static u8 _data[RAMFS_BLOCK_SIZE * RAMFS_BLOCK_COUNT];

static DeviceStatus _check_range(u32 offset, u32 count)
{
	return (offset >= RAMFS_BLOCK_COUNT) ||
		(count >= RAMFS_BLOCK_COUNT) ||
		(offset + count >= RAMFS_BLOCK_COUNT);
}

static DeviceStatus _ramdisk_read(u32 offset, u32 count, u8 *buffer)
{
	if(_check_range(offset, count))
	{
		return DEVICE_STATUS_OUT_OF_BOUNDS;
	}

	memcpy(buffer, _data + offset * RAMFS_BLOCK_SIZE, count * RAMFS_BLOCK_SIZE);
	return DEVICE_STATUS_OK;
}

static DeviceStatus _ramdisk_write(u32 offset, u32 count, const u8 *buffer)
{
	if(_check_range(offset, count))
	{
		return DEVICE_STATUS_OUT_OF_BOUNDS;
	}

	memcpy(_data + offset * RAMFS_BLOCK_SIZE, buffer, count * RAMFS_BLOCK_SIZE);
	return DEVICE_STATUS_OK;
}

BlockDevice ramdisk =
{
	.BlockSize = RAMFS_BLOCK_SIZE,
	.BlockSizePOT = 9,
	.BlockCount = RAMFS_BLOCK_COUNT,
	.Read = _ramdisk_read,
	.Write = _ramdisk_write,
};
