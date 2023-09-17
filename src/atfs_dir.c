#include "atfs_dir.h"


void read_chunk(BlockDevice *dev)
{
	#if 0
	u8 buffer[dev->BlockSize * NumChunks];

	dev->Read(ChunkAddr, NumChunks, buffer);

	/* Buffer enthält die gelesenen chunks */

	// dev->Write(ChunkAddr, NumChunks, buffer);

	#endif
}

void create_directory_entry(BlockDevice *dev, u32 chunkAddr, u32 sizeInChunks, DirEntry entry) {
    u8 buff[dev->BlockSize*sizeInChunks];
    dev->Read(chunkAddr, sizeInChunks, buff);

    DirEntry *entries = (DirEntry*)buff;
    int firstFree = 0;
    for (int i = 0; i < (dev->BlockSize*sizeInChunks) / sizeof(DirEntry); ++i) {
        if(entries[i].type == 0){
            firstFree = i;
            break;
        }
    }

    entries[firstFree] = entry;

    dev->Write(chunkAddr, sizeInChunks, buff);
}

DirEntry get_adress_by_path(BlockDevice *dev, char* path, DirEntry currDir) {
    printf("CHECK: \n");

    if(path[0] == '/'){
        ++path;
    }

    char* ptrToSlash = strchr(path, '/');
    printf("First Slash to: %s\n", ptrToSlash);

    size_t nameLength = ptrToSlash ? ptrToSlash - path : strlen(path);

    printf("Length Name: %d\n", nameLength);
    if(nameLength == 0) {
        return currDir;
    }

    u8 buff[dev->BlockSize*currDir.cSize];
    dev->Read(currDir.addr, currDir.cSize, buff);

    DirEntry *entries = (DirEntry*)buff;

    for (int i = 0; i < (dev->BlockSize*currDir.cSize) / sizeof(DirEntry); ++i) {
        if(strncmp(entries[i].name, path, nameLength) == 0){
            if(entries[i].type == 1) {
                if(ptrToSlash) {
                    return get_adress_by_path(dev, ptrToSlash + 1, entries[i]);
                }
                else {
                    return entries[i];
                }
            } else {
                return entries[i];
            }
        }
    }

}