#pragma once

#include "TKEngineResource.h"

/////////////////////////////////////////////////////////////////////
// TKVertexResource
/////////////////////////////////////////////////////////////////////
class TKVertexResource : public TKEngineResource
{
protected:
#ifdef TK_D3D9
	LPDIRECT3DVERTEXBUFFER9         pVB;
    LPDIRECT3DVERTEXDECLARATION9    pVDEC;
#elif defined(TK_D3D11)
	ID3D11InputLayout *				m_pLayout;
	ID3D11Buffer *					m_pVB;
#endif

public:
    TKVertexResource(TKEngine *pEngine, int ID);
    virtual ~TKVertexResource(void);

    virtual void Init(void);
    virtual void Release(void);

#ifdef TK_D3D9
    LPDIRECT3DVERTEXBUFFER9 & GetVB(void)       { return pVB; }
    LPDIRECT3DVERTEXDECLARATION9 & GetVDEC(void){ return pVDEC; }
#elif defined(TK_D3D11)
	ID3D11Buffer * GetVB(void)					{ return m_pVB;	}
	ID3D11InputLayout *	GetLayout(void)			{ return m_pLayout;  }
#endif
};

/////////////////////////////////////////////////////////////////////
// TKIndexResource
/////////////////////////////////////////////////////////////////////
class TKIndexResource : public TKEngineResource
{
protected:
#ifdef TK_D3D9
	LPDIRECT3DINDEXBUFFER9          pIB;
#elif defined(TK_D3D11)
	ID3D11Buffer*					m_pIB;
#endif

public:
    TKIndexResource(TKEngine *pEngine, int ID);
    virtual ~TKIndexResource(void);

    virtual void Init(void);
    virtual void Release(void);

#ifdef TK_D3D9
	LPDIRECT3DINDEXBUFFER9 & GetIB(void)        { return pIB; }
#elif defined(TK_D3D11)
	ID3D11Buffer*			GetIB(void)			{ return m_pIB;  }
#endif
};

/////////////////////////////////////////////////////////////////////
// TKRenderResource
/////////////////////////////////////////////////////////////////////
class TKRenderResource : public TKEngineResource
{
protected:
#ifdef TK_D3D9
	LPDIRECT3DVERTEXBUFFER9         pVB;
    LPDIRECT3DVERTEXDECLARATION9    pVDEC;
    LPDIRECT3DINDEXBUFFER9          pIB;
#elif defined(TK_D3D11)
	ID3D11InputLayout *				m_pLayout;
	ID3D11Buffer *					m_pVB;
	ID3D11Buffer *					m_pIB;
#endif

public:
    TKRenderResource(TKEngine *pEngine, int ID);
    virtual ~TKRenderResource(void);

    virtual void Init(void);
    virtual void Release(void);

#ifdef TK_D3D9
	LPDIRECT3DVERTEXBUFFER9 & GetVB(void)       { return pVB; }
    LPDIRECT3DVERTEXDECLARATION9 & GetVDEC(void){ return pVDEC; }
    LPDIRECT3DINDEXBUFFER9 & GetIB(void)        { return pIB; }
#elif defined(TK_D3D11)
	ID3D11Buffer *			GetVB(void) { return m_pVB; }
	ID3D11InputLayout *		GetLayout(void) { return m_pLayout; }
	ID3D11Buffer*			GetIB(void) { return m_pIB; }
#endif
};