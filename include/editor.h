#pragma once

#include "sys.h"

enum mode {
	NORMAL,
	INSERT
};

extern enum mode mode;

struct gap {
	char *buf;
	u64 size;
	u64 start, end;
};

void gap_init(int);
void gap_save(int);
void gap_free(void);
void move_left(void);
void move_right(void);
void move_up(void);
void move_down(void);
void move_to(u64);
void setch(char);
void backspace(void);
void del(void);
u64  gap_len(void);
void render(void);
