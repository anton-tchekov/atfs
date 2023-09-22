/**
 * @file    dev.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    28.06.2023
 * @brief   Block device interface
 */

#ifndef __DEV_H__
#define __DEV_H__

#include "types.h"

/** Device status code */
enum
{
	DEVICE_STATUS_OK,
	DEVICE_STATUS_FAILURE,
	DEVICE_STATUS_OUT_OF_BOUNDS,
	DEVICE_STATUS_COUNT,
};

typedef int DeviceStatus;

/** Block device interface struct */
typedef struct
{
	/** Size of each block in bytes */
	u32 BlockSize;

	/** Size of each block as a power of two */
	u32 BlockSizePOT;

	/** Number of blocks in device */
	u32 BlockCount;

	/** Multi-block read operation */
	DeviceStatus (*Read)(u32 offset, u32 count, u8 *buffer);

	/** Multi-block write operation */
	DeviceStatus (*Write)(u32 offset, u32 count, const u8 *buffer);
} BlockDevice;

/**
 * @brief Returns a human-readable status string for a status code
 *
 * @param status The status code
 * @return Pointer to string constant
 */
const char *dev_status_string(DeviceStatus status);

/**
 * @brief Prints the hexdump a single device block to stdio
 *
 * @param dev Block device
 * @param block Block number
 */
void dev_print_block(BlockDevice *dev, u32 block);

#endif /* __DEV_H__ */
