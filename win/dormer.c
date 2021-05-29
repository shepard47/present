#include <present.h>
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>

extern void grinit();
extern void resized();

struct Dormer dm;
HINSTANCE inst;
HWND win;
WNDCLASSEX wc;
MSG e;
DEVMODE dmode;

long long __stdcall
winproc(HWND win, UINT etype, WPARAM wp, LPARAM lp)
{
	switch(etype){
	case WM_CHAR:
		dm.ev = 2;
		dm.key = wp; /* UTF-16 */
		/*char *c;
		WideCharToMultiByte(CP_ACP, 0, &wp, -1, c, 1, 0, 0);*/
		break;
	case WM_LBUTTONDOWN:
		dm.ev = 1;
		dm.btn = 1;
		break;
	case WM_LBUTTONUP:
		dm.ev = 1;
		dm.btn = 0;
		break;
	case WM_RBUTTONDOWN:
		dm.ev = 1;
		dm.btn = 4;
		break;
	case WM_RBUTTONUP:
		dm.ev = 1;
		dm.btn = 0;
		break;
	case WM_MBUTTONDOWN:
		dm.ev = 1;
		dm.btn = 2;
		break;
	case WM_MBUTTONUP:
		dm.ev = 1;
		dm.btn = 0;
		break;
	case WM_MOUSEWHEEL:
		dm.ev = 1;
		if(GET_WHEEL_DELTA_WPARAM(wp) > 0)
			dm.btn = 8;
		else
			dm.btn = 16;
		break;
	case WM_MOUSEMOVE:
		dm.ev = 1;
		dm.btn = 0;
		dm.x = (float)GET_X_LPARAM(lp)/(float)(dm.width)*2-1;
		dm.y = -(float)(GET_Y_LPARAM(lp)/(float)(dm.height)*2-1);
		break;
	case WM_SIZE:
		dm.width = LOWORD(lp);
		dm.height = HIWORD(lp);
		resized();
		break;
	case WM_CLOSE:
		if(closed())
			DestroyWindow(win);
		break;
	case WM_DESTROY:
		exit(0);
		break;
	default:
		return DefWindowProc(win, etype, wp, lp);
	}
}

static int
cursor(void)
{
	switch(dm.cursor){
	case Chide:
		ShowCursor(0);
		return 32512;
	case Carrow:
		return 32512;
	case Ccross:
		return 32515;
	case Cload:
		return 32514;
	case Ctext:
		return 32513;
	case Chand:
		return 32649;
	case Cno:
		return 32648;
	case Chelp:
		return 32651;
	case Cmove:
		return 32646;
	default:
		return 32512;
	}
}

void
setcurs(int curs)
{
	dm.cursor = curs;
	SetCursor(LoadCursor(0, MAKEINTRESOURCE(cursor())));
}

static void
fscr(void)
{
	memset(&dmode, 0, sizeof(dmode));
	dmode.dmSize = sizeof(dmode);
	dmode.dmPelsWidth = (unsigned long)GetSystemMetrics(SM_CXSCREEN);
	dmode.dmPelsHeight = (unsigned long)GetSystemMetrics(SM_CYSCREEN);
	dm.width = GetSystemMetrics(SM_CXSCREEN);
	dm.height = GetSystemMetrics(SM_CYSCREEN);
	dmode.dmBitsPerPel = 32;
	dmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	ChangeDisplaySettings(&dmode, CDS_FULLSCREEN);
}

void
winit(char *label)
{
	if(dm.width == 0 || dm.height == 0){
		dm.width = 800;
		dm.height = 600;
	}

	inst = GetModuleHandle(0);
	
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = winproc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = inst;
	wc.hIcon = LoadIcon(0, IDI_WINLOGO); /* custom icon */
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(0, MAKEINTRESOURCE(cursor()));
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = label;
	wc.cbSize = sizeof(WNDCLASSEX);
	if(!RegisterClassEx(&wc)){
		perror("RegisterClassEx");
		exit(-1);
	}

	int style = WS_OVERLAPPED;
	if(dm.screen){
		fscr(); /* handle multiple screens */
		style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
	}

	dm.win = win = CreateWindowEx(WS_EX_APPWINDOW, label, label, style, 0, 0, dm.width, dm.height, 0, 0, inst, 0);
	if(win == 0){
		perror("CreateWindowEx");
		exit(-1);
	}

	grinit();

	ShowWindow(win, SW_SHOW);
	UpdateWindow(win);
	SetForegroundWindow(win);
	SetFocus(win);
}

void
readev(void)
{
	if(PeekMessage(&e, 0, 0, 0, PM_REMOVE)){
		TranslateMessage(&e);
		DispatchMessage(&e);
	}else{
		dm.ev = 0;
	}
}
