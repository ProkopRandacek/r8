#include <ucw/lib.h>

#include <stdio.h>

#include "common.h"
#include "autoconf.h"

int main(int argc, char* argv[]) {
	log_init(argv[0]);
	//log_file("log");
	msg(L_INFO, "start");
	msg(L_INFO, "common.h? " __commontesttmp);
#ifdef CONFIG_LINUX
	msg(L_INFO, "ucw said that im running on linux");
#else
	msg(L_INFO, "ucw didnt say that im running on linux");
#endif
	return 0;
}
