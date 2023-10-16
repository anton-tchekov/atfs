# ATFS - A file system from scratch

This page is a documentation for the ATFS file system,
and a description of how a simple file system can be created.

The goal of this project is to create a custom filesystem
for a hobby operating system using only simple
algorithms and datastructures, to make it easy to understand
and learn from. (and it also uses less code space, which
is nice on microcontrollers)

## Storage device access

By making the filesystem independant from the storage device
it uses, for example hard drive, SSD, USB flash drive, SD card,
the filesystem can be used in many different contexts without
changing the filesystem code itself.

This is accomplished by introducing a block device interface,
which sits between the functions that access the hardware,
and the functions of the file system.

## Tracking free space

We need a way to keep track of which blocks on the disk
are already occupied and which blocks are free to use.

### Free space bitmap

There are many different ways to do this, for example a
bitmap, where one bit represents the state of one block,
a 0 for free and 1 for used. The bitmap is located at a
fixed position and searched when space needs to be allocated.

For a 4 GiB drive with 4 KiB sectors the bitmap will take
up only 128 KiB of storage.

```
	4 GiB = 2^32 bytes
	4 KiB = 2^12 bytes
	1 byte = 8 bits = 2^3 bits

	2^32 bytes / 2^12 bytes * 1 bit = 2^20 bits
	2^20 bits / 2^3 bits per byte = 2^17 bytes = 128 KiB
```

In relative terms, the bitmap uses only a very small percentage
of the whole drive (in the example above: 0.003%), but it can still
grow very large in absolute numbers, like on a multi-terabyte drive.

Problems also arise with fragmentation and finding free space
becomes harder with more space being full.

### Linked list of free areas

ATFS uses a linked list of free areas (groups of contiguous blocks) instead.

### File storage

[More good info on file systems](https://web.stanford.edu/~ouster/cgi-bin/cs140-winter13/lecture.php?topic=files)

There are three main ways of storing files.

- Linked list (used by FAT32)
	- Every block points to the next block of the file
	- with FAT the linked list is located in the file allocation table,
		not the data blocks themselves.

	- **Pros:** Easy to add more space to a file (even in the middle!)
	- **Cons:** Long seek times for non-sequential access
		(mitigated by keeping the FAT in memory)
		high fragmentation

- (Multi-level) indexed allocation (used by UNIX fileystems)
	- A tree of pointers
	- **Pros:** Random access for large files with only two or three levels of
		indirection, More indirect blocks are added only when needed

	- **Cons:** Overhead for large files, because a pointer to every
		individual block of the file needs to be stored.

- Extents (ext4, NTFS)
	One file can have multiple extents, which are contiguous ranges of blocks,
	so only the start and end need to be stored.
	- **Pros:** Less fragmentation, reduced metadata overhead
	- **Cons:** With multiple extents possible, probably the best option

In ATFS, one file is represented by a single extent,
meaning that a file always occupies a contiguous range of blocks.
The file capacity is determined at creation. If a file
needs to grow, a new file with more space can be created,
and the contents of the original file copied to it.
Therefore, growing a file is a expensive operation and should be
avoided if possible, by preallocating enough space.
On the other hand, shrinking a file by freeing the
end part is cheap.

- **Pros:** Very simple, few seeks, easy random access
- **Cons:** Hard to predict what size is needed at creation time,
	Large files may become impossible with fragmentation.

### Directories

A directory is simply a special type of file, that stores
directory entries, which contain metadata for files.

Unix-Type Filesystems use Inodes inbetween the directory entry
and the file data, which makes hard links possible.

ATFS stores the following information in a directory entry:

- File starting block number (4 bytes)
- File capacity in blocks (4 bytes)
- File type (1 byte)
- File name (55 bytes)

Specifically omitted metadata and ideas for the future:

- Last modification and access date and time
- Permissions (Read, write, execute)
- Attributes (Hidden, system, read-only, etc.)
- Owner, Group
- Tags (Important, Work, Personal, TODO, etc.)
- Versioning

The length of the file name field is chosen specifically so that
the total size of a directory entry is 64 bytes, which is a
neat power of two.

Another possibility would be to make the file name field
variable length, which would use less space since most
file names are short, but also increase complexity.

Because we store the filename as a null-terminated string,
the maximum filename length is 54 bytes, which is acceptable.

### File Type enum
- 0: Unused (or deleted) directory entry
- 1: Directory
- 2: Regular File
- Higher values are reserved for future expansion

## Formatting

Three steps:
- Creating the boot block (Header, Some info)
- Creating the root directory (clear a few blocks after boot block)
- Creating the linked list of free areas (remaining disk space)

## Simple operations

### Create file or directory

`` create `path` `capacity` ``

Allocate space. Create directory entry.

### Open

`` open `path` ``

Find directory entry. Store file start block and size in
file struct.

### Close

`` close `file-pointer` ``

If the file is marked as deleted, free it.

### Read / Write

`` read `file` `block` `count` `buffer` ``

`` write `file` `block` `count` `buffer` ``

Add offset to block number, do a bounds check with count.
Then forward call to block device interface.

## More complex operations

### Move / Rename (work in progress)

`` move `destination-path` `source-path` ``

Renaming consists of two steps, finding the directory entry,
then changing the name.

If the move destination is in the same directory as the source,
perform rename operation.

Find the directory entry of the source. Find a free directory entry
in the destination directory and copy it there. Delete the source
directory entry.

**Note:** The file contents are not touched at all!

### Delete (work in progress)

`` delete `path` ``

If a file is being deleted, free all of its blocks.
If a directory is deleted, call the delete function for every
directory entry. Then fill it's directory entry with all zeros
to mark it as deleted.

**Concurrency Problem**

The delete operation has a concurrency problem.
The following example sequence of calls illustrates the problem:

1. Create file `test`
2. Open `test`
3. Read contents of `test`
4. Delete `test` !!
5. Write `Hello World` to `test`
6. Close `test`
7. We have potentially corrupted the file system by overwriting data structures

There are two ways of preventing this problem:
1. Prevent the deletion (Windows does this)
2. Only mark the file as deleted, and delete it when it is closed
	(Linux does this)

### Copy (work in progress)

`` copy `destination-path` `source-path` ``

