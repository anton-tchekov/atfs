/**
 * @file    atfs.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    18.09.2023
 */

#include "atfs.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>

const u8 ATFS_SIGNATURE[4] = { 'A', 'T', 'F', 'S' };

/* --- PRIVATE --- */
static ATFS_Status _dir_entry_find(
	const char *name, size_t name_len, u32 block, ATFS_DirEntry *entry)
{
#if 0
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
#endif

	return ATFS_STATUS_OK;
}

static void _dir_entry_get(u8 *buf, ATFS_DirEntry *entry)
{
#if 0
	entry->StartBlock = load_le_32(buf + ATFS_DIR_ENTRY_OFFSET_START);
	entry->SizeBlocks = load_le_32(buf + ATFS_DIR_ENTRY_OFFSET_SIZE);
	memcpy(entry->Name, buf + ATFS_DIR_ENTRY_OFFSET_NAME,
		ATFS_MAX_FILE_NAME_LENGTH + 1);
	entry->Type = buf[ATFS_DIR_ENTRY_OFFSET_TYPE];
#endif
}

/* --- PUBLIC --- */
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

ATFS_Status atfs_fread(ATFS_File *file, void *buf, size_t bytes)
{
#if 0
	if(block >= file->SizeBlocks)
	{
		return STATUS_OUT_OF_BOUNDS;
	}

	return disk_read(file->StartBlock + block, buf);
#endif

	return ATFS_STATUS_OK;
}

ATFS_Status atfs_fwrite(ATFS_File *file, const void *buf, size_t bytes)
{
#if 0
	if(block >= file->SizeBlocks)
	{
		return STATUS_OUT_OF_BOUNDS;
	}

	return disk_write(file->StartBlock + block, buf);
#endif

	return ATFS_STATUS_OK;
}

ATFS_Status atfs_fcreate(BlockDevice *dev, const char *path,
	u64 capacity_bytes)
{
#if 0
	Dir dir;
	DirEntry entry;
	u32 start;

	if(!_traverse(path, &dir))
	{
		return;
	}

	RETURN_IF(fs_alloc(capacity, &start));
#endif

	return ATFS_STATUS_OK;
}

ATFS_Status atfs_fopen(BlockDevice *dev, const char *path, ATFS_File *file)
{
#if 0
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
#endif

	return ATFS_STATUS_OK;
}

ATFS_Status atfs_dopen(BlockDevice *dev, const char *path, ATFS_Dir *dir)
{
	return atfs_fopen(dev, path, &dir->InternalFile);
}

ATFS_Status fs_dread(ATFS_Dir *dir, ATFS_DirEntry *entry)
{
#if 0
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
#endif

	return ATFS_STATUS_OK;
}
