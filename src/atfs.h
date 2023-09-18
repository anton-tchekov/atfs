/**
 * @file    atfs.h
 * @author  Tim Gabrikowski, Anton Tchekov
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

#define PROPAGATE(X) do { int a = X; if(a) { return a; } } while(0)

enum
{
	ATFS_STATUS_OK,
	ATFS_STATUS_NO_SUCH_FILE_OR_DIRECTORY = DEVICE_STATUS_COUNT,
	ATFS_STATUS_NO_SPACE_LEFT_ON_DEVICE,
};

typedef int ATFS_Status;

const char *atfs_status_string(ATFS_Status status);

#if 0


/*
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

#include <status.h>
#include <types.h>
#include <atfs.h>


/* --- Directory entries --- */

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


/** File Handle */
typedef struct
{
	/** Starting block of the file */
	u32 StartBlock;

	/** Size of the file in blocks */
	u32 SizeBlocks;
} File;

/** Directory Handle */
typedef struct
{
	/** Number of entries in the directory */
	u32 NumEntries;

	/** Every directory is a file */
	File InternalFile;
} Dir;

/**
 * Directory entry: 64 bytes
 * - 4 byte address
 * - 4 byte size
 * - 54 byte file name
 * - 1 byte null terminator
 * - 1 byte type (directory = 0, text file = non-zero)
 */
typedef struct
{
	/** Starting block of the file */
	u32 StartBlock;

	/** Size of the file in blocks */
	u32 SizeBlocks;

	/** Name of the file */
	char Name[ATFS_MAX_FILE_NAME_LENGTH + 1];

	/** Directory/File type */
	u8 Type;
} DirEntry;


/**
 * @brief Format disk with ATFS
 *
 * @param disk Drive number
 */
Status fs_format(u32 disk);

/**
 * @brief Mount disk with ATFS
 *
 * @param disk Drive number
 * @param ident Identifier
 */
Status fs_mount(u32 disk, const char *ident);

/**
 * @brief Create a file. If the file already exists, it is resized to
 *        the specified size. When a file is resized, all file handles to it
 *        are invalidated and need to be reopened.
 *
 * @param path Path to file
 * @param size Capacity of the file in blocks
 * @return Status
 */
Status fs_fcreate(const char *path, u32 size);

/**
 * @brief Open a file to get a file handle. It is not neccessary to close
 *        a file, therefore there is no `fs_fclose` function.
 *
 * @param path Path to file
 * @param file Output parameter file handle
 * @return Status
 */
Status fs_fopen(const char *path, File *file);

/**
 * @brief Reads one block from the file.
 *
 * @param file File handle
 * @param block The block to read
 * @param buf Buffer for read data (512 bytes)
 * @return Status
 */
Status fs_fread(File *file, u32 block, void *buf);

/**
 * @brief Write one block to the file
 *
 * @param file File handle
 * @param block The block to write
 * @param buf Buffer with data to write (512 bytes)
 * @return Status
 */
Status fs_fwrite(File *file, u32 block, const void *buf);

/**
 * @brief Recursively delete a files and directories
 *
 * @param path Path to file
 * @return Status
 */
Status fs_delete(const char *path);

/**
 * @brief Move or rename a file or a directory.
 *
 * @param dest Destination path
 * @param src Source path
 * @return Status
 */
Status fs_rename(const char *dest, const char *src);

/**
 * @brief Recursively copy files and directories.
 *
 * @param dest Destination path
 * @param src Source path
 * @return Status
 */
Status fs_copy(const char *dest, const char *src);

/**
 * @brief Open a directory
 *
 * @param path Path to directory
 * @param dir Output parameter directory handle
 * @return Status
 */
Status fs_dopen(const char *path, Dir *dir);

/**
 * @brief Read a directory entry
 *
 * @param dir Directory handle
 * @param index File index
 * @param entry Output parameter directory entry
 * @return Status
 */
Status fs_dread(Dir *dir, u32 index, DirEntry *entry);

#endif



#endif /* __ATFS_H__ */
