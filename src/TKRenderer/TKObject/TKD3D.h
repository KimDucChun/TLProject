#pragma once

#include "TKBaseObject.h"

class TKCamera;

class TKD3D : public TKBaseObject
{
protected:
#if defined(TK_D3D9)
	LPDIRECT3D9             m_pD3D;
    LPDIRECT3DDEVICE9       m_pD3DDevice;
    D3DPRESENT_PARAMETERS   m_d3dpp;
#elif defined(TK_D3D11)
	ID3D11Device*			m_pD3D;
	ID3D11DeviceContext*	m_pD3DImmediateContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D*		m_pDepthStencilBuffer;
	ID3D11DepthStencilView* m_pDepthStencilView;

	UINT					m_m4xMsaaQuality;
#endif

    TKCamera                *pCamera;

    XMMATRIX				matProjection;

    void CheckD3D(void);

    DWORD   dsVSProcessing;
    bool    bDepthBiasCap;
    bool    bIsVertexShader;
    bool    bIsPixelShader;

public:
    TKD3D(TKEngine *pEngine);
    virtual ~TKD3D(void);

    void Init(void);
    void Release(void);

    void SetCamera(TKCamera *pCamera);
    TKCamera *GetCamera(void);

#if defined(TK_D3D11)
	ID3D11Device * GetD3DD(void) { return m_pD3D; }
	ID3D11DeviceContext * GetD3DContext(void) { return m_pD3DImmediateContext; }

	void InitRenderTargetView(void);
	void InitDepthStencilBufferView(void);
	void InitViewPort(void);

#elif defined(TK_D3D9)
	LPDIRECT3DDEVICE9 GetD3DDevice(void) { return m_pD3DDevice; }
	D3DPRESENT_PARAMETERS * GetD3Dpp(void) { return &m_d3dpp; }

	void SetInitRenderStates(void);
	bool SetupMatrices(void);
#endif

    XMMATRIX * GetMatProjection(void);
    
};
