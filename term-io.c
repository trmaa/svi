#include "sys.h"
#include "debug.h"
#include "term-io.h"

static struct term_conf old_conf;
static int initialized;

void term_init(void)
{
	if (ioctl(STDIN, 0x5401, &old_conf) < 0)
		fatal("ioctl TCGETS fallo", NULL, OS_ERROR);

	struct term_conf new = old_conf;
	new.c_lflag &= ~(ICANON | ECHO);
	new.c_cc[VMIN] = 1;
	new.c_cc[VTIME] = 0;

	if (ioctl(STDIN, 0x5402, &new) < 0) {
		ioctl(STDIN, 0x5402, &old_conf);
		fatal("ioctl TCSETS fallo", NULL, OS_ERROR);
	}

	initialized = 1;
}

void term_restore(void)
{
	if (initialized)
		ioctl(STDIN, 0x5402, &old_conf);
}

bool kbhit(void)
{
	int n = 0;
	ioctl(STDIN, FIONREAD, &n);
	return n > 0;
}

key_t getch()
{
	key_t k = 0;

	if (!initialized)
		term_init();

	if (read(STDIN, &k, 1) < 0)
		fatal("read fallo", NULL, OS_ERROR);

	return k;
}

void putch(char c)
{
	write(STDOUT, &c, 1);
}
