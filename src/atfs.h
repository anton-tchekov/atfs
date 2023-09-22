/**
 * @file    atfs.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    18.09.2023
 * @brief   ATFS File System
 *
 * File System Properties:
 * - All integers are stored as little endian
 * - All files are contiguous
 * - Filename separator is a dot '.' not slash '/'
 * - Filenames may only contain lowercase letters ('a' to 'z'),
 *   numbers ('0' to '9') and underscore ('_') and may not start with
 *   a number. File extensions are not supported.
 *   Example path: "home.anton.images.vacation.beach"
 */

#ifndef __ATFS_H__
#define __ATFS_H__

#include "types.h"
#include "dev.h"

/** Delimiter character between path components */
#define ATFS_DIR_SEPARATOR           '.'

/** Size of the root directory in blocks */
#define ATFS_INITIAL_ROOT_SIZE      4

/** Boot sector offset in blocks */
#define ATFS_SECTOR_BOOT            0

/** Boot sector size */
#define ATFS_SIZE_BOOT              1

/** Current FS Revision */
#define ATFS_REVISION               1

/* --- Directory entries --- */

/** Size of a directory entry in bytes as a power of two */
#define ATFS_DIR_ENTRY_SIZE_POT     6

/** Size of a directory entry in bytes */
#define ATFS_DIR_ENTRY_SIZE          (1 << ATFS_DIR_ENTRY_SIZE_POT)

/** Maximum number of entries in a directory */
#define ATFS_DIR_MAX_ENTRIES       32

/** Byte offset of the file starting block number in a directory entry */
#define ATFS_DIR_ENTRY_OFFSET_START 0

/** Byte offset of the file size in blocks in a directory entry */
#define ATFS_DIR_ENTRY_OFFSET_SIZE  4

/** Byte offset of the file type in a directory entry */
#define ATFS_DIR_ENTRY_OFFSET_TYPE  8

/** Byte offset of the file name in a directory entry */
#define ATFS_DIR_ENTRY_OFFSET_NAME  9

/** Maximum length of a file name */
#define ATFS_MAX_FILE_NAME_LENGTH  54

/* --- Boot block --- */

/** FS Signature (bytes 0-4) */
extern const u8 ATFS_SIGNATURE[4];

/** Offset of signature in ATFS boot block */
#define ATFS_OFFSET_SIGNATURE       0

/** Offset of revision in ATFS boot block */
#define ATFS_OFFSET_REVISION        4

/** Byte offset of the Pointer to the next free area */
#define ATFS_OFFSET_FREE_NEXT       8

/** Byte offset of the Size of the current free area in blocks */
#define ATFS_OFFSET_FREE_SIZE      12

/** Offset of root block address in ATFS boot block */
#define ATFS_OFFSET_ROOT_BLOCK     16

/** Offset of root block size in ATFS boot block */
#define ATFS_OFFSET_ROOT_SIZE      20

/** Error propagation macro */
#define PROPAGATE(X) do { int a = X; if(a) { return a; } } while(0)

/** ATFS status code enum */
enum
{
	ATFS_STATUS_OK,
	ATFS_STATUS_NO_SUCH_FILE_OR_DIRECTORY = DEVICE_STATUS_COUNT,
	ATFS_STATUS_NO_SPACE_LEFT_ON_DEVICE,
	ATFS_STATUS_PATH_FORMAT_INVALID,
	ATFS_STATUS_DIRECTORY_FULL,
	ATFS_STATUS_OUT_OF_BOUNDS,
	ATFS_STATUS_DIR_END,
	ATFS_STATUS_NOT_IMPLEMENTED,
};

typedef int ATFS_Status;

/** ATFS File type enum */
typedef enum
{
	ATFS_TYPE_FREE,
	ATFS_TYPE_DIR,
	ATFS_TYPE_FILE,
} ATFS_FileType;

/** Directory Entry struct */
typedef struct
{
	/** Size of the file in blocks */
	u32 SizeBlocks;

	/** Starting address of the file in blocks */
	u32 StartBlock;

	/** File type */
	u8 Type;

	/** File name */
	char Name[ATFS_MAX_FILE_NAME_LENGTH + 1];
} ATFS_DirEntry;

/** Directory entry struct without the file name field */
typedef struct
{
	/** Size of the file in blocks */
	u32 SizeBlocks;

	/** Starting address of the file in blocks */
	u32 StartBlock;

	/** File type */
	u8 Type;
} ATFS_NamelessDirEntry;

/** File Handle */
typedef struct
{
	/* Device on which the file is located */
	BlockDevice *Device;

	/** Starting block of the file */
	u32 StartBlock;

	/** Capacity of the file in blocks */
	u32 SizeBlocks;
} ATFS_File;

/** Directory Handle */
typedef struct
{
	/** Every directory is a file */
	ATFS_File InternalFile;

	/* Current directory block */
	u32 Block;

	/* Current directory offset within block */
	u32 Offset;
} ATFS_Dir;

/**
 * @brief Returns a human-readable status string for a status code
 *
 * @param status The status code
 * @return Pointer to string constant
 */
const char *atfs_status_string(ATFS_Status status);

/* --- Directories --- */
ATFS_Status atfs_dopen(BlockDevice *dev, const char *path, ATFS_Dir *dir);
ATFS_Status atfs_dread(ATFS_Dir *dir, ATFS_DirEntry *entry);

#endif /* __ATFS_H__ */
