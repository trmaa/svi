#pragma once

#include "sys.h"

#define ICANON 0x02
#define ECHO   0x08
#define VMIN   6
#define VTIME  5
#define FIONREAD 0x541B

typedef unsigned long long key_t;

struct term_conf {
	u32 c_iflag, c_oflag, c_cflag, c_lflag;
	u8 c_line, c_cc[32];
	u32 c_ispeed, c_ospeed;
};

void term_init(void);
void term_restore(void);
bool  kbhit(void);
key_t getch();
void putch(char);
