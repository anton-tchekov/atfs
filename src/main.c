/**
 * @file    main.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    22.09.2023
 * @brief   ATFS Test functions
 */

#include <stdio.h>
#include "ramdisk.h"
#include "atfs_dir.h"
#include <string.h>
#include <stdlib.h>
#include "atfs.h"
#include "atfs_format.h"
#include "atfs_alloc.h"

ATFS_Status atfs_ls(BlockDevice *dev, const char *path, int detailed);
ATFS_Status atfs_tree(BlockDevice *dev, const char *path);

#define WHITESPACE " \n\t\v\f\r"

typedef void (*ShellCommandFunction)(int, char **);

typedef struct
{
	ShellCommandFunction Command;
	char *Name, *Desc;
} ShellCommand;

static void _cmd_help(int count, char **args);
static void _cmd_ls(int count, char **args);
static void _cmd_dir(int count, char **args);
static void _cmd_tree(int count, char **args);
static void _cmd_rm(int count, char **args);
static void _cmd_mkdir(int count, char **args);
static void _cmd_move(int count, char **args);
static void _cmd_copy(int count, char **args);

static const ShellCommand _cmds[] =
{
	{ _cmd_help,  "help",  "Print this list" },
	{ _cmd_ls ,   "ls",    "List directory" },
	{ _cmd_dir,   "dir",   "List directory detailed" },
	{ _cmd_tree,  "tree",  "Print directory tree" },
	{ _cmd_rm,    "rm",    "Recursively delete files and directories"  },
	{ _cmd_mkdir, "mkdir", "Create directory" },
	{ _cmd_move,  "move",  "Move/Rename" },
	{ _cmd_copy,  "copy",  "Copy" },
	{ NULL,       "exit",  "Exit program" },
	{ NULL,       "quit",  "Alias for exit" },
};

static void _cmd_help(int count, char **args)
{
	int i;
	printf("ATFS Version 0.1 Shell\n\n");
	for(i = 0; i < (int)ARRLEN(_cmds); ++i)
	{
		printf("%-10s:  %s\n", _cmds[i].Name, _cmds[i].Desc);
	}

	(void)count, (void)args;
}

static void _cmd_ls(int count, char **args)
{
	if(count != 2)
	{
		printf("Usage: ls `path`\n");
		return;
	}

	printf("%s\n", atfs_status_string(atfs_ls(&ramdisk, args[1], 0)));
}

static void _cmd_dir(int count, char **args)
{
	if(count != 2)
	{
		printf("Usage: dir `path`\n");
		return;
	}

	printf("%s\n", atfs_status_string(atfs_ls(&ramdisk, args[1], 1)));
}

static void _cmd_tree(int count, char **args)
{
	if(count != 2)
	{
		printf("Usage: tree `path`\n");
		return;
	}

	printf("%s\n", atfs_status_string(atfs_tree(&ramdisk, args[1])));
}

static void _cmd_rm(int count, char **args)
{
	if(count != 2)
	{
		printf("Usage: rm `path`\n");
		return;
	}

	printf("%s\n", atfs_status_string(atfs_delete(&ramdisk, args[1])));
}

static void _cmd_mkdir(int count, char **args)
{
	if(count != 2)
	{
		printf("Usage: mkdir `path-to-new-dir`\n");
		return;
	}

	printf("%s\n", atfs_status_string(atfs_fcreate(
		&ramdisk, args[1], ATFS_TYPE_DIR, 4)));
}

static void _cmd_move(int count, char **args)
{
	if(count != 3)
	{
		printf("Usage: move `src` `dst`\n");
		return;
	}

	printf("%s\n", atfs_status_string(atfs_move(&ramdisk, args[2], args[1])));
}

static void _cmd_copy(int count, char **args)
{
	if(count != 3)
	{
		printf("Usage: copy `src` `dst`\n");
		return;
	}

	printf("%s\n", atfs_status_string(ATFS_STATUS_NOT_IMPLEMENTED));
}

static const char *dirslash(ATFS_FileType type)
{
	return type == ATFS_TYPE_DIR ? "/" : "";
}

static void nspaces(int n)
{
	while(n--)
	{
		printf(" ");
	}
}

ATFS_Status atfs_ls(BlockDevice *dev, const char *path, int detailed)
{
	ATFS_Dir dir;
	ATFS_DirEntry entry;
	ATFS_Status status;

	PROPAGATE(atfs_dopen(dev, path, &dir));

	if(detailed)
	{
		while((status = atfs_dread(&dir, &entry)) == ATFS_STATUS_OK)
		{
			printf("%8d  %s%s\n", entry.SizeBlocks,
				entry.Name, dirslash(entry.Type));
		}
	}
	else
	{
		while((status = atfs_dread(&dir, &entry)) == ATFS_STATUS_OK)
		{
			printf("%s%s\n", entry.Name, dirslash(entry.Type));
		}
	}

	if(status != ATFS_STATUS_DIR_END)
	{
		return status;
	}

	return ATFS_STATUS_OK;
}

ATFS_Status atfs_tree(BlockDevice *dev, const char *path)
{
	/* Tree function without recursion, nice! */
	char buf[256];
	ATFS_Dir dir[16];
	ATFS_DirEntry entry;
	ATFS_Status status;
	int nesting;

	nesting = 0;
	strcpy(buf, path);
	PROPAGATE(atfs_dopen(dev, buf, &dir[nesting]));

	while(nesting >= 0)
	{
		while((status = atfs_dread(&dir[nesting], &entry)) == ATFS_STATUS_OK)
		{
			nspaces(4 * nesting);
			printf("- %s%s\n", entry.Name, dirslash(entry.Type));
			if(entry.Type == ATFS_TYPE_DIR)
			{
				++nesting;
				atfs_path_join(buf, entry.Name);
				PROPAGATE(atfs_dopen(dev, buf, &dir[nesting]));
			}
		}

		if(status != ATFS_STATUS_DIR_END)
		{
			return status;
		}

		--nesting;
		atfs_path_parent(buf);
	}

	return ATFS_STATUS_OK;
}

int main(void)
{
	BlockDevice *dev = &ramdisk;
	int i, count;
	char *pch, buf[256], *args[16];

	printf("RAMDISK Format: %s\n", atfs_status_string(atfs_format(dev)));
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

		if(!strcmp(args[0], "exit") ||
			!strcmp(args[0], "quit"))
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
			printf("Command not found.\n"
				"Type \"help\" for a list of valid commands\n");
		}
	}

	return 0;
}

/* --- Commented out --- */

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

#endif

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

#if 0
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
	printf("%s\n", atfs_status_string(atfs_tree(dev, "")));
#endif
