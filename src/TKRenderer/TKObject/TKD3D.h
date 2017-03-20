#pragma once

#include "TKBaseObject.h"

class TKCamera;

class TKD3D : public TKBaseObject
{
protected:
	ID3D11Device*			m_pD3D;
	ID3D11Device1*          m_pD3D1;
	ID3D11DeviceContext*	m_pD3DImmediateContext;
	ID3D11DeviceContext1*	m_pD3DImmediateContext1;
	IDXGISwapChain*			m_pSwapChain;
	IDXGISwapChain1*        m_pSwapChain1;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D*		m_pDepthStencilBuffer;
	ID3D11DepthStencilView* m_pDepthStencilView;

	UINT					m_m4xMsaaQuality;

    TKCamera                *pCamera;

    XMMATRIX				matProjection;

    DWORD   dsVSProcessing;
    bool    bDepthBiasCap;
    bool    bIsVertexShader;
    bool    bIsPixelShader;

public:
    TKD3D(void);
    virtual ~TKD3D(void);

    void Init(void);
	void Frame(void);
    void Release(void);

    void SetCamera(TKCamera *pCamera);
    TKCamera *GetCamera(void);

	ID3D11Device * GetD3DD(void) { return m_pD3D; }
	ID3D11DeviceContext * GetD3DContext(void) { return m_pD3DImmediateContext; }

	void InitRenderTargetView(void);
	void InitDepthStencilBufferView(void);
	void InitViewPort(void);

	void RT_Clear();

    XMMATRIX * GetMatProjection(void);
    
};
