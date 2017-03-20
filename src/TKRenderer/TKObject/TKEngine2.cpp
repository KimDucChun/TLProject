#include "TKEngine2.h"

static TKEngine2 * g_pEngine = NULL;
TKEngine2 * GetEngine()
{
	return g_pEngine;
}

TKEngine2::TKEngine2(HINSTANCE hInstance, HWND hWnd)
	:m_hInstance(hInstance)
	,m_hWnd(hWnd)
{
	g_pEngine = this;

	m_pD3D = new TKD3D();
}

TKEngine2::~TKEngine2(void)
{
	SAFEDELETE(m_pD3D);

	g_pEngine = nullptr;
}

void TKEngine2::Init(void)
{
	m_pD3D->Init();
}

void TKEngine2::Release(void)
{
	m_pD3D->Release();
}

TKD3D * TKEngine2::GetD3D(void)
{
	return m_pD3D;
}

ID3D11DeviceContext * TKEngine2::GetD3DContext(void)
{
	return m_pD3D->GetD3DContext();
}

HWND TKEngine2::GethWnd(void)
{
	return m_hWnd;
}

HINSTANCE TKEngine2::GethInstance(void)
{
	return m_hInstance;
}

void TKEngine2::Frame(void)
{
	if (m_pD3D->GetD3DContext() != nullptr)
	{
		m_pD3D->Frame();

		//m_pRenderFuncList->Render();
	}
}

XMMATRIX * TKEngine2::GetViewMatrix(void)
{
	//return &(m_pD3D->GetCamera()->GetFS()->GetViewMatrix());
	return NULL;
}

XMMATRIX * TKEngine2::GetProjectionMatrix(void)
{
	//return m_pD3D->GetMatProjection();
	return NULL;
}

void TKEngine2::SetCamera(TKCamera *pCamera)
{
	//m_pD3D->SetCamera(pCamera);
}

TKCamera * TKEngine2::GetCamera(void)
{
	//return m_pD3D->GetCamera();
	return NULL;
}