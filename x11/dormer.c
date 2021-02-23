#include <present.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>

extern void grinit();
extern void resized();

Window win;
static Atom del;
static XEvent e;
static Cursor c;

static void
fail(char *func)
{
	perror(func);
	XCloseDisplay(dm.dis);
	exit(-1);
}

static void
fscr(void)
{
		XWMHints hints;
		hints.flags = 2;
		Atom state = XInternAtom(dm.dis, "_NET_WM_STATE", True);
		Atom fscr = XInternAtom(dm.dis, "_NET_WM_STATE_FULLSCREEN", True);
		XChangeProperty(dm.dis, win, state, XA_ATOM, 32, PropModeReplace, (unsigned char *)&fscr, 1);
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

	setcurs(dm.cursor);
	del = XInternAtom(dm.dis, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dm.dis, win, &del, 1);
}

static int
cursor(int curs)
{
	switch(curs){
	case Chide:
		/*XFixesHideCursor(dm.dis, DefaultRootWindow(dm.dis));*/
		break;
	case Carrow:
		return 2;
	case Ccross:
		return 34;
	case Cload:
		return 150;
	case Ctext:
		return 152;
	case Chand:
		return 58;
	case Cno:
		return 24;
	case Chelp:
		return 92;
	case Cmove:
		return 52;
	}
}

void
setcurs(int curs)
{
	dm.cursor = curs;
	c = XCreateFontCursor(dm.dis, cursor(curs));
	XDefineCursor(dm.dis, win, c);
}

void
handle(void)
{
	XNextEvent(dm.dis, &e);
	switch(e.type){
	case KeyPress:
		dm.ev = 2;
		dm.key = XkbKeycodeToKeysym(dm.dis, e.xkey.keycode, 0, 0); /* TODO: UTF-8 */
		break;
	case ButtonPress:
		dm.ev = 1;
		switch(e.xbutton.button){
		case 1:
			dm.btn = 1;
			break;
		case 2:
			dm.btn = 2;
			break;
		case 3:
			dm.btn = 4;
			break;
		case 4:
			dm.btn = 8;
			break;
		case 5:
			dm.btn = 16;
			break;
		}
		break;
	case Expose:
		dm.width = e.xexpose.width;
		dm.height = e.xexpose.height;
		resized();
		break;
	case MotionNotify:
		dm.ev = 1;
		dm.btn = 0;
		dm.x = (float)(e.xbutton.x)/(float)(dm.width)*2-1;
		dm.y = -((float)(e.xbutton.y)/(float)(dm.height)*2-1);
		break;
	case ClientMessage:
		if(e.xclient.data.l[0] == del){
			if(closed())
				exit(0);
		}
	}
}

void
readev(void)
{
	if(XPending(dm.dis) > 0)
		handle();
	else
		dm.ev = 0;
}
