/**
 * @file    atfs_path.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    18.09.2023
 */

#include "atfs_path.h"
#include "atfs.h"
#include <string.h>
#include <ctype.h>

char *atfs_path_join(char *path, const char *append)
{
	char *start;

	start = path + strlen(path);
	*start = ATFS_DIR_SEPARATOR;
	strcpy(start + 1, append);
	return path;
}

const char *atfs_path_last(const char *path)
{
	int c;
	const char *s, *last_sep;

	last_sep = path;
	for(s = path; (c = *s); ++s)
	{
		if(c == ATFS_DIR_SEPARATOR)
		{
			last_sep = s + 1;
		}
	}

	return last_sep;
}

char *atfs_path_parent(char *path)
{
	int c;
	char *s, *last_sep;

	last_sep = path;
	for(s = path; (c = *s); ++s)
	{
		if(c == ATFS_DIR_SEPARATOR)
		{
			last_sep = s;
		}
	}

	*last_sep = '\0';
	return path;
}

size_t atfs_path_first_len(const char *path)
{
	int c;
	const char *p;

	for(p = path; (c = *p); ++p)
	{
		if(c == ATFS_DIR_SEPARATOR)
		{
			break;
		}
	}

	return p - path;
}

const char *atfs_path_rest(const char *path)
{
	int c;

	for(; (c = *path); ++path)
	{
		if(c == ATFS_DIR_SEPARATOR)
		{
			return path + 1;
		}
	}

	return path;
}

int atfs_path_valid(const char *path)
{
	const char *s;
	int c, was_sep;

	/* Initialize with true because path may not start with separator */
	was_sep = 1;
	for(s = path; (c = *s); ++s)
	{
		if(c == ATFS_DIR_SEPARATOR)
		{
			if(was_sep)
			{
				/* Two separators in a row are not allowed */
				return 0;
			}

			was_sep = true;
		}
		else if(isdigit(c))
		{
			if(was_sep)
			{
				/* Path component may not start with a digit */
				return 0;
			}

			was_sep = 0;
		}
		else if(c == '_' || islower(c))
		{
			was_sep = 0;
		}
		else
		{
			/* Invalid character */
			return 0;
		}
	}

	/* Path may not end with separator, but allow empty path */
	return !was_sep || *path == '\0';
}
