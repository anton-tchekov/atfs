#include <stdio.h>
#include "ramdisk.h"
#include "atfs_dir.h"
#include <string.h>
#include <stdlib.h>
#include "atfs.h"
#include "atfs_format.h"
#include "atfs_alloc.h"

#define ROOT_FILE_ADDR 0x00000001
#define ROOT_FILE_CHUNK_SIZE 4

int main(int argc, char **argv)
{
	BlockDevice *dev = &ramdisk;

#if 0
	u8 buffer[dev->BlockSize];
	memset(buffer, 0, sizeof(buffer));
	dev->Write(0, 1, buffer);

    DirEntry rootDir = {
            1, 1, 4, 2048, "/"
    };

    DirEntry tmpDir = {
            1, 6, 1, 512, "tmp"
    };
    DirEntry homeDir = {
            1, 8, 1, 512, "home"
    };
    DirEntry timDir = {
            1, 10, 1, 512, "tim"
    };
    DirEntry antonDir = {
            2, 12, 1, 512, "anton"
    };

    create_directory_entry(dev, ROOT_FILE_ADDR, ROOT_FILE_CHUNK_SIZE, homeDir);
    create_directory_entry(dev, ROOT_FILE_ADDR, ROOT_FILE_CHUNK_SIZE, tmpDir);
    create_directory_entry(dev, 8, 1, timDir);
    create_directory_entry(dev, 10, 1, antonDir);

    dev_print_block(dev, ROOT_FILE_ADDR);
    dev_print_block(dev, 8);

    DirEntry test = get_adress_by_path(dev, "/home/tim/anton", rootDir);

    printf("Adress: %d Name: %s Size (c): %d\n", test.addr, test.name, test.cSize);


	printf("%s\n", atfs_status_string(atfs_format(dev)));


	//printf("%s\n", atfs_status_string(DEVICE_STATUS_OUT_OF_BOUNDS));
	//printf("%s\n", atfs_status_string(ATFS_NO_SPACE_LEFT_ON_DEVICE));
#endif

	printf("%s\n", atfs_status_string(atfs_format(dev)));
	dev_print_block(dev, 0);
	dev_print_block(dev, 1);
	dev_print_block(dev, 2);
	dev_print_block(dev, 3);
	dev_print_block(dev, 4);
	dev_print_block(dev, 5);

	atfs_print_free(dev);

	u32 start;

	u32 first = 0;
	printf("%s\n", atfs_status_string(atfs_alloc(dev, 10, &first)));
	printf("allocated area: %d\n", first);
	atfs_print_free(dev);

	u32 second = 0;
	printf("%s\n", atfs_status_string(atfs_alloc(dev, 3, &second)));
	printf("allocated area: %d\n", second);
	atfs_print_free(dev);

	start = 0;
	printf("%s\n", atfs_status_string(atfs_free(dev, first, 10)));
	atfs_print_free(dev);

	start = 0;
	printf("%s\n", atfs_status_string(atfs_alloc(dev, 1, &start)));
	printf("allocated area: %d\n", start);

	printf("%s\n", atfs_status_string(atfs_free(dev, second, 3)));


	atfs_print_free(dev);

	return 0;
}
