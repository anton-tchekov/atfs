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

#define ATFS_DIR_ENTRY_SIZE        64

#define ATFS_DIR_ENTRY_SIZE_POT     6

/** Maximum number of entries in a directory */
#define ATFS_DIR_MAX_ENTRIES       32

#define ATFS_DIR_ENTRY_OFFSET_START 0

#define ATFS_DIR_ENTRY_OFFSET_SIZE  4

#define ATFS_DIR_ENTRY_OFFSET_TYPE  8

#define ATFS_DIR_ENTRY_OFFSET_NAME  9

/** Maximum length of a file name */
#define ATFS_MAX_FILE_NAME_LENGTH  54

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

#define PROPAGATE(X) do { int a = X; if(a) { return a; } } while(0)

enum
{
	ATFS_STATUS_OK,
	ATFS_STATUS_NO_SUCH_FILE_OR_DIRECTORY = DEVICE_STATUS_COUNT,
	ATFS_STATUS_NO_SPACE_LEFT_ON_DEVICE,
	ATFS_STATUS_PATH_FORMAT_INVALID,
	ATFS_STATUS_DIRECTORY_FULL,
};

typedef int ATFS_Status;

typedef enum
{
	ATFS_TYPE_FREE,
	ATFS_TYPE_DIR,
	ATFS_TYPE_FILE,
} ATFS_FileType;

/** Directory Entry structure */
typedef struct
{
	/** Capacity of the file in blocks */
	u32 CapacityBlocks;

	/** Starting address of the file in blocks */
	u32 StartBlock;

	/** File type */
	u8 Type;

	/** File name */
	char Name[ATFS_MAX_FILE_NAME_LENGTH + 1];
} ATFS_DirEntry;

/** File Handle */
typedef struct
{
	/** Current position pointer */
	u64 Position;

	/** File size in bytes */
	u64 SizeBytes;

	/** Starting block of the file */
	u32 StartBlock;

	/** Capacity of the file in blocks */
	u32 CapacityBlocks;
} ATFS_File;

/** Directory Handle */
typedef struct
{
	/** Every directory is a file */
	ATFS_File InternalFile;
} ATFS_Dir;

/**
 * @brief Returns a human-readable status string for a status code
 *
 * @param status The status code
 * @return Pointer to string constant
 */
const char *atfs_status_string(ATFS_Status status);

/* --- Files --- */
ATFS_Status atfs_fopen(BlockDevice *dev, const char *path, ATFS_File *file);
ATFS_Status atfs_fread(ATFS_File *file, void *buf, size_t bytes);
ATFS_Status atfs_fwrite(ATFS_File *file, const void *buf, size_t bytes);
ATFS_Status atfs_fcreate(BlockDevice *dev, const char *path,
	u64 capacity_bytes);
ATFS_Status atfs_rename(BlockDevice *dev, const char *dst, const char *src);
ATFS_Status atfs_delete(BlockDevice *dev, const char *path);

/* --- Directories --- */
ATFS_Status atfs_dopen(BlockDevice *dev, const char *path, ATFS_Dir *dir);
ATFS_Status fs_dread(ATFS_Dir *dir, ATFS_DirEntry *entry);

#endif /* __ATFS_H__ */
