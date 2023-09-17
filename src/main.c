#include <stdio.h>
#include "ramdisk.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	BlockDevice *dev = &ramdisk;

	u8 buffer[dev->BlockSize];
	memset(buffer, 0, sizeof(buffer));
	strcpy((char *)buffer, "Hello World");

	dev->Write(0, 1, buffer);

	dev_print_block(dev, 0);

	return 0;
}
