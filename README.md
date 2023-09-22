# atfs

<h1>Making a File System from Scratch</h1>
<p>
This page serves simultaneously as a documentation for the
ATFS file system, and as a description of how a simple
file system can be implemented.
</p>
<p>
The goal of this project is to create a custom filesystem
for a hobby operating system using only simple
algorithms and datastructures, to make it easy to understand
and learn from. (and it also uses less code space, which
is nice on microcontrollers)
</p>
<h2>Storage Device Access</h2>
<p>
By making the filesystem independant from the storage device
it uses, for example hard drive, SSD, USB flash drive, SD card,
the filesystem can be used in many different contexts without
changing the filesystem code itself.
</p>
<p>
This is accomplished by introducing a block device interface,
which sits between the functions that access the hardware,
and the functions of the filesystem.
</p>
<h2>Tracking Free Space</h2>
<p>
We need a way to keep track of which blocks on the disk
are already occupied and which blocks are free to be used.
</p>
<h3>Free Space Bitmap</h3>
<p>
There are many different ways to do this, for example a
bitmap, where one bit represents the state of one block,
a 0 for free and 1 for used.
The bitmap is located at a fixed position and searched
when space needs to be allocated.

For a 4 GiB drive with 4 KiB sectors the bitmap will take
up only 128 KiB of storage

<code>
	4 GiB = 2^32 bytes
	4 KiB = 2^12 bytes
	1 byte = 8 bits = 2^3 bits

	2^32 bytes / 2^12 bytes * 1 bit = 2^20 bits
	2^20 bits / 2^3 bits per byte = 2^17 bytes = 128 KiB
</code>

In relative terms, the bitmap uses only a very small percentage
of the whole drive (in the example above: 0.003%), but it can still
grow very large in absolute numbers, like on a multi-terabyte drive.

Problems also arise with fragmentation and finding free Space
becomes harder with more space being full.
</p>
<h3>Linked List of Free Areas</h3>
<p>
We decided to use a linked list of free areas instead.
An area as a contiguous blocks.
</p>
<h2>File storage</h2>
<p>
There are three main ways of storing files.
<ol>
	<li>Linked List (used by FAT32)</li>
	<li>Indexed allocation (UNIX fileystems)</li>
	<li>Extents (contiguous allocation)</li>
</ol>
</p>
<p>
For simplicity, one file is represented by a single extent,
meaning that a file always occupies a contiguous range of blocks.
The file capacity is determined at creation. If a file
needs to grow, a new file with more space can be created,
and the contents of the original file copied to it.
Therefore, growing a file is a expensive operation and should be
avoided if possible, by preallocating enough space.
On the other hand, shrinking a file by freeing the
end part is cheap.
</p>
<h2>Directories</h2>
<p>
A directory is simply a special type of file, that stores
directory entries, which contain metadata for files.
</p>
<p>
Unix-Type Filesystems use Inodes inbetween the directory entry
and the file data,
</p>
<p>
We decided to store the following information
in a directory entry:
</p>
<ul>
<li>File starting block number (4 bytes)</li>
<li>File capacity in blocks (4 bytes)</li>
<li>File type (1 byte)</li>
<li>File name (55 bytes)</li>
</ul>
<p>
Specifically omitted metadata and ideas for the future:
</p>
<ul>
<li>Last modification and access date and time</li>
<li>Permissions (Read, write, execute)</li>
<li>Attributes (Hidden, system, read-only, etc.)</li>
<li>Owner, Group</li>
<li>Tags (Important, Work, Personal, TODO, etc.)</li>
<li>Versioning</li>
</ul>
<p>
The length of the file name field is chosen specifically so that
the total size of a directory entry is 64 bytes, which is a
neat power of two.
</p>
<p>
Another possibility would be to make the file name field
variable length, which would use less space since most
file names are short, but also increase complexity.
</p>
<p>
Because we store the filename as a null-terminated string,
the maximum filename length is 54 bytes, which is acceptable.
</p>
<h3>File Type enum</h3>
<ul>
<li>0: Unused (or deleted) directory entry</li>
<li>1: Directory</li>
<li>2: Regular File</li>
<li>Higher values are reserved for future expansion</li>
</ul>
<h2>Formatting</h2>
<p>
Creating the boot sector
</p>
<h2>Simple operations</h2>
<h3>Create</h3>
<p>
<code>create `path` `capacity`</code>
</p>
<p>
Allocate space. Create directory entry
</p>
<h3>Open</h3>
<p>
<code>open `path`</code>
</p>
<p>
Find directory entry. Store file start block and size in
file struct.
</p>
<h3>Read / Write</h3>
<p>
<code>read `file` `block` `buffer`</code>
</p>
<p>
<code>write `file` `block` `buffer`</code>
</p>
<p>
Add offset to block number, do bounds check.
Then forward call to block device.
</p>
<h2>More complex operations</h2>
<h3>Move / Rename</h3>
<p>
<code>move `destination-path` `source-path`</code>
</p>
<p>
Renaming consists of two steps, finding the directory entry,
then changing the name.
</p>
<p>
If the move destination is in the same directory as the source,
perform rename operation.

Find the directory entry of the source. Find a free directory entry
in the destination directory and copy it there. Delete the source
directory entry.

<strong>Note:</strong> The file contents are not touched at all
</p>
<h3>Delete</h3>
<p>
<code>delete `path`</code>
</p>
<p>
If a file is being deleted, free all of its blocks.
If a directory is deleted, call the delete function for every
directory entry. Then fill it's directory entry with all zeros
to mark it as deleted.
</p>
<h3>Copy</h3>
<p>
<code>copy `destination-path` `source-path`</code>
</p>
<p>
Implementing the copy operation is left as an excersise for the
reader. Tip: Use recursion. The delete function is a good starting
point.
</p>

