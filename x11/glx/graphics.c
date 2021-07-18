#include <present.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <aux/shader.h>

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
glXCreateContextAttribsARBProc glXCreateContextAttribsARB;

extern Window win;
extern void	flextInit(void);
extern int prog;

void
grinit(void)
{
	int scr = XDefaultScreen(dm.dis);

	GLXFBConfig *fbc;
	int fbc_attr[] = {
		GLX_X_RENDERABLE, True,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_RENDER_TYPE,	GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE,	8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		GLX_STENCIL_SIZE, 8,
		GLX_DOUBLEBUFFER, True,
		None
	};
	int fbcnt;
	fbc = glXChooseFBConfig(dm.dis, scr, fbc_attr, &fbcnt);
	int best_fbc = -1, worst_fbc = -1, best_samp = -1, worst_samp = 999;
	int i;
	for (i = 0; i < fbcnt; ++i) {
		XVisualInfo *tvi = glXGetVisualFromFBConfig(dm.dis, fbc[i]);
		if (tvi) {
			int samp_buf, samples;
			glXGetFBConfigAttrib(dm.dis, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
			glXGetFBConfigAttrib(dm.dis, fbc[i], GLX_SAMPLES, &samples);
			if (best_fbc < 0 || (samp_buf && samples > best_samp)) {
				best_fbc = i;
				best_samp = samples;
			}
			if (worst_fbc < 0 || !samp_buf || samples < worst_samp) {
				worst_fbc = i;
				worst_samp = samples;
			}
		}
	}
	GLXFBConfig best = fbc[best_fbc];
	XVisualInfo *vi = glXGetVisualFromFBConfig(dm.dis, best);
	Visual *vis = vi->visual;
	Window root = XRootWindow(dm.dis, vi->screen);
	Colormap cmap = XCreateColormap(dm.dis, root, vis, AllocNone);

	int mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.background_pixel = BlackPixel(dm.dis, scr);
	swa.background_pixmap = None;
	swa.border_pixel = 0;
	swa.event_mask =	KeyPressMask	| KeyReleaseMask	|
						ButtonPressMask	| ButtonReleaseMask	|
						EnterWindowMask	| LeaveWindowMask	|
						ExposureMask	| PointerMotionMask | FocusChangeMask;

	win = XCreateWindow(dm.dis, root, 0, 0, dm.width, dm.height, 0, 0, InputOutput, vis, mask, &swa);

	int ctx_attr[] = {
		0x2091, 3,
		0x2092, 3,
		None
	};
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const unsigned char*)"glXCreateContextAttribsARB");
	GLXContext ctx = glXCreateContextAttribsARB(dm.dis, best, 0, True, ctx_attr);
	glXMakeCurrent(dm.dis, win, ctx);
	XFree(fbc);
	XFree(vi);
	
	flextInit();

	mkprog(&prog, &svert, &sfrag);
}

void
swapbuf(void)
{
	glXSwapBuffers(dm.dis, win);
}
