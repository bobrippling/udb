#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "daemon.h"
#include "util.h"

void daemon_fork()
{
	if(isatty(0)){
		switch(fork()){
			case -1:
				die("fork():");
			case 0:
				close(0);
				close(1);
				close(2);
				break;

			default:
				exit(0); /* parent */
		}
	}
}

void daemon_init_dir(char *dir)
{
	if(mkdir_p(dir))
		die("mkdir %s:", dir);
}

void daemon_create_io(char *dir)
{
	if(chdir(dir))
		die("chdir %s:", dir);

	freopen("out", "w", stdout);

	if(mkfifo(FIFO, 0600) == -1 && errno != EEXIST)
		die("mkfifo(\"%s\"):", FIFO);
}

void sdb_vprintf(const char *fmt, va_list l)
{
	vprintf(fmt, l); /* stdout -> log */
}

void sdb_printf(const char *fmt, ...)
{
	va_list l;
	va_start(l, fmt);
	sdb_vprintf(fmt, l);
	va_end(l);
}