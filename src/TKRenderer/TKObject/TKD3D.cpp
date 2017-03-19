#include "TKD3D.h"
#include "TKEngine.h"
//#include "TKCamera.h"

TKD3D::TKD3D(TKEngine *pEngine)
	:TKBaseObject(pEngine, -1)
#if defined(TK_D3D11)
	,m_pD3D(nullptr)
	,m_pD3DImmediateContext(nullptr)
	,m_pSwapChain(nullptr)
	,m_pDepthStencilBuffer(nullptr)
	,m_pRenderTargetView(nullptr)
	,m_pDepthStencilView(nullptr)
#elif defined(TK_D3D9)
	,m_pD3D(NULL)
	,m_pD3DDevice(NULL)
#endif
	,pCamera(NULL)
{
}

TKD3D::~TKD3D(void)
{
#if defined(TK_D3D11)
	SAFERELEASE(m_pD3D);
	SAFERELEASE(m_pD3DImmediateContext);
	SAFERELEASE(m_pSwapChain);
	SAFERELEASE(m_pDepthStencilBuffer);
	SAFERELEASE(m_pRenderTargetView);
	SAFERELEASE(m_pDepthStencilView);
#elif defined(TK_D3D9)
	SAFERELEASE(m_pD3DDevice);
    SAFERELEASE(m_pD3D);
#endif
}

void TKD3D::Init(void)
{
    __super::Init();

#if defined(TK_D3D11)

	D3D_DRIVER_TYPE _createDriverType;
	UINT _createDeviceFlags = 0;
#ifdef _DEBUG
	_createDriverType = D3D_DRIVER_TYPE_WARP;
	_createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#else
	_createDriverType = D3D_DRIVER_TYPE_HARDWARE;
#endif

	D3D_FEATURE_LEVEL _featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT _numFeatureLevels = ARRAYSIZE(_featureLevels);

	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr = D3D11CreateDevice(0, _createDriverType, 0, _createDeviceFlags, _featureLevels, _numFeatureLevels, D3D11_SDK_VERSION, &m_pD3D, &featureLevel, &m_pD3DImmediateContext);
	if (FAILED(hr))
		return;

	IDXGIDevice* dxgiDevice = nullptr;
	hr = m_pD3D->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	if (FAILED(hr))
		return;

	IDXGIAdapter* dxgiAdapter = nullptr;
	hr = m_pD3D->QueryInterface(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));
	if (FAILED(hr))
		return;

	IDXGIFactory* dxgiFactory = nullptr;
	hr = m_pD3D->QueryInterface(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiAdapter));
	if (FAILED(hr))
		return;

	m_m4xMsaaQuality = 0;
	hr = m_pD3D->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_m4xMsaaQuality);
	if (FAILED(hr))
		return;
	
	assert(m_m4xMsaaQuality > 0);

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

	SAFERELEASE(dxgiDevice);
	SAFERELEASE(dxgiAdapter);
	SAFERELEASE(dxgiFactory);


	InitRenderTargetView();
	InitDepthStencilBufferView();
	InitViewPort();


#elif defined(TK_D3D9)
	if (m_pD3D != NULL) return;
	if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION))) return;
	ZeroMemory(&m_d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	CheckD3D();

	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	m_d3dpp.BackBufferCount = 1;

#ifdef WINDOW_MODE
	m_d3dpp.Windowed = TRUE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	//d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
#else
	m_d3dpp.Windowed = FALSE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;
#endif

	if (m_pD3DDevice != NULL) return;

	//Direct3D9: :BackBufferCount not specified, considered default 1 
	//Direct3D9: :DoneExclusiveMode
	//Direct3D9: (INFO) :Failed to create driver indexbuffer

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		ENGINE_DEVICETYPE,
		GetEngine()->GethWnd(),
		ENGINE_BEHAVIORFLAG,
		&m_d3dpp,
		&m_pD3DDevice
		)))
	{
		return;
	}

	SetInitRenderStates();

	D3DXMatrixPerspectiveFovLH(&matProjection, D3DX_PI / 4,
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		GetEngine()->GetNearDistance(),
		GetEngine()->GetFarDistance()
		);
#endif


}

void TKD3D::Release(void)
{
#if defined(TK_D3D11)
	SAFERELEASE(m_pD3D);
	SAFERELEASE(m_pD3DImmediateContext);
	SAFERELEASE(m_pSwapChain);
	SAFERELEASE(m_pDepthStencilBuffer);
	SAFERELEASE(m_pRenderTargetView);
	SAFERELEASE(m_pDepthStencilView);
#elif defined(TK_D3D9)
    SAFERELEASE(m_pD3DDevice);
    SAFERELEASE(m_pD3D);
#endif    
    __super::Release();
}

void TKD3D::InitRenderTargetView(void)
{
#if defined(TK_D3D11)
	SAFERELEASE(m_pRenderTargetView);

	ID3D11Texture2D * pBackBuffer;
	HRESULT hr;
	
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return;

	hr = m_pD3D->CreateRenderTargetView(pBackBuffer, 0, &m_pRenderTargetView);
	if (FAILED(hr))
		return;

	SAFERELEASE(pBackBuffer);
#endif
}

void TKD3D::InitDepthStencilBufferView(void)
{
#if defined(TK_D3D11)
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
#endif
}

void TKD3D::InitViewPort(void)
{
#if defined(TK_D3D11)
	D3D11_VIEWPORT	vp;

	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = SCREEN_WIDTH;
	vp.Height = SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_pD3DImmediateContext->RSSetViewports(1, &vp);
#endif
}


#ifdef TK_D3D9
void TKD3D::SetInitRenderStates(void)
{
    m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, GetEngine()->GetFillMode() );
    m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    m_pD3DDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
    m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
    m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    m_pD3DDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_ARGB(255, 255, 255, 255));
}

bool TKD3D::SetupMatrices(void)
{
    // 월드매트릭스 변환은 각각의 오브젝트에서 변환한다.
    m_pD3DDevice->SetTransform( D3DTS_WORLD, GetEngine()->GetMatrixIdentity() );

    // 뷰 매트릭스 변환
    m_pD3DDevice->SetTransform( D3DTS_VIEW, &(pCamera->GetFS()->GetViewMatrix()) );

    // 프로젝션 매트릭스 계산
    D3DXMatrixPerspectiveFovLH( &matProjection, D3DX_PI/4 , 
                                (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 
                                GetEngine()->GetNearDistance(), 
                                GetEngine()->GetFarDistance()
                              );
    // 프로젝션 매트릭스 변환
    m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProjection);

    return true;
}
#endif

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

void TKD3D::CheckD3D(void)
{
/*
	m_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps );	
	m_bDepthBiasCap = false;
	if( (m_D3DCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) )
	{			
		if( !m_IsMixedVertexProcess )
		{
			m_dwVSProcess = D3DCREATE_HARDWARE_VERTEXPROCESSING; 
		}
		else
		{
			if( m_D3DCaps.MaxVertexBlendMatrices >= MAX_WEIGHT_D3D && 
				m_D3DCaps.MaxVertexBlendMatrixIndex >= MAX_INDEXMATRIX_D3D )
			{			
				m_dwVSProcess = D3DCREATE_HARDWARE_VERTEXPROCESSING; 
			}
			else
			{
				m_CheckState.m_IsMixedVertexProcess = true;			
				m_dwVSProcess = D3DCREATE_MIXED_VERTEXPROCESSING; 
			}
		}

		if( m_D3DCaps.VertexShaderVersion >= D3DVS_VERSION(1,1) )// D3DCREATE_SOFTWARE_VERTEXPROCESSING|
		{				
			m_CheckState.m_IsVertexShader = true;			
		}			
		if( m_D3DCaps.PixelShaderVersion >= D3DVS_VERSION(1,1) ) 
		{				
			m_CheckState.m_IsPixelShader = true;			
		}		
	}
	else//if( (m_D3DCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 )
	{			
		m_CreationParameters.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	// 새로운 depth bias 기능을 지원하는지 검사
	if ((m_D3DCaps.RasterCaps & D3DPRASTERCAPS_SLOPESCALEDEPTHBIAS) && 
		(m_D3DCaps.RasterCaps & D3DPRASTERCAPS_DEPTHBIAS))
	{
		m_bDepthBiasCap = true;        // DepthBias 기능이 있다면 TRUE
	}

	m_CreationParameters.DeviceType = D3DDEVTYPE_HAL;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if ( !g_UserInput.bDeviceREF && g_pEngine->m_bDeviceREF )
	{		
		g_pEngine->m_bDeviceREF = FALSE;
	}

	if( g_pEngine->m_bDeviceREF )
	{	
		if( m_CreationParameters.DeviceType != D3DDEVTYPE_REF )
		{
			if( m_CheckState.m_IsMixedVertexProcess)
				m_dwVSProcess &= ~D3DCREATE_MIXED_VERTEXPROCESSING;
			else
				m_dwVSProcess &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
			m_dwVSProcess &= ~D3DCREATE_PUREDEVICE;                            
			m_dwVSProcess |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			m_CheckState.m_IsMixedVertexProcess = false;
		}
		m_CreationParameters.DeviceType = D3DDEVTYPE_REF;
	}
*/

}