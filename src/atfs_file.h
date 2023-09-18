#ifndef __ATFS_FILE_H__
#define __ATFS_FILE_H__

#include "types.h"

typedef struct
{

} ATFS_File;

void atfs_read(ATFS_File *file, void *buffer, size_t bytes);
void atfs_write(ATFS_File *file, const void *buffer, size_t bytes);

#endif /* __ATFS_FILE_H__ */
