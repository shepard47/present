#include <present.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xfixes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

extern void grinit();
extern void resized();

Window win;
static Atom del;
static XEvent e;
static Cursor c;
struct Dormer dm;
static XIM xim;
static XIC xic;

static void
die(char *func)
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
		die("XOpenDisplay");
	grinit();
	dm.win = &win;
	XStoreName(dm.dis, win, label);
	if(dm.screen)
		fscr();
	XMapWindow(dm.dis, win);

	setcurs(dm.cursor);

	xim = XOpenIM(dm.dis, 0, 0, 0);
	if(xim != 0)
		xic = XCreateIC(xim, XNInputStyle, XIMPreeditNothing|XIMStatusNothing,
				XNClientWindow, win, XNFocusWindow, win, (void*)0);

	del = XInternAtom(dm.dis, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dm.dis, win, &del, 1);
}

static int
cursor(int curs)
{
	switch(curs){
	case Chide:
		XFixesHideCursor(dm.dis, DefaultRootWindow(dm.dis));
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
	return 0;
}

void
setcurs(int curs)
{
	dm.cursor = curs;
	c = XCreateFontCursor(dm.dis, cursor(curs));
	XDefineCursor(dm.dis, win, c);
}

static void
rev(void *start, int size)
{
	char *lo = start;
	char *hi = start + size - 1;
	char swap;
	while (lo < hi){
		swap = *lo;
		*lo++ = *hi;
		*hi-- = swap;
	}
}

void
handle(void)
{
	XNextEvent(dm.dis, &e);
	if (XFilterEvent(&e, None))
		handle();
	switch(e.type){
	case FocusIn:
		if(xic != 0)
			XSetICFocus(xic);
		break;
	case FocusOut:
		if (xic != 0)
			XUnsetICFocus(xic);
		break;
	case KeyPress:
		dm.ev = 2;

		int len;
		char buf[4];
		KeySym key;
		Status status;

		if(xic != 0)
			len = Xutf8LookupString(xic, &e.xkey, buf, 4, &key, &status);
		rev(buf, strlen(buf));
	
		if(buf != 0)
			break;
		switch(key){
		case XK_BackSpace:
			dm.key = 0x08;
			break;
		case XK_Tab:
			dm.key = 0x09;	
			break;
		case XK_Linefeed:
			dm.key = 0x0a;
			break;
		case XK_Clear:
			puts("clear");
			break;
		case XK_Return:
			puts("return");
			break;
		case XK_Pause:
			puts("pause");
			break;
		case XK_Escape:
			dm.key = 0x1b;
			break;
		case XK_Shift_L:
			puts("left shift");
			break;
		case XK_Shift_R:
			puts("right shift");
			break;
		case XK_Control_L:
			puts("left control");
			break;
		case XK_Control_R:
			puts("right control");
			break;
		case XK_Super_L:
			puts("left super");
			break;
		case XK_Super_R:
			puts("right super");
			break;
		case XK_Alt_L:
			puts("left alt");
			break;
		case XK_Alt_R:
			puts("right alt");
			break;
		}
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

int
usec(void)
{
	struct timeval t;
	gettimeofday(&t, 0);
	return t.tv_sec * 1000000 + t.tv_usec;	
}
