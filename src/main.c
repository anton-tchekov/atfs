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

ATFS_Status atfs_ls(BlockDevice *dev, const char *path)
{
	ATFS_Dir dir;
	ATFS_DirEntry entry;
	ATFS_Status status;

	PROPAGATE(atfs_dopen(dev, path, &dir));
	while((status = atfs_dread(&dir, &entry)) == ATFS_STATUS_OK)
	{
		printf("%8d  %s%s\n", entry.SizeBlocks,
			entry.Name, entry.Type == ATFS_TYPE_DIR ? "/" : "");
	}

	if(status != ATFS_STATUS_DIR_END)
	{
		return status;
	}

	return ATFS_STATUS_OK;
}

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

	printf("%s\n", atfs_status_string(DEVICE_STATUS_OUT_OF_BOUNDS));
	printf("%s\n", atfs_status_string(ATFS_NO_SPACE_LEFT_ON_DEVICE));
#endif

	printf("%s\n", atfs_status_string(atfs_format(dev)));
	dev_print_block(dev, 0);
	dev_print_block(dev, 1);
	dev_print_block(dev, 2);
	dev_print_block(dev, 3);
	dev_print_block(dev, 4);
	dev_print_block(dev, 5);

	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "bin", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "boot", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "dev", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "etc", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "home", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "lib", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "media", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "mnt", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "opt", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "proc", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "run", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "bin", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "srv", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "sys", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "tmp", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "usr", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "var", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "aa", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "bb", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "cc", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "dd", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "ee", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "ff", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "gg", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "hh", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "ii", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "jj", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "kk", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "ll", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "mm", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "nn", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "oo", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "pp", ATFS_TYPE_DIR, 4)));

	printf("create subdirs?\n");
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "usr.bin", ATFS_TYPE_DIR, 4)));
	printf("%s\n", atfs_status_string(atfs_fcreate(dev, "home.anton", ATFS_TYPE_DIR, 4)));

	dev_print_block(dev, 1);
	dev_print_block(dev, 2);
	dev_print_block(dev, 3);
	dev_print_block(dev, 4);

	atfs_print_free(dev);

	printf("%s\n", atfs_status_string(atfs_ls(dev, "")));


#if 0
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
#endif

	return 0;
}


#if 0

#include "fs.h"
#include <atfs.h>
#include <types.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <status.h>

#define ARRLEN(A) (sizeof(A) / sizeof(*(A)))

static char buf[512];

#define TEST_PATH_VALID(str) printf("path_valid(\"%s\") = %s\n", str, path_valid(str) ? "true" : "false")
#define TEST_PATH_PARENT(str) printf("path_parent(\"%s\") = \"%s\"\n", str, path_parent(strdup(str)))
#define TEST_PATH_JOIN(str, append) printf("path_join(\"%s\", \"%s\") = \"%s\"\n", str, append, path_join(strcpy(buf, str), append))

void _test(void)
{
	TEST_PATH_VALID("home.files.studium.gs.its_board.a2_drehgeber.main");
	TEST_PATH_VALID("sys");
	TEST_PATH_VALID("sys.init");
	TEST_PATH_VALID(".abc");
	TEST_PATH_VALID("abc.");
	TEST_PATH_VALID("a.bc..");
	TEST_PATH_VALID("a.b.c.d.e.f.g");
	TEST_PATH_VALID("home..files");
	TEST_PATH_VALID("home.files");
	TEST_PATH_VALID("home.4chan");
	TEST_PATH_VALID("home.chan2");
	TEST_PATH_VALID("home.Uppercase");
	TEST_PATH_VALID("home.#hashtag");
	TEST_PATH_VALID("");

	printf("\n\n");

	TEST_PATH_PARENT("home.files.images");
	TEST_PATH_PARENT("home");

	printf("\n\n");

	TEST_PATH_JOIN("home.files.images", "vacation");
	TEST_PATH_JOIN("sys", "init");

	printf("\n\n");

	printf("fs_format() = %s\n", status_str(fs_format()));

	printf("\n\n");
}

/* --- Terminal Emulator --- */
#define WHITESPACE " \n\t\v\f\r"

static void _cmd_pwd(int count, char **args);
static void _cmd_cd(int count, char **args);
static void _cmd_ls(int count, char **args);
static void _cmd_dir(int count, char **args);
static void _cmd_help(int count, char **args);
static void _cmd_rm(int count, char **args);
static void _cmd_mkdir(int count, char **args);
static void _cmd_mv(int count, char **args);
static void _cmd_cp(int count, char **args);
static void _cmd_mount(int count, char **args);

static char _path[512];

typedef void (*ShellCommandFunction)(int, char **);

typedef struct
{
	char *Name, *Desc;
	ShellCommandFunction Command;
} ShellCommand;

ShellCommand _cmds[] =
{
	{ "pwd", "Print current working directory", _cmd_pwd },
	{ "cd", "Change current directory", _cmd_cd },
	{ "ls", "List directory", _cmd_ls },
	{ "dir", "List directory detailed", _cmd_dir },
	{ "help", "Print this list", _cmd_help },
	{ "rm", "Recursively delete files and directories", _cmd_rm },
	{ "mkdir", "Create directory", _cmd_mkdir },
	{ "mv", "Move/Rename file", _cmd_mv },
	{ "cp", "Copy", _cmd_cp },
	{ "mount", "Mount device as ", _cmd_mount },
};

/**
 * @brief Print working directory
 *
 * @param count Ignored
 * @param args Ignored
 */
static void _cmd_pwd(int count, char **args)
{
	printf("%s\n", _path);
	(void)count, (void)args;
}

static void _cmd_cd(int count, char **args)
{

}

static void _cmd_ls(int count, char **args)
{
	/*u32 i;
	Dir dir;
	DirEntry entry;

	RETURN_IF(fs_dopen(path, &dir));
	printf("Number of Entries: %d\n", dir.NumEntries);

	exit(1);

	for(i = 0; i < dir.NumEntries; ++i)
	{
		RETURN_IF(fs_dread(&dir, i, &entry));
		printf("%s\n", entry.Name);
	}

	return STATUS_OK;*/
}

static void _cmd_dir(int count, char **args)
{

}

static void _cmd_help(int count, char **args)
{
	int i;
	printf("ImaginaryOS Version 0.1 - 384 KiB RAM\n\n");
	for(i = 0; i < (int)ARRLEN(_cmds); ++i)
	{
		printf("%-10s:  %s\n", _cmds[i].Name, _cmds[i].Desc);
	}

	(void)count, (void)args;
}

static void _cmd_rm(int count, char **args)
{
	printf("Usage: mkdir [path]\n");
}

static void _cmd_mkdir(int count, char **args)
{
	printf("Usage: mkdir [new-dir-path]\n");
}

static void _cmd_mv(int count, char **args)
{

}

static void _cmd_cp(int count, char **args)
{

}

static void _cmd_mount(int count, char **args)
{
	printf("Usage: mount [device-number] [mount-name]\n");
}

int main(void)
{
	int i, count;
	char *pch, buf[128], *args[16];

	_test();

	/* Terminal simulator */
	printf("TERMINAL READY.\n");
	for(;;)
	{
		printf("> ");
		fgets(buf, sizeof(buf), stdin);

		count = 0;
		pch = strtok(buf, WHITESPACE);
		while(pch)
		{
			args[count++] = pch;
			pch = strtok(NULL, WHITESPACE);
		}

		if(!strcmp(args[0], "exit"))
		{
			break;
		}

		for(i = 0; i < (int)ARRLEN(_cmds); ++i)
		{
			if(!strcmp(args[0], _cmds[i].Name))
			{
				_cmds[i].Command(count, args);
				break;
			}
		}

		if(i == (int)ARRLEN(_cmds))
		{
			printf("Command not found. Type \"help\" for a list of valid commands\n");
		}
	}

	return 0;
}

#endif
