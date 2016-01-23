/*
 * Copyright (c) 2013 Bob Beck <beck@obtuse.com>
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

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "torture.h"

int
main(int argc, char *argv[])
{
	char	**pargv;
	int	  error, pargc;

	pargv = NULL;
	error = pargc = 0;

	if (argc < 2)
		usage(MAIN_USAGE);

	/* torture test called with no options. */
	if (argc == 2) {
		pargc = 0;
		pargv = NULL;
	} else {
		/* Pass the program name to. */
		pargc = argc - 1;
		pargv = &argv[1];
	}
	if (strncmp("swap", argv[1], 4) == 0)
		error = swapkill(pargc, pargv);
	else if (strncmp("iogen", argv[1], 5) == 0)
		error = iogen(pargc, pargv);
	else if (strncmp("filbuf", argv[1], 6) == 0)
		error = filbuf(pargc, pargv);
	else if (strncmp("coredumper", argv[1], 10) == 0)
		error = coredumper(pargc, pargv);
	else
		usage(MAIN_USAGE);
	return (error);
}

__dead void
usage(const char *cmdstr)
{
	extern char	*__progname;

	fprintf(stderr, "%s version %s\n", __progname, VERSION);
	fprintf(stderr, "usage: %s %s\n", __progname, cmdstr);

	exit(1);
}
