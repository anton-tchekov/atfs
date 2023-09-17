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
