/*
 * main.c - where the magic begins
 *
 * License: GNU's GPL, see online
 * Author: Pablo Trik Marin (trmaa) <mail@pablotrik.online>
 */

#include "bool.h"
#include "string.h"
#include "term-io.h"

struct {
	int x, y;
} cursor;

main(int argc, char *argv[])
{
	term_init();

	for (;;) {
		if (!kbhit())
			continue;

		key_t k = getch();
		putch(k);

		if (k == 'q')
			break;
	}

	term_restore();
}
