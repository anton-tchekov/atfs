#include <stdio.h>
#include "ramdisk.h"
#include <string.h>
#include <stdlib.h>
#include "atfs.h"

int main(int argc, char **argv)
{
	BlockDevice *dev = &ramdisk;

	u8 buffer[dev->BlockSize];
	memset(buffer, 0, sizeof(buffer));
	strcpy((char *)buffer, "Hello World");

	dev->Write(0, 1, buffer);

	dev_print_block(dev, 0);

	printf("%s\n", atfs_status_string(DEVICE_STATUS_OUT_OF_BOUNDS));
	printf("%s\n", atfs_status_string(ATFS_NO_SPACE_LEFT_ON_DEVICE));

	return 0;
}
