#include "atfs_path.h"

char *path_join(char *path, const char *append)
{
	char *start = path + strlen(path);
	*start = ATFS_DIR_SEPARATOR;
	strcpy(start + 1, append);
	return path;
}

char *path_parent(char *path)
{
	char c, *s, *last_sep;

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

bool path_valid(const char *path)
{
	bool was_sep;
	char c;
	const char *s;

	/* Initialize with true because path may not start with separator */
	was_sep = true;
	for(s = path; (c = *s); ++s)
	{
		if(c == ATFS_DIR_SEPARATOR)
		{
			if(was_sep)
			{
				/* Two separators in a row are not allowed */
				return false;
			}

			was_sep = true;
		}
		else if(isdigit(c))
		{
			if(was_sep)
			{
				/* Path component may not start with a digit */
				return false;
			}

			was_sep = false;
		}
		else if(c == '_' || islower(c))
		{
			was_sep = false;
		}
		else
		{
			/* Invalid character */
			return false;
		}
	}

	/* Path may not end with separator, but allow empty path */
	return !was_sep || *path == '\0';
}
