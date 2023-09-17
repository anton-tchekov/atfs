#include "atfs_dir.h"
#include "dev.h"

void read_chunk(BlockDevice *dev)
{
	u8 buffer[dev->BlockSize * NumChunks];

	dev->Read(ChunkAddr, NumChunks, buffer);

	/* Buffer enthält die gelesenen chunks */

	// dev->Write(ChunkAddr, NumChunks, buffer);
}

