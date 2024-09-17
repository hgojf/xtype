PROG = xtype
LDADD = -lX11 -lXtst

CFLAGS += -I/usr/X11R6/include
LDFLAGS += -L/usr/X11R6/lib

DESTDIR = /usr/local
MANDIR = /man/man

.include <bsd.prog.mk>
