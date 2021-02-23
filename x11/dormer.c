#include <present.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

extern void grinit();
extern void resized();

Window win;
static Atom del;

void
fail(char *func)
{
	perror(func);
	XCloseDisplay(dm.dis);
	exit(-1);
}

void
fscr(void)
{
	/* fullscreen */
}

void
winit(char *label)
{
	dm.dis = XOpenDisplay(0);
	if(dm.dis == 0)
		fail("XOpendDisplay");
	grinit();
	dm.win = &win;
	XStoreName(dm.dis, win, label);
	if(dm.screen)
		fscr();
	XMapWindow(dm.dis, win);

	del = XInternAtom(dm.dis, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dm.dis, win, &del, 1);
}

void
readev(void)
{

}
