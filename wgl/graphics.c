#include <present.h>
#include <windows.h>

HDC dc;

void
grinit()
{
	dc = GetDC(dm.win);

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), 1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA, 32,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
	};
	int fm = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, fm, &pfd);
	HGLRC old = wglCreateContext(dc);
	wglMakeCurrent(dc, old);

	int attr[] = {
		0x2091, 3,
		0x2092, 3,
		0x9126, 0x0001,
		0x2094, 0x0002,
		0
	};
	
	HGLRC ctx = ((HGLRC(*)(HDC, HGLRC, int*))(wglGetProcAddress("wglCreateContextAttribsARB")))(dc, old, attr);
	wglMakeCurrent(dc, ctx);
	wglDeleteContext(old);

	mkprog();
}

void
swapbuf(void)
{
	SwapBuffers(dc);
}