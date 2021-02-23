#include <present.h>
#define COBJMACROS
#define CINTERFACE
#include <d3d11.h>

extern IDXGISwapChain *sc;
extern ID3D11Device *dev;
extern ID3D11DeviceContext *ctx;
extern ID3D11RenderTargetView *backbuf;
float col[] = { 0,0.2,0.4,1 };

void
present(void)
{
	ID3D11DeviceContext_ClearRenderTargetView(ctx, backbuf, col);
	IDXGISwapChain_Present(sc, 0, 0);
}