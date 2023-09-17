#include <stdio.h>
#include "ramdisk.h"

int main(int argc, char **argv)
{
	BlockDevice *dev = &ramdisk;

	dev_print_block(dev, 0);

	return 0;
}
