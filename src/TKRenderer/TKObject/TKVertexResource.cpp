#include "TKVertexResource.h"
#include "TKEngine.h"

/////////////////////////////////////////////////////////////////////
// TKVertexResource
/////////////////////////////////////////////////////////////////////
TKVertexResource::TKVertexResource(int ID)
	:TKEngineResource(ID)
#ifdef TK_D3D9
	,pVB(NULL)
	,pVDEC(NULL)
#elif defined(TK_D3D11)
	,m_pLayout(nullptr)
	,m_pVB(nullptr)
#endif
{
}

TKVertexResource::~TKVertexResource(void)
{
}

void TKVertexResource::Init(void)
{
    __super::Init();
}

void TKVertexResource::Release(void)
{
#ifdef TK_D3D9
    SAFERELEASE(pVB);
    SAFERELEASE(pVDEC);
#elif defined(TK_D3D11)
	SAFERELEASE(m_pLayout);
	SAFERELEASE(m_pVB);
#endif
    __super::Release();
}

/////////////////////////////////////////////////////////////////////
// TKIndexResource
/////////////////////////////////////////////////////////////////////
TKIndexResource::TKIndexResource(int ID)
	:TKEngineResource(ID)
#ifdef TK_D3D9
	,pIB(NULL)
#elif defined(TK_D3D11)
	,m_pIB(nullptr)
#endif
{
}

TKIndexResource::~TKIndexResource(void)
{
#ifdef TK_D3D9
    SAFERELEASE(pIB);
#elif defined(TK_D3D11)
	SAFERELEASE(m_pIB);
#endif
}

void TKIndexResource::Init(void)
{
    __super::Init();
}

void TKIndexResource::Release(void)
{
#ifdef TK_D3D9
    SAFERELEASE(pIB);
#elif defined(TK_D3D11)
	SAFERELEASE(m_pIB);
#endif
    __super::Release();
}

/////////////////////////////////////////////////////////////////////
// TKRenderResource
/////////////////////////////////////////////////////////////////////
TKRenderResource::TKRenderResource(int ID)
	:TKEngineResource(ID)
#ifdef TK_D3D9
	,pVB(NULL)
	,pVDEC(NULL)
	,pIB(NULL)
#elif defined(TK_D3D11)
	, m_pLayout(nullptr)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
#endif
{
}

TKRenderResource::~TKRenderResource(void)
{
#ifdef TK_D3D9
    SAFERELEASE(pVB);
    SAFERELEASE(pVDEC);
    SAFERELEASE(pIB);
#elif defined(TK_D3D11)
	SAFERELEASE(m_pVB);
	SAFERELEASE(m_pLayout);
	SAFERELEASE(m_pIB);
#endif
}

void TKRenderResource::Init(void)
{
    __super::Init();
}

void TKRenderResource::Release(void)
{
#ifdef TK_D3D9
    SAFERELEASE(pVB);
    SAFERELEASE(pVDEC);
    SAFERELEASE(pIB);
#elif defined(TK_D3D11)
	SAFERELEASE(m_pVB);
	SAFERELEASE(m_pLayout);
	SAFERELEASE(m_pIB);
#endif
    __super::Release();
}