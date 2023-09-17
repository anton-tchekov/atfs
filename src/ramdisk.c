/**
 * @file    ramdisk.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    28.06.2023
 */

#include "ramdisk.h"
#include <string.h>

#define RAMFS_BLOCK_SIZE   512
#define RAMFS_BLOCK_COUNT  512

static u8 _data[RAMFS_BLOCK_SIZE * RAMFS_BLOCK_COUNT];

static DeviceStatus _ramdisk_read(u32 offset, u32 count, u8 *buffer)
{
	memcpy(buffer, _data + offset * RAMFS_BLOCK_SIZE, count * RAMFS_BLOCK_SIZE);
	return DEVICE_STATUS_OK;
}

static DeviceStatus _ramdisk_write(u32 offset, u32 count, u8 *buffer)
{
	memcpy(_data + offset * RAMFS_BLOCK_SIZE, buffer, count * RAMFS_BLOCK_SIZE);
	return DEVICE_STATUS_OK;
}

BlockDevice ramdisk =
{
	.BlockSize = RAMFS_BLOCK_SIZE,
	.BlockCount = RAMFS_BLOCK_COUNT,
	.Read = _ramdisk_read,
	.Write = _ramdisk_write,
};
