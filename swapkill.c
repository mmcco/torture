/*
 * Copyright (c) 2010 Thordur I. Bjornsson <thib@foo.is> 
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>
#include <sys/queue.h>
#include <sys/wait.h>

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "torture.h"

static void	child(size_t);

struct mem {
	LIST_ENTRY(mem)	mem_next;
	char		mem_data[1024 * 1024];	/* 1 MB */
};

LIST_HEAD(, mem)	mem_head = LIST_HEAD_INITIALIZER(mem_head);

static void
child(size_t size)
{
	struct mem	*mp;
	int		 i;

	/* Allocate size MBs in 1MB chunks. */
	do {
		mp = malloc(sizeof(struct mem));
		if (mp == NULL)  {
			warnx("swap: malloc()");	/* XXX */
			break;
		}

		for (i = 0; i != (1024 * 1024); i++)
			mp->mem_data[i] = 'T';

		LIST_INSERT_HEAD(&mem_head, mp, mem_next);
		size--;

	} while (size);
}

int
swapkill(int pargc, char *pargv[])
{
	const char	*errstr;
	size_t		 size;
	pid_t		 pid;
	unsigned int	 waitfor;
	int		 ch, numprocs, i;

	numprocs = 4;	/* Four procs */
	size = 512;	/* 512M each */
	waitfor = 300;	/* Sleep for 5mins */

	while ((ch = getopt(pargc, pargv, "hn:s:w:")) != -1) {
		switch (ch) {
		case 'n':
			numprocs = (int)strtonum(optarg, 1, 128, &errstr);
			if (errstr)
				errx(1, "# of procs out of range: %s", errstr);
			break;
		case 's':
			size = (size_t)strtonum(optarg, 1, 8192, &errstr);
			if (errstr)
				errx(1, "size is out of range: %s", errstr);
			break;
		case 'w':
			waitfor = (unsigned int)strtonum(optarg, 1, 7200, &errstr);
			if (errstr)
				errx(1, "wait is out of range: %s", errstr);
			break;
		default:
			usage(SWAP_USAGE);
			/* NOTREACHED */
		}
	}
	pargc -= optind;
	pargv += optind;

	for (i = 0; i != numprocs; i++) {
		switch (pid = fork()) {
		case -1:
			warnx("failed to fork child %i", i);
			break;
		case 0:
			/* child */
			child(size);
			sleep(waitfor);
			break;
		default:
			break;
		}
	}

	while (waitpid(WAIT_ANY, NULL, 0)) {
		if (errno == ECHILD)
			break;
	}

	return (0);
}
