#include "dev.h"
#include <inttypes.h>
#include <stdio.h>
#include <assert.h>

void dev_print_block(BlockDevice *dev, u32 block)
{
	u8 buffer[dev->BlockSize];

	if(dev->Read(block, 1, buffer) != DEVICE_STATUS_OK)
	{
		fprintf(stderr, "Read error on block %"PRIu32"\n", block);
		return;
	}


}

const char *dev_status_string(DeviceStatus status)
{
	static const char *status_strs[] =
	{
		"Ok",
		"Failure",
		"Out of bounds access",
	};

	assert(status >= 0 && status < ARRLEN(status_strs));
	return status_strs[status];
}
