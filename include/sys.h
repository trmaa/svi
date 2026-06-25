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

enum syscall_type : reg_t {
	READ	= 0,
	WRITE	= 1,
	POLL	= 7,
	IOCTL	= 16,
	EXIT	= 60
};

reg_t syscall(enum syscall_type rax, ...);

#define read(fd, buf, size) syscall(READ, fd, buf, size)
#define write(fd, buf, size) syscall(WRITE, fd, buf, size)
#define ioctl(fd, cmd, dest) syscall(IOCTL, fd, cmd, dest)
#define exit(err) syscall(EXIT, err)
