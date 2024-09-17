PROG = xtype
LDADD = -lX11 -lXtst

CFLAGS += -I/usr/X11R6/include
LDFLAGS += -L/usr/X11R6/lib

.include <bsd.prog.mk>
