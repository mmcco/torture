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

#ifndef __TORTURE_H__
#define	__TORTURE_H__

#define	VERSION		"1.0"

#define	SWAP_USAGE	"swap [-h] [-n numprocs | -s size | -w wait]"
int		swapkill(int, char **);

/* XXX: Omitting iogen version number */
#define	IOGEN_VERSION	"3.1"
#define	IOGEN_USAGE							\
	"iogen [-rk] [-s size] [-b size] [-p percentage] "		\
	    "[-d path] [-f path] [-n processes] [-t interval]\n"	\
	"-h this help\n"						\
	"-s <file size> [k|m|g]; Default = 1g\n"			\
	"-b <io size> [k|m]; Default = 64k\n"				\
	"-p <read percentage>; Default = 50\n"				\
	"-r randomize io block size; Default = no\n"			\
	"-d <target directory>; Default = current directory\n"		\
	"-f <result directory>; Default = iogen.res\n"			\
	"-n <number of io processes>; Default = 1\n"			\
	"-t <seconds between update>; Default = 60 seconds\n"		\
	"-T <seconds to timeout io>; Default = disabled\n"		\
	"-P <payload pattern>; ? displays patterns, Default = readable text\n" \
	"-k kill all running io processes\n\n"				\
	"If parameters are omited defaults will be used.\n"
int		iogen(int, char **);

#define	FILBUF_USAGE	"filbuf [-oi] [ -s size[{k|m|g}] ] [ -p idpattern ]"
int		filbuf(int, char **);

#define	COREDUMPER_USAGE	"coredumper size nproc dir"
int		coredumper(int, char **);

#define	MAIN_USAGE	"<command> [options ...]"
__dead void	usage(const char *);

#endif	/* !__TORTURE_H__ */
