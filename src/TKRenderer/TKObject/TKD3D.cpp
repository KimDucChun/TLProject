#include "TKD3D.h"
#include "TKEngine.h"
#include "DirectXColors.h"
//#include "TKCamera.h"

TKD3D::TKD3D(void)
	:TKBaseObject(-1)
	,m_pD3D(nullptr)
	//,m_pD3D1(nullptr)
	,m_pD3DImmediateContext(nullptr)
	//,m_pD3DImmediateContext1(nullptr)
	,m_pSwapChain(nullptr)
	//,m_pSwapChain1(nullptr)
	,m_pDepthStencilBuffer(nullptr)
	,m_pRenderTargetView(nullptr)
	,m_pDepthStencilView(nullptr)
	,pCamera(NULL)
{
}

TKD3D::~TKD3D(void)
{
	SAFERELEASE(m_pD3D);
	//SAFERELEASE(m_pD3D1);
	SAFERELEASE(m_pD3DImmediateContext);
	//SAFERELEASE(m_pD3DImmediateContext1);
	SAFERELEASE(m_pSwapChain);
	//SAFERELEASE(m_pSwapChain1);
	SAFERELEASE(m_pDepthStencilBuffer);
	SAFERELEASE(m_pRenderTargetView);
	SAFERELEASE(m_pDepthStencilView);
}

void TKD3D::Init(void)
{
	__super::Init();

	D3D_DRIVER_TYPE _createDriverType;
	UINT _createDeviceFlags = 0;
#ifdef _DEBUG
	_createDriverType = D3D_DRIVER_TYPE_HARDWARE;
	_createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#else
	_createDriverType = D3D_DRIVER_TYPE_HARDWARE;
#endif

	D3D_FEATURE_LEVEL _featureLevels[] =
	{
		//D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT _numFeatureLevels = ARRAYSIZE(_featureLevels);

	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr = D3D11CreateDevice(0, _createDriverType, 0, _createDeviceFlags, _featureLevels, _numFeatureLevels, 
		D3D11_SDK_VERSION, &m_pD3D, &featureLevel, &m_pD3DImmediateContext);
	if (FAILED(hr))
		return;

	// 이부분은 DXTutorial에서 복붙
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = m_pD3D->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* dxgiAdapter = nullptr;
			hr = dxgiDevice->GetAdapter(&dxgiAdapter);
			if (SUCCEEDED(hr))
			{
				hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				dxgiAdapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr))
		return;

	m_m4xMsaaQuality = 0;
	hr = m_pD3D->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_m4xMsaaQuality);
	if (FAILED(hr))
		return;

	assert(m_m4xMsaaQuality > 0);

#if 0
	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		hr = m_pD3D->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_pD3D1));
		if (SUCCEEDED(hr))
		{
			(void)m_pD3DImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_pD3DImmediateContext1));
		}

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = SCREEN_WIDTH;
		sd.Height = SCREEN_HEIGHT;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//sd.SampleDesc.Count = 1;
		//sd.SampleDesc.Quality = 0;
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_m4xMsaaQuality - 1;

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(m_pD3D, GetEngine()->GethWnd(), &sd, nullptr, nullptr, &m_pSwapChain1);
		if (SUCCEEDED(hr))
		{
			hr = m_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_pSwapChain));
		}

		dxgiFactory2->Release();
	}
	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = SCREEN_WIDTH;
		sd.BufferDesc.Height = SCREEN_HEIGHT;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = GetEngine()->GethWnd();
		//sd.SampleDesc.Count = 1;
		//sd.SampleDesc.Quality = 0;
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_m4xMsaaQuality - 1;

		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(m_pD3D, &sd, &m_pSwapChain);
	}
#else
	DXGI_SWAP_CHAIN_DESC	sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 4x msaa
	sd.SampleDesc.Count = 4;
	sd.SampleDesc.Quality = m_m4xMsaaQuality - 1;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = GetEngine()->GethWnd();
#ifdef WINDOW_MODE
	sd.Windowed = true;
#else
	sd.Windowed = false;
#endif
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	hr = dxgiFactory->CreateSwapChain(m_pD3D, &sd, &m_pSwapChain);
	if (FAILED(hr))
		return;

#endif

	SAFERELEASE(dxgiFactory);

	InitRenderTargetView();
	InitDepthStencilBufferView();
	InitViewPort();
}

void TKD3D::Release(void)
{
	SAFERELEASE(m_pD3D);
	//SAFERELEASE(m_pD3D1);
	SAFERELEASE(m_pD3DImmediateContext);
	//SAFERELEASE(m_pD3DImmediateContext1);
	SAFERELEASE(m_pSwapChain);
	//SAFERELEASE(m_pSwapChain1);
	SAFERELEASE(m_pDepthStencilBuffer);
	SAFERELEASE(m_pRenderTargetView);
	SAFERELEASE(m_pDepthStencilView);
    __super::Release();
}

void TKD3D::InitRenderTargetView(void)
{
	SAFERELEASE(m_pRenderTargetView);

	ID3D11Texture2D * pBackBuffer;
	HRESULT hr;
	
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return;

	hr = m_pD3D->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	if (FAILED(hr))
		return;

	SAFERELEASE(pBackBuffer);
}

void TKD3D::InitDepthStencilBufferView(void)
{
	SAFERELEASE(m_pDepthStencilBuffer);
	SAFERELEASE(m_pDepthStencilView);

	D3D11_TEXTURE2D_DESC	depthStencilDesc;
	depthStencilDesc.Width = SCREEN_WIDTH;
	depthStencilDesc.Height = SCREEN_HEIGHT;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 4x msaa
	depthStencilDesc.SampleDesc.Count = 4;
	depthStencilDesc.SampleDesc.Quality = m_m4xMsaaQuality - 1;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HRESULT hr;

	hr = m_pD3D->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencilBuffer);
	if (FAILED(hr))
		return;

	hr = m_pD3D->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView);
	if (FAILED(hr))
		return;

	m_pD3DImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
}

void TKD3D::InitViewPort(void)
{
	D3D11_VIEWPORT	vp;

	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = SCREEN_WIDTH;
	vp.Height = SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_pD3DImmediateContext->RSSetViewports(1, &vp);
}

void TKD3D::SetCamera(TKCamera *pCamera)
{
    this->pCamera = pCamera;
}

TKCamera *TKD3D::GetCamera(void)
{
    return pCamera;
}

XMMATRIX * TKD3D::GetMatProjection(void)
{
    return &matProjection;
}

void TKD3D::Frame(void)
{
	RT_Clear();

	HRESULT hr = m_pSwapChain->Present(0, 0);
	if (FAILED(hr))
		return;
}

void TKD3D::RT_Clear()
{
	//FLOAT ClearColor[4];
	//ClearColor[0] = 0.0f;
	//ClearColor[1] = 0.0f;
	//ClearColor[2] = 0.0f;
	//ClearColor[3] = 0.0f;
	m_pD3DImmediateContext->ClearRenderTargetView(m_pRenderTargetView, Colors::MidnightBlue);
}