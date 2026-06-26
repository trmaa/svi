/*
 * main.c - where the magic begins
 *
 * Copyright (c) 2026
 * License: GNU's GPL, see online
 * Author: Pablo Trik Marin (trmaa) <mail@pablotrik.online>
 */

#include "sys.h"
#include "debug.h"
#include "term-io.h"
#include "editor.h"

static int  fd = -1;
static bool running = true;

static void
update_file(key_t k)
{
	switch (mode) {
	case NORMAL:
		switch (k) {
		case 'w':
			gap_save(fd);
			break;
		case 'q':
			running = false;
			break;
		case 'h':
			move_left();
			break;
		case 'l':
			move_right();
			break;
		case 'j':
			move_down();
			break;
		case 'k':
			move_up();
			break;
		case 'x':
			del();
			break;
		case 'i':
			mode = INSERT;
			break;
		}
		break;
	case INSERT:
		switch (k) {
		case 0x7f:
			backspace();
			break;
		case '\033':
			mode = NORMAL;
			break;
		default:
			setch((char)k);
			break;
		}
		break;
	}
}

main(int argc, char *argv[])
{
	if (argc < 2)
		fatal("No args yet to be implemented.", help, ERROR);
	if (argc > 2)
		fatal("Too meny args!", help, USER_ERROR);

	fd = open(argv[1], O_RDWR);

	term_init();
	clrscr();
	gap_init(fd);
	render();

	while (running) {
		if (!kbhit())
			continue;

		key_t k = getch();
		update_file(k);
		clrscr();
		render();
	}

	gap_free();
	close(fd);
	clrscr();
	term_restore();
}
