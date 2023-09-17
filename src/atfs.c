#include "atfs.h"
#include <assert.h>

const u8 ATFS_SIGNATURE[4] = { 'A', 'T', 'F', 'S' };

const char *atfs_status_string(ATFS_Status status)
{
	static const char *status_str[] =
	{
		"No such file or directory",
		"No space left on device",
	};

	if((DeviceStatus)status < DEVICE_STATUS_COUNT)
	{
		return dev_status_string((DeviceStatus)status);
	}

	status -= DEVICE_STATUS_COUNT;
	assert(status < ARRLEN(status_str));
	return status_str[status];
}
