/**
 * @file    atfs.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    29.05.2023
 * @brief   ATFS File System (ATFS = Amazing Technology File System)
 *
 * - All integers are stored as little endian
 * - All files are contiguous
 * - Filename separator is a dot '.' not slash '/'
 * - Filenames may only contain lowercase letters ('a' to 'z'),
 *   numbers ('0' to '9') and underscore ('_') and may not start with
 *   a number. File extensions are not supported.
 *
 * - Example path: "root.images.vacation.me_on_the_beach"
 */

#ifndef __ATFS_H__
#define __ATFS_H__

#include <types.h>

/** Boot sector offset in blocks */
#define ATFS_SECTOR_BOOT            0

/** Boot sector size */
#define ATFS_SIZE_BOOT              1

/** Current FS Revision */
#define ATFS_REVISION               1

/* --- Boot sector byte offsets --- */

/** Offset of signature in ATFS bootsector */
#define ATFS_OFFSET_SIGNATURE       0

/** Offset of revision in ATFS bootsector */
#define ATFS_OFFSET_REVISION        4

/** Offset of init block address in ATFS bootsector */
#define ATFS_OFFSET_INIT_BLOCK      8

/** Offset of init program size in ATFS bootsector */
#define ATFS_OFFSET_INIT_SIZE      12

/** Offset of root block address in ATFS bootsector */
#define ATFS_OFFSET_ROOT_BLOCK     16

/** Offset of root block size in ATFS bootsector */
#define ATFS_OFFSET_ROOT_SIZE      20

/* --- Directories --- */

/** Byte offset for number of directory entries field */
#define ATFS_DIR_OFFSET_NUM_ENTRIES 0

/** Character used to separate directories in a path */
#define ATFS_DIR_SEPARATOR           '.'

/* --- Directory entries --- */

/** Size of a directory entry in bytes */
#define ATFS_DIR_ENTRY_SIZE        64

/** Maximum length of a file name */
#define ATFS_MAX_FILE_NAME_LENGTH  54

/** Byte offset of the starting block */
#define ATFS_DIR_ENTRY_OFFSET_START 0

/** Byte offset of the size in blocks */
#define ATFS_DIR_ENTRY_OFFSET_SIZE  4

/** Byte offset of the entry name */
#define ATFS_DIR_ENTRY_OFFSET_NAME  8

/** Byte offset of the entry type */
#define ATFS_DIR_ENTRY_OFFSET_TYPE 63

/* --- Free List --- */
/** Byte offset of the Pointer to the next free area */
#define ATFS_FREE_OFFSET_NEXT     504

/** Byte offset of the Size of the current free area in blocks */
#define ATFS_FREE_OFFSET_SIZE     508

/** FS Signature (bytes 0-4) */
extern const u8 ATFS_SIGNATURE[4];

#endif /* __ATFS_H__ */
