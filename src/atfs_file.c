/**
 * @file    atfs_file.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    21.09.2023
 */

#include "atfs_file.h"
#include "atfs_path.h"
#include "atfs_alloc.h"
#include "atfs_util.h"
#include <string.h>

/* TODO: Also too many parameters for my liking */
static ATFS_Status _dir_entry_find(BlockDevice *dev,
	const char *name, size_t name_len,
	u32 block, u32 size,
	ATFS_NamelessDirEntry *entry)
{
	u8 buf[dev->BlockSize];
	u32 end, offset;
	const u8 *cur;
	const char *entry_name;
	ATFS_FileType entry_type;

	for(end = block + size; block < end; ++block)
	{
		dev->Read(block, 1, buf);
		for(offset = 0; offset < dev->BlockSize;
			offset += ATFS_DIR_ENTRY_SIZE)
		{
			cur = buf + offset;
			entry_name = (const char *)(cur + ATFS_DIR_ENTRY_OFFSET_NAME);
			entry_type = cur[ATFS_DIR_ENTRY_OFFSET_TYPE];
			if(entry_type != ATFS_TYPE_FREE &&
				strlen(entry_name) == name_len &&
				!strncmp(name, entry_name, name_len))
			{
				entry->Type = entry_type;
				entry->StartBlock = atfs_read32(
					cur + ATFS_DIR_ENTRY_OFFSET_START);
				entry->SizeBlocks = atfs_read32(
					cur + ATFS_DIR_ENTRY_OFFSET_SIZE);
				return ATFS_STATUS_OK;
			}
		}
	}

	return ATFS_STATUS_NO_SUCH_FILE_OR_DIRECTORY;
}

ATFS_Status _dir_entry_insert(BlockDevice *dev, u32 block,
	ATFS_DirEntry *entry);

static int _file_check_bounds(u32 start, u32 count, u32 capacity)
{
	/* TODO: Implement this without a sneaky integer overflow bug */
	return 0;
	(void)start;
	(void)count;
	(void)capacity;
}

static void dir_entry_init(
	ATFS_DirEntry *entry, u32 start, u32 size, ATFS_FileType type,
	const char *name)
{
	entry->Type = type;
	entry->StartBlock = start;
	entry->SizeBlocks = size;
	strcpy(entry->Name, name);
}

static ATFS_Status check_path(const char *path)
{
	return atfs_path_valid(path) ? ATFS_STATUS_OK :
		ATFS_STATUS_PATH_FORMAT_INVALID;
}

static ATFS_Status atfs_traverse(BlockDevice *dev, const char *path,
	u32 *parent, u32 *size, const char **last, const char **end)
{
	u8 buf[dev->BlockSize];
	ATFS_NamelessDirEntry entry;
	const char *name;
	int c;

	PROPAGATE(dev->Read(0, 1, buf));
	entry.StartBlock = atfs_read32(buf + ATFS_OFFSET_ROOT_BLOCK);
	entry.SizeBlocks = atfs_read32(buf + ATFS_OFFSET_ROOT_SIZE);
	for(name = path; (c = *path); ++path)
	{
		if(c == ATFS_DIR_SEPARATOR)
		{
			PROPAGATE(_dir_entry_find(dev, name, path - name,
				entry.StartBlock, entry.SizeBlocks, &entry));

			if(entry.Type != ATFS_TYPE_DIR)
			{
				return ATFS_STATUS_NO_SUCH_FILE_OR_DIRECTORY;
			}

			name = path + 1;
		}
	}

	/* TODO: This is way to many pointer parameters */
	*parent = entry.StartBlock;
	*size = entry.SizeBlocks;
	*last = name;
	*end = path;
	return ATFS_STATUS_OK;
}

ATFS_Status atfs_fcreate(BlockDevice *dev, const char *path,
	ATFS_FileType type, u32 size)
{
	const char *last, *end;
	u32 start, parent, parent_size;
	ATFS_DirEntry entry;

	PROPAGATE(check_path(path));
	PROPAGATE(atfs_traverse(dev, path, &parent, &parent_size, &last, &end));
	PROPAGATE(atfs_alloc(dev, size, &start));
	dir_entry_init(&entry, start, size, type, last);

	/* TODO BUG: This leaks allocated space for file if insert fails */
	PROPAGATE(_dir_entry_insert(dev, parent, &entry));
	return ATFS_STATUS_OK;
}

ATFS_Status atfs_fopen(BlockDevice *dev, const char *path, ATFS_File *file)
{
	ATFS_NamelessDirEntry entry;
	const char *name, *end;

	PROPAGATE(check_path(path));
	PROPAGATE(atfs_traverse(dev, path, &entry.StartBlock, &entry.SizeBlocks,
		&name, &end));

	if(*name != '\0')
	{
		PROPAGATE(_dir_entry_find(dev, name, end - name,
			entry.StartBlock, entry.SizeBlocks, &entry));
	}

	file->Device = dev;
	file->StartBlock = entry.StartBlock;
	file->SizeBlocks = entry.SizeBlocks;
	return ATFS_STATUS_OK;
}

ATFS_Status atfs_fread(ATFS_File *file, u32 block, u32 count, void *buf)
{
	if(_file_check_bounds(block, count, file->SizeBlocks))
	{
		return ATFS_STATUS_OUT_OF_BOUNDS;
	}

	return file->Device->Read(file->StartBlock + block, count, buf);
}

ATFS_Status atfs_fwrite(ATFS_File *file, u32 block, u32 count, const void *buf)
{
	if(_file_check_bounds(block, count, file->SizeBlocks))
	{
		return ATFS_STATUS_OUT_OF_BOUNDS;
	}

	return file->Device->Write(file->StartBlock + block, count, buf);
}
