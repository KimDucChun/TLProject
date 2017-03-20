#pragma once

#include "TKBaseObject.h"

class TKD3D;
class TKCamera;

class TKEngine2 : public TKBaseObject
{
private:
#ifdef DEBUG_MODE
    char                DebugStr[100];
#endif DEBUG_MODE

    string              EngineRootDir;

	XMMATRIX			matIdentity;            // ������Ʈ����
    HWND                m_hWnd;                 // ���� ���� �ڵ�
    HINSTANCE           m_hInstance;              // ���� ���� �ν��Ͻ�

    // ���� �⺻ ��ü
    TKD3D               *m_pD3D;                  // D3D ����̽� ��ü
public:
    TKEngine2(HINSTANCE hInstance, HWND hWnd);
    virtual ~TKEngine2(void);

	void		Init(void);
	void		Frame(void);
	void		Release(void);

    TKD3D * GetD3D(void);
	ID3D11DeviceContext * GetD3DContext(void);

    HWND GethWnd(void);
    HINSTANCE GethInstance(void);

	XMMATRIX *		GetViewMatrix(void);
	XMMATRIX *		GetProjectionMatrix(void);
	void			SetCamera(TKCamera *pCamera);
	TKCamera *		GetCamera(void);
};

extern TKEngine2 * GetEngine();

#include "./TKEngine2.hpp"

//#define R_TIMEDELTA                     GetEngine()->GetTime()->GetTimeDelta()
#define R_TIMEDELTA                     0.0f;	// �ӽ�