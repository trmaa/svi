#pragma once

enum status {
	OK = 0,
	ERROR = 1,
	USER_ERROR = 2,
	OS_ERROR = -1
};

void fatal(char *, void (*)(enum status), enum status);
