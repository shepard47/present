#include <present.h>
#define COBJMACROS
#define CINTERFACE
#include <d3d11.h>
//#include <d3dcompiler.h>
#include <string.h>

IDXGISwapChain *sc;
ID3D11Device *dev;
ID3D11DeviceContext *ctx;
ID3D11RenderTargetView *backbuf;
ID3D11InputLayout *layout;
ID3D11VertexShader *vert;
ID3D11PixelShader *frag;

void
mkprog(void)
{
	/*ID3D10Blob *vs, *fs;

	D3DReadFileToBlob(VERT, &vs);
	D3DReadFileToBlob(FRAG, &fs);

	ID3D11Device_CreateVertexShader(dev, ID3D10Blob_GetBufferPointer(vs), ID3D10Blob_GetBufferSize(vs), 0, &vert);
	ID3D11Device_CreatePixelShader(dev, ID3D10Blob_GetBufferPointer(fs), ID3D10Blob_GetBufferSize(fs), 0, &frag);
	ID3D11DeviceContext_VSSetShader(ctx, vert, 0, 0);
	ID3D11DeviceContext_PSSetShader(ctx, frag, 0, 0);*/
}

void
grinit(void)
{
	DXGI_SWAP_CHAIN_DESC scd;
	memset(&scd, 0, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = dm.win;
	scd.SampleDesc.Count = 4;
	scd.Windowed = 1;
	D3D11CreateDeviceAndSwapChain(0,D3D_DRIVER_TYPE_HARDWARE,0,0,0,0,D3D11_SDK_VERSION,&scd,&sc,&dev,0,&ctx);

	ID3D11Texture2D *pbackbuf;
	IDXGISwapChain_GetBuffer(sc, 0, &IID_ID3D11Texture2D, (void*)&pbackbuf);
	ID3D11Device_CreateRenderTargetView(dev, pbackbuf, 0, &backbuf);
	ID3D11Texture2D_Release(pbackbuf);
	ID3D11DeviceContext_OMSetRenderTargets(ctx, 1, &backbuf, 0);

	D3D11_VIEWPORT viewport;
	memset(&viewport, 0, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;
	ID3D11DeviceContext_RSSetViewports(ctx, 1, &viewport);

	mkprog();
}

void
swapbuf(void)
{
	IDXGISwapChain_Present(sc, 0, 0);
}

