#pragma once

#define NULL 0

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned char u8;

typedef unsigned long long reg_t;

enum fd {
	STDIN	= 0,
	STDOUT	= 1,
};

enum open_flag {
	O_RDONLY    = 0,
	O_WRONLY    = 01,
	O_RDWR      = 02,
	O_CREAT     = 0100,
	O_EXCL      = 0200,
	O_TRUNC     = 01000,
	O_APPEND    = 02000,
	O_NONBLOCK  = 04000,
};

enum open_mode {
	S_IRUSR     = 00400,
	S_IWUSR     = 00200,
	S_IXUSR     = 00100,
	S_IRGRP     = 00040,
	S_IWGRP     = 00020,
	S_IXGRP     = 00010,
	S_IROTH     = 00004,
	S_IWOTH     = 00002,
	S_IXOTH     = 00001,
};

enum syscall_type : reg_t {
	READ	= 0,
	WRITE	= 1,
	OPEN	= 2,
	CLOSE	= 3,
	IOCTL	= 16,
	EXIT	= 60
};

reg_t syscall(enum syscall_type rax, ...);

#define read(fd, buf, size) syscall(READ, fd, buf, size)
#define write(fd, buf, size) syscall(WRITE, fd, buf, size)
#define open(path, ...) syscall(OPEN, path, ##__VA_ARGS__)
#define close(fd) syscall(CLOSE, fd)
#define ioctl(fd, cmd, dest) syscall(IOCTL, fd, cmd, dest)
#define exit(err) syscall(EXIT, err)

#define BRK        12
#define LSEEK      8
#define FTRUNCATE  77

#define brk(a)        syscall(BRK, a)
#define lseek(f,o,w)  syscall(LSEEK, f, o, w)
#define ftruncate(f,l) syscall(FTRUNCATE, f, l)

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define TIOCGWINSZ 0x5413

struct winsize {
	unsigned short ws_row;
	unsigned short ws_col;
	unsigned short ws_xpixel;
	unsigned short ws_ypixel;
};
