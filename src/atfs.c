/**
 * @file    atfs.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    18.09.2023
 */

#include "atfs.h"
#include <assert.h>

const u8 ATFS_SIGNATURE[4] = { 'A', 'T', 'F', 'S' };

const char *atfs_status_string(ATFS_Status status)
{
	static const char *status_str[] =
	{
		"No such file or directory",
		"No space left on device",
	};

	if(status < DEVICE_STATUS_COUNT)
	{
		return dev_status_string(status);
	}

	status -= DEVICE_STATUS_COUNT;
	assert(status >= 0 && status < (int)ARRLEN(status_str));
	return status_str[status];
}

#if 0

#include <ctype.h>
#include <string.h>

/**
 * @brief Read directory entry in a endianness-independant way
 *
 * @param buf Data buffer
 * @param entry Directory entry output
 */
static void dir_entry_get(void *buf, DirEntry *entry)
{
	u8 *buf8 = buf;
	entry->StartBlock = load_le_32(buf8 + ATFS_DIR_ENTRY_OFFSET_START);
	entry->SizeBlocks = load_le_32(buf8 + ATFS_DIR_ENTRY_OFFSET_SIZE);
	memcpy(entry->Name, buf8 + ATFS_DIR_ENTRY_OFFSET_NAME,
		ATFS_MAX_FILE_NAME_LENGTH + 1);
	entry->Type = buf8[ATFS_DIR_ENTRY_OFFSET_TYPE];
}

/**
 * @brief Find a directory entry in a directory
 *
 * @param name
 * @param name_len
 * @param block
 * @param entry
 * @return Status
 */
static Status _find_entry(
	const char *name, u32 name_len, u32 block, DirEntry *entry)
{
	u8 buf[ATFS_BLOCK_SIZE];
	u32 i, num_entries, offset;

	RETURN_IF(disk_read(block, buf));
	num_entries = load_le_32(buf + ATFS_DIR_OFFSET_NUM_ENTRIES);
	offset = ATFS_DIR_ENTRY_SIZE;
	for(i = 0; i < num_entries; ++i)
	{
		if(offset == ATFS_BLOCK_SIZE)
		{
			offset = 0;
			++block;
			RETURN_IF(disk_read(block, buf));
		}

		dir_entry_get(buf + offset, entry);

		/* Compare name with each directory entry */
		if(strlen(entry->Name) == name_len &&
			!strncmp(name, entry->Name, name_len))
		{
			return STATUS_OK;
		}

		offset += ATFS_DIR_ENTRY_SIZE;
	}

	return STATUS_NO_SUCH_FILE_OR_DIR;
}

/* --- PUBLIC --- */

Status fs_fcreate(const char *path, u32 size)
{
	#ifdef AAA
	Dir dir;
	DirEntry entry;
	if(_traverse(path, &dir))
	{
		u32 start;

		/* New file */
		RETURN_IF(fs_alloc(size, &start));
	}
	else if(file.SizeBlocks != size)
	{
		/* TODO: Resize file */
	}
	#endif

	return STATUS_OK;
}

bool fs_exists(const char *path)
{
	File f;
	return fs_fopen(path, &f);
}

Status fs_fopen(const char *path, File *file)
{
	u8 buf[ATFS_BLOCK_SIZE];
	DirEntry entry;
	const char *s, *name;
	char c;
	u32 name_len;

	/* Check path format beforehand for simplicity */
	if(!path_valid(path))
	{
		return STATUS_PATH_FORMAT_INVALID;
	}

	/* Traverse directory structure starting in root directory */
	RETURN_IF(disk_read(ATFS_SECTOR_BOOT, buf));
	entry.StartBlock = load_le_32(buf + ATFS_OFFSET_ROOT_BLOCK);
	entry.SizeBlocks = load_le_32(buf + ATFS_OFFSET_ROOT_SIZE);
	s = path;
	while(*s)
	{
		/* Get next part of path string */
		name = s;
		for(; (c = *s) && c != ATFS_DIR_SEPARATOR; ++s) {}
		name_len = s - name;

		/* Search current directory */
		RETURN_IF(_find_entry(name, name_len, entry.StartBlock, &entry));

		/* If in the middle of a path */
		if(!*s)
		{
			/* Skip directory separator */
			++s;

			/* Found entry must be a directory */
			if(entry.Type)
			{
				return STATUS_NO_SUCH_FILE_OR_DIR;
			}
		}
	}

	/* Entry was found */
	file->StartBlock = entry.StartBlock;
	file->SizeBlocks = entry.SizeBlocks;
	return STATUS_OK;
}

Status fs_fread(File *file, u32 block, void *buf)
{
	if(block >= file->SizeBlocks)
	{
		return STATUS_OUT_OF_BOUNDS;
	}

	return disk_read(file->StartBlock + block, buf);
}

Status fs_fwrite(File *file, u32 block, const void *buf)
{
	if(block >= file->SizeBlocks)
	{
		return STATUS_OUT_OF_BOUNDS;
	}

	return disk_write(file->StartBlock + block, buf);
}

Status fs_delete(const char *path)
{
	return STATUS_OK;
	(void)path;
}

Status fs_rename(const char *dest, const char *src)
{
	/* Create new directory entries */

	/* Delete old directory entries */

	return STATUS_OK;
	(void)dest, (void)src;
}

Status fs_dopen(const char *path, Dir *dir)
{
	u8 buf[ATFS_BLOCK_SIZE];

	RETURN_IF(fs_fopen(path, &dir->InternalFile));
	RETURN_IF(fs_fread(&dir->InternalFile, 0, buf));
	dir->NumEntries = load_le_32(buf);
	return STATUS_OK;
}

Status fs_dread(Dir *dir, u32 index, DirEntry *entry)
{
	u8 buf[ATFS_BLOCK_SIZE];
	u32 offset, block;

	if(index >= dir->NumEntries)
	{
		return STATUS_OUT_OF_BOUNDS;
	}

	/* Add one because the first directory entry is reserved for metadata */
	offset = (index + 1) * ATFS_DIR_ENTRY_SIZE;
	block = offset / ATFS_BLOCK_SIZE;
	offset -= block * ATFS_BLOCK_SIZE;
	RETURN_IF(fs_fread(&dir->InternalFile, block, buf));
	dir_entry_get(buf + offset, entry);
	return STATUS_OK;
}
#endif

