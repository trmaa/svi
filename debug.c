#include "sys.h"
#include "string.h"
#include "debug.h"

void help(enum status err)
{
	print("USE:\n");
	print("\tsvi <file>\n");
	exit(err);
}

void fatal(char *msg, void (*callback)(enum status), enum status err)
{
	if (msg != NULL) {
		print("\e[1;31mFatal: ");
		print(msg);
		print("\e[0m\n");
	}

	if (callback != NULL)
		callback(err);

	exit(err);
}
