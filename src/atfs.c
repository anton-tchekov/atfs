/**
 * @file    atfs.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    18.09.2023
 */

#include "atfs.h"
#include "atfs_util.h"
#include "atfs_path.h"
#include "atfs_alloc.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>

const u8 ATFS_SIGNATURE[4] = { 'A', 'T', 'F', 'S' };

/* --- PRIVATE --- */
typedef struct
{
	u32 Type;
	u32 CapacityBlocks;
	u32 StartBlock;
} ATFS_NamelessDirEntry;

static ATFS_Status _dir_entry_find(BlockDevice *dev,
	const char *name, size_t name_len, u32 block, ATFS_NamelessDirEntry *entry)
{
	u8 buf[dev->BlockSize];
	u32 i, offset, entry_type;
	const char *entry_name;

	offset = dev->BlockSize;
	for(i = 0; i < ATFS_DIR_MAX_ENTRIES; ++i)
	{
		if(offset == dev->BlockSize)
		{
			offset = 0;
			PROPAGATE(dev->Read(block, 1, buf));
			++block;
		}

		entry_name = (const char *)(entry + ATFS_DIR_ENTRY_OFFSET_NAME);
		entry_type = buf[offset + ATFS_DIR_ENTRY_OFFSET_TYPE];
		if(entry_type != ATFS_TYPE_FREE &&
			strlen(entry_name) == name_len &&
			!strncmp(name, entry_name, name_len))
		{
			entry->Type = entry_type;
			entry->StartBlock =
				atfs_read32(buf + offset + ATFS_DIR_ENTRY_OFFSET_START);
			entry->CapacityBlocks =
				atfs_read32(buf + offset + ATFS_DIR_ENTRY_OFFSET_SIZE);
			return ATFS_STATUS_OK;
		}

		offset += ATFS_DIR_ENTRY_SIZE;
	}

	return ATFS_STATUS_NO_SUCH_FILE_OR_DIRECTORY;
}

static void _dir_entry_write(u8 *buf, ATFS_DirEntry *entry)
{
	buf[ATFS_DIR_ENTRY_OFFSET_TYPE] = entry->Type;
	atfs_write32(buf + ATFS_DIR_ENTRY_OFFSET_START, entry->StartBlock);
	atfs_write32(buf + ATFS_DIR_ENTRY_OFFSET_SIZE, entry->SizeBlocks);
	strcpy(buf + ATFS_DIR_ENTRY_OFFSET_NAME, entry->Name);
}

static ATFS_Status _dir_entry_insert(BlockDevice *dev, u32 block,
	ATFS_DirEntry *entry)
{
	u8 buf[dev->BlockSize];
	u32 i, cur, offset, insert_index, entry_type;
	const char *entry_name;

	insert_index = ATFS_DIR_MAX_ENTRIES;
	offset = dev->BlockSize;
	cur = block;
	for(i = 0; i < ATFS_DIR_MAX_ENTRIES; ++i)
	{
		if(offset == dev->BlockSize)
		{
			offset = 0;
			PROPAGATE(dev->Read(cur, 1, buf));
			++cur;
		}

		entry_type = buf[offset + ATFS_DIR_ENTRY_OFFSET_TYPE];
		entry_name = (const char *)(entry + ATFS_DIR_ENTRY_OFFSET_NAME);
		if(entry_type == ATFS_TYPE_FREE)
		{
			insert_index = offset;
		}
		else if(!strcmp(entry_name, entry->Name))
		{
			insert_index = offset;
			break;
		}

		offset += ATFS_DIR_ENTRY_SIZE;
	}

	if(insert_index == ATFS_DIR_MAX_ENTRIES)
	{
		return ATFS_STATUS_DIRECTORY_FULL;
	}

	i = dev->BlockSizePOT - ATFS_DIR_ENTRY_SIZE_POT;
	cur = block + (insert_index >> i);
	offset = insert_index & ((1 << (i + 1)) - 1);

	PROPAGATE(dev->Read(cur, 1, buf));
	_dir_entry_write(buf + offset, entry);
	PROPAGATE(dev->Write(cur, 1, buf));
	return ATFS_STATUS_OK;
}

/* --- PUBLIC --- */
const char *atfs_status_string(ATFS_Status status)
{
	static const char *status_str[] =
	{
		"No such file or directory",
		"No space left on device",
		"Invalid path format",
		"Directory full",
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
	u32 capacity, start;

	/* Check path format beforehand for simplicity */
	if(!atfs_path_valid(path))
	{
		return ATFS_STATUS_PATH_FORMAT_INVALID;
	}

	capacity = capacity_bytes >> dev->BlockSizePOT;

#if 0
	Dir dir;
	DirEntry entry;

	if(!_traverse(path, &dir))
	{
		return;
	}

#endif

	PROPAGATE(atfs_alloc(dev, capacity, &start));



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
