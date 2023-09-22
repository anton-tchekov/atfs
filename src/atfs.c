/**
 * @file    atfs.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    18.09.2023
 */

#include "atfs.h"
#include "atfs_util.h"
#include "atfs_alloc.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>

const u8 ATFS_SIGNATURE[4] = { 'A', 'T', 'F', 'S' };

/* --- PRIVATE --- */
ATFS_Status _dir_entry_delete(BlockDevice *dev,
	const char *name, u32 block, u32 size)
{
	u8 buf[dev->BlockSize];
	u32 end, offset;
	for(end = block + size; block < end; ++block)
	{
		dev->Read(block, 1, buf);
		for(offset = 0; offset < dev->BlockSize;
			offset += ATFS_DIR_ENTRY_SIZE)
		{
			char *entry_name =
				(char *)(buf + offset + ATFS_DIR_ENTRY_OFFSET_NAME);
			if(!strcmp(entry_name, name))
			{
				// atfs_free(dev, entry_start, entry_size);
				memset(entry_name, 0, ATFS_DIR_ENTRY_SIZE);
				dev->Write(block, 1, buf);
				return ATFS_STATUS_OK;
			}
		}
	}

	return ATFS_STATUS_NO_SUCH_FILE_OR_DIRECTORY;
}

static void _dir_entry_write(u8 *buf, ATFS_DirEntry *entry)
{
	atfs_write32(buf + ATFS_DIR_ENTRY_OFFSET_START, entry->StartBlock);
	atfs_write32(buf + ATFS_DIR_ENTRY_OFFSET_SIZE, entry->SizeBlocks);
	buf[ATFS_DIR_ENTRY_OFFSET_TYPE] = entry->Type;
	strncpy((char *)(buf + ATFS_DIR_ENTRY_OFFSET_NAME), entry->Name,
		ATFS_MAX_FILE_NAME_LENGTH + 1);
}

static void _dir_entry_get(u8 *buf, ATFS_DirEntry *entry)
{
	entry->Type = buf[ATFS_DIR_ENTRY_OFFSET_TYPE];
	entry->StartBlock = atfs_read32(buf + ATFS_DIR_ENTRY_OFFSET_START);
	entry->SizeBlocks = atfs_read32(buf + ATFS_DIR_ENTRY_OFFSET_SIZE);
	strncpy(entry->Name, (char *)(buf + ATFS_DIR_ENTRY_OFFSET_NAME),
		ATFS_MAX_FILE_NAME_LENGTH + 1);
}

ATFS_Status _dir_entry_insert(BlockDevice *dev, u32 block,
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
			if(insert_index == ATFS_DIR_MAX_ENTRIES)
			{
				insert_index = i;
			}
		}
		else if(!strcmp(entry_name, entry->Name))
		{
			insert_index = i;
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
	offset = (insert_index & ((1 << i) - 1)) << ATFS_DIR_ENTRY_SIZE_POT;

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
		"Out of bounds file access",
		"End of directory reached",
		"Not implemented",
	};

	if(status < DEVICE_STATUS_COUNT)
	{
		return dev_status_string(status);
	}

	status -= DEVICE_STATUS_COUNT;
	assert(status >= 0 && status < (int)ARRLEN(status_str));
	return status_str[status];
}

ATFS_Status atfs_dopen(BlockDevice *dev, const char *path, ATFS_Dir *dir)
{
	dir->Block = 0;
	dir->Offset = 0;
	return atfs_fopen(dev, path, &dir->InternalFile);
}

ATFS_Status atfs_dread(ATFS_Dir *dir, ATFS_DirEntry *entry)
{
	BlockDevice *dev = dir->InternalFile.Device;
	u8 buf[dev->BlockSize];

	if(dir->Block >= dir->InternalFile.SizeBlocks)
	{
		return ATFS_STATUS_DIR_END;
	}

	PROPAGATE(atfs_fread(&dir->InternalFile, dir->Block, 1, buf));
	for(;;)
	{
		if(buf[dir->Offset + ATFS_DIR_ENTRY_OFFSET_TYPE] != ATFS_TYPE_FREE)
		{
			_dir_entry_get(buf + dir->Offset, entry);
			dir->Offset += ATFS_DIR_ENTRY_SIZE;
			if(dir->Offset >= dev->BlockSize)
			{
				dir->Offset = 0;
				++dir->Block;
			}

			return ATFS_STATUS_OK;
		}

		dir->Offset += ATFS_DIR_ENTRY_SIZE;
		if(dir->Offset >= dev->BlockSize)
		{
			dir->Offset = 0;
			++dir->Block;
			if(dir->Block >= dir->InternalFile.SizeBlocks)
			{
				return ATFS_STATUS_DIR_END;
			}

			PROPAGATE(atfs_fread(&dir->InternalFile, dir->Block, 1, buf));
		}
	}
}
