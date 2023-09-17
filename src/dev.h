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

typedef enum
{
	DEVICE_STATUS_OK,
	DEVICE_STATUS_FAILURE,
} DeviceStatus;

/** Block device interface struct */
typedef struct
{
	/** Size of each block in bytes */
	u32 BlockSize;

	/** Number of blocks in device */
	u32 BlockCount;

	/** Multi-block read operation */
	DeviceStatus (*Read)(u32 offset, u32 count, u8 *buffer);

	/** Multi-block write operation */
	DeviceStatus (*Write)(u32 offset, u32 count, u8 *buffer);
} BlockDevice;

#endif /* __DEV_H__ */
