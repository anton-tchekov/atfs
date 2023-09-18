/**
 * @file    atfs.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    29.05.2023
 * @brief   ATFS File System
 */

#ifndef __ATFS_H__
#define __ATFS_H__

#include "types.h"
#include "dev.h"

/** Size of the root directory in blocks */
#define ATFS_INITIAL_ROOT_SIZE      4

/** Boot sector offset in blocks */
#define ATFS_SECTOR_BOOT            0

/** Boot sector size */
#define ATFS_SIZE_BOOT              1

/** Current FS Revision */
#define ATFS_REVISION               1

/* --- Boot sector --- */

/** FS Signature (bytes 0-4) */
extern const u8 ATFS_SIGNATURE[4];

/** Offset of signature in ATFS bootsector */
#define ATFS_OFFSET_SIGNATURE       0

/** Offset of revision in ATFS bootsector */
#define ATFS_OFFSET_REVISION        4

/** Byte offset of the Pointer to the next free area */
#define ATFS_OFFSET_FREE_NEXT       8

/** Byte offset of the Size of the current free area in blocks */
#define ATFS_OFFSET_FREE_SIZE      12

/** Offset of root block address in ATFS bootsector */
#define ATFS_OFFSET_ROOT_BLOCK     16

/** Offset of root block size in ATFS bootsector */
#define ATFS_OFFSET_ROOT_SIZE      20

enum
{
	ATFS_STATUS_OK,
	ATFS_STATUS_NO_SUCH_FILE_OR_DIRECTORY = DEVICE_STATUS_COUNT,
	ATFS_STATUS_NO_SPACE_LEFT_ON_DEVICE,
};

typedef int ATFS_Status;

const char *atfs_status_string(ATFS_Status status);

#endif /* __ATFS_H__ */
