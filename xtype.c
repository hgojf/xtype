/*
 * Copyright (c) 2024 Henry Ford <fordhenry2299@gmail.com>

 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.

 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define XK_MISCELLANY

#include <err.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <X11/extensions/XTest.h>
#include <X11/keysymdef.h>
#include <X11/Xlib.h>

static int xfakekey(Display *, int);

static void usage(void);

int
main(int argc, char *argv[])
{
	Display *dpy;
	int ch, evb, erb, ma, mi;

	while ((ch = getopt(argc, argv, "")) != -1) {
		switch (ch) {
		default:
			usage();
		}
	}

	argc -= optind;
	argv += optind;

	if (argc != 0)
		usage();

	if ((dpy = XOpenDisplay(NULL)) == NULL)
		errx(1, "failed to open display");

	if (pledge("stdio", NULL) == -1)
		err(1, "pledge");

	if (!XTestQueryExtension(dpy, &evb, &erb, &ma, &mi))
		errx(1, "X server has no support for XTest");

	while ((ch = fgetc(stdin)) != EOF) {
		if (xfakekey(dpy, ch) == -1)
			errx(1, "couldnt fake key");
	}

	XSync(dpy, 1);

	XCloseDisplay(dpy);
}

static int
xfakekey(Display *dpy, int ch)
{
	int code, mod;

	switch (ch) {
	case '!':
	case '@':
	case '#':
	case '%':
	case '^':
	case '&':
	case '*':
	case '(':
	case ')':
	case '_':
	case '+':
	case '~':
	case '{':
	case '}':
	case '|':
	case ':':
	case '"':
	case '?':
	case '>':
		code = XKeysymToKeycode(dpy, ch);
		mod = XKeysymToKeycode(dpy, XK_Shift_L);
		break;
	case '\n':
		code = XKeysymToKeycode(dpy, XK_KP_Enter);
		mod = -1;
		break;
	default:
		if (ch <= 0x20)
			return 0;
		if (ch >= 0x80)
			return -1;
		mod = -1;
		if (isupper(ch))
			mod = XKeysymToKeycode(dpy, XK_Shift_L);
		code = XKeysymToKeycode(dpy, ch);
		break;
	}

	if (mod != -1)
		if (!XTestFakeKeyEvent(dpy, mod, True, 0))
			return -1;

	if (!XTestFakeKeyEvent(dpy, code, True, 0))
		return -1;
	if (!XTestFakeKeyEvent(dpy, code, False, 0))
		return -1;

	if (mod != -1)
		if (!XTestFakeKeyEvent(dpy, mod, False, 0))
			return -1;

	return 0;
}

static void
usage(void)
{
	fprintf(stderr, "usage: xtype\n");
	exit(2);
}
