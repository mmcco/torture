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

#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "torture.h"

static void	child(int);

char		*outdir = "filbuf";
size_t		 size, bufsize;
int		 numfiles, numprocs;

int
filbuf(int pargc, char *pargv[])
{
	const char	*errstr;
	pid_t		 pid;
	int		 ch, i, error;

	size = 64 * 1024 * 1024;
	bufsize = 64 * 1024;
	numfiles = 64;
	numprocs = 8;

	while ((ch = getopt(pargc, pargv, "d:s:n:f")) != -1) {
		switch (ch) {
		case 'd':
			outdir = optarg;
			/* XXX */
			break;
		case 's':
			size = (size_t)strtonum(optarg, 1, 8196, &errstr);
			if (errstr)
				errx(1, "size out of range: %s", errstr);
			break;
		case 'n':
			numprocs = (int)strtonum(optarg, 1, 128, &errstr);
			if (errstr)
				errx(1, "# of procs out of range: %s", errstr);
			break;
		case 'f':
			numfiles = (int)strtonum(optarg, 1, 8196, &errstr);
			if (errstr)
				errx(1, "# of files out of range: %s", errstr);
			break;
		default:
			usage(FILBUF_USAGE);
			/* NOTREACHED */
		}
	}
	pargc -= optind;
	pargv += optind;

	error = mkdir(outdir, 0760);
	if (error && errno != EEXIST)
		errx(1, "can't create outdir");

	for (i = 0; i != numprocs; i++) {
		switch (pid = fork()) {
		case -1:
			warn("failed to fork child %i", i);
			break;
		case 0:
			child(i);
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

void
child(int childno)
{
	off_t	 off, len;
	char	*buf, *cbuf;
	char	 filename[MAXPATHLEN];
	int	 fd, i, ret;

	buf = malloc(bufsize);
	cbuf = malloc(bufsize);
	if (buf == NULL || cbuf == NULL)
		err(1, "child %i", childno);

	for (i = 0; i != numfiles; i++) {
		snprintf(filename, MAXPATHLEN, "%s/child%i.%i",
		    outdir, childno, i);

		fd = open(filename, O_RDWR|O_CREAT, 0640);
		if (fd == -1)
			err(1, "can't open file %s", filename);

		memset(buf, i, bufsize);
		
		for (off = 0; off < size; off += len) {
			len = bufsize;
			if ((off + len) > size)
				len = size - off;

			ret = write(fd, buf, len);
			if (ret != len)
				err(1, "error writing to %s", filename);
		}

		close(fd);
	}

	sync(); sleep(1); sync();

	off = 0;
	len = 0;

	for (i = 0; i != numfiles; i++) {
		snprintf(filename, MAXPATHLEN, "%s/child%i.%i",
		    outdir, childno, i);

		fd = open(filename, O_RDONLY, 0640);
		if (fd == -1)
			err(1, "can't open file %s", filename);

		memset(cbuf, i, bufsize);

		for (off = 0; off < size; off += len) {
			len = bufsize;
			if ((off + len) > size)
				len = size - off;

			ret = read(fd, buf, len);
			if (ret != len)
				err(1, "can't read from %s", filename);

			if (memcmp(buf, cbuf, len)) {
				/*
				 * Need a good way to log the output
				 * if this happens.
				 */
				errx(1, "write/read mistmach for %s",
				    filename);
			}

			close(fd);
		}
	}
}
