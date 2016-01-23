PROG=	torture

SRCS=	torture.c	\
	swapkill.c	\
	iogen.c		\
	filbuf.c	\
	coredumper.c

MAN=	torture.8

BINDIR=	/usr/local/sbin

CFLAGS+=	-g -Wall -Werror -I${.CURDIR}
CFLAGS+=	-Wshadow -Wpointer-arith -Wcast-qual -Wbounded
#CFLAGS+=	-Wstrict-prototypes -Wmissing-prototypes
#CFLAGS+=	-Wmissing-declarations
#CFLAGS+=	-Wsign-compare

.include <bsd.prog.mk>
