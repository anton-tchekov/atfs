#include "dev.h"
#include <inttypes.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

void dev_print_block(BlockDevice *dev, u32 block)
{
	u32 p, i, c;
	u8 buffer[dev->BlockSize];

	if(dev->Read(block, 1, buffer) != DEVICE_STATUS_OK)
	{
		fprintf(stderr, "Read error on block %"PRIu32"\n", block);
		return;
	}

	printf("Prining block %"PRIu32":\n", block);

	for(p = 0; p < dev->BlockSize; p += 16)
	{
		printf("%04x: ", p);

		for(i = 0; i < 16; ++i)
		{
			printf("%02x ", buffer[p + i]);
		}

		printf("[ ");
		for(i = 0; i < 16; ++i)
		{
			c = buffer[p + i];
			printf("%c", isprint(c) ? c : '.');
		}

		printf(" ]\n");
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

	assert(status >= 0 && status < (int)ARRLEN(status_strs));
	return status_strs[status];
}
