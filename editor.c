/*
 * editor.c - gap buffer implementation
 *
 * Copyright (c) 2026
 * License: GNU's GPL, see online
 * Author: Pablo Trik Marin (trmaa) <mail@pablotrik.online>
 */

#include "sys.h"
#include "editor.h"

enum mode mode = NORMAL;

static struct gap gap;
static u64 heap_start;

static char
char_at(u64 pos)
{
	if (pos < gap.start)
		return gap.buf[pos];
	return gap.buf[pos - gap.start + gap.end];
}

static void
grow_gap(void)
{
	u64 grow = gap.size;
	u64 new  = gap.size + grow;
	u64 rlen = gap.size - gap.end;
	u64 ne   = new - rlen;
	u64 i;

	brk(gap.buf + new);

	for (i = 0; i < rlen; i++)
		gap.buf[ne + i] = gap.buf[gap.end + i];

	gap.end  = ne;
	gap.size = new;
}

void gap_init(int fd)
{
	u64 off, i;

	heap_start = (u64)brk(0);

	off = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	off = off * 2 + 4096;
	brk((void *)(heap_start + off));

	gap.buf  = (char *)heap_start;
	gap.size = off;

	for (i = 0;; i++) {
		u64 n = read(fd, gap.buf + i, 1);
		if (n != 1)
			break;
	}

	gap.start = i;
	gap.end   = off;
}

void gap_free(void)
{
	brk((void *)heap_start);
}

void gap_save(int fd)
{
	u64 len;

	lseek(fd, 0, SEEK_SET);
	write(fd, gap.buf, gap.start);
	write(fd, gap.buf + gap.end, gap.size - gap.end);

	len = gap.start + (gap.size - gap.end);
	ftruncate(fd, len);
}

u64 gap_len(void)
{
	return gap.start + (gap.size - gap.end);
}

void setch(char c)
{
	if (gap.start == gap.end)
		grow_gap();

	gap.buf[gap.start++] = c;
}

void backspace(void)
{
	if (gap.start > 0)
		gap.start--;
}

void del(void)
{
	if (gap.end < gap.size)
		gap.end++;
}

void move_left(void)
{
	if (gap.start > 0)
		gap.buf[--gap.end] = gap.buf[--gap.start];
}

void move_right(void)
{
	if (gap.end < gap.size)
		gap.buf[gap.start++] = gap.buf[gap.end++];
}

void move_to(u64 pos)
{
	u64 count;

	if (pos == gap.start)
		return;

	if (pos < gap.start) {
		count = gap.start - pos;
		for (u64 i = 0; i < count; i++)
			gap.buf[gap.end - count + i] = gap.buf[pos + i];
		gap.start = pos;
		gap.end  -= count;
	} else {
		count = pos - gap.start;
		for (u64 i = 0; i < count; i++)
			gap.buf[gap.start + i] = gap.buf[gap.end + i];
		gap.start = pos;
		gap.end  += count;
	}
}

void move_up(void)
{
	u64 pos, line, col, prev, target, i;

	pos = gap.start;
	line = pos;

	while (line > 0 && char_at(line - 1) != '\n')
		line--;

	if (line == 0)
		return;

	col = 0;
	for (i = line; i < pos; i++)
		col++;

	prev = line - 1;
	while (prev > 0 && char_at(prev - 1) != '\n')
		prev--;

	target = prev;
	for (i = 0; i < col && char_at(target) != '\n'; i++)
		target++;

	move_to(target);
}

void move_down(void)
{
	u64 pos, line, next, col, target, i;

	pos = gap.start;
	line = pos;

	while (line > 0 && char_at(line - 1) != '\n')
		line--;

	col = 0;
	for (i = line; i < pos; i++)
		col++;

	next = pos;
	while (char_at(next) != '\n' && next < gap_len())
		next++;

	if (next == gap_len())
		return;

	next++;

	target = next;
	for (i = 0; i < col && char_at(target) != '\n'; i++)
		target++;

	move_to(target);
}

static u64
utoa(char *buf, u64 n)
{
	char tmp[20];
	u64 i = 0, j = 0;

	if (n == 0) {
		tmp[j++] = '0';
	} else {
		while (n > 0) {
			tmp[j++] = '0' + (n % 10);
			n /= 10;
		}
	}

	while (j > 0)
		buf[i++] = tmp[--j];

	return i;
}

static void
gotoxy(u64 row, u64 col)
{
	char buf[32];
	u64 i = 0;

	buf[i++] = '\033';
	buf[i++] = '[';
	i += utoa(buf + i, row);
	buf[i++] = ';';
	i += utoa(buf + i, col);
	buf[i++] = 'H';

	write(STDOUT, buf, i);
}

static void
cursor_pos(u64 pos, u64 *row, u64 *col)
{
	u64 r = 0, c = 0;

	for (u64 i = 0; i < pos; i++) {
		if (char_at(i) == '\n') {
			r++;
			c = 0;
		} else {
			c++;
		}
	}

	*row = r;
	*col = c;
}

void render(void)
{
	u64 len, row, col, rows, lines, i;
	struct winsize ws;

	len  = gap_len();
	rows = 24;

	if (ioctl(STDOUT, TIOCGWINSZ, &ws) >= 0)
		rows = ws.ws_row;

	if (rows < 2)
		rows = 2;

	lines = 0;
	for (i = 0; i < len; i++)
		if (char_at(i) == '\n')
			lines++;

	write(STDOUT, "\033[H", 3);

	write(STDOUT, gap.buf, gap.start);
	write(STDOUT, gap.buf + gap.end, gap.size - gap.end);

	for (i = lines + 1; i < rows - 1; i++) {
		gotoxy(i + 1, 1);
		write(STDOUT, "~\033[K", 4);
	}

	gotoxy(rows, 1);
	write(STDOUT, "\033[K", 3);
	write(STDOUT, "\033[7m", 4);

	switch (mode) {
	case NORMAL:
		write(STDOUT, "-- NORMAL --", 12);
		break;
	case INSERT:
		write(STDOUT, "-- INSERT --", 12);
		break;
	}

	write(STDOUT, "\033[0m", 4);

	cursor_pos(gap.start, &row, &col);
	gotoxy(row + 1, col + 1);
}
