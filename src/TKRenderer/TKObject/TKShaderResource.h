#pragma once

#include "TKEngineResource.h"

/////////////////////////////////////////////////////////////////////
// TKShaderResource
/////////////////////////////////////////////////////////////////////
class TKShaderResource : public TKEngineResource
{
private:
    string          ShaderFileName;    // ���̴� ���ϸ�
    string          TechniqueName;     // ��ũ�� �̸�
protected:
    int             PassIndex;          // ��ũ�� �н� �ε���

#ifdef TK_D3D9
	LPD3DXEFFECT    pEffect;
    LPD3DXBUFFER    pErr;
    D3DXHANDLE      hTechnique;
#endif

public:
    TKShaderResource(int ID);
    virtual ~TKShaderResource(void);

    virtual void Init(void);
    virtual void Release(void);

#ifdef TK_D3D9
    LPD3DXEFFECT    GetEffect(void)                 { return pEffect; }
    D3DXHANDLE      GetTechnique(void)              { return hTechnique; }
#endif

    string & GetShaderFileName(void)                { return ShaderFileName; }
    void SetShaderFileName(string & ShaderFileName) { this->ShaderFileName = ShaderFileName; }

    string & GetTechniqueName(void)                 { return TechniqueName; }
    void SetTechniqueName(string & TechniqueName)   { this->TechniqueName = TechniqueName; }

    int GetPassIndex(void)                          { return PassIndex; }
    void SetPassIndex(int PassIndex)                { this->PassIndex = PassIndex; }
};

////////////////////////////////////////////////////////////////////////////////
// TKMeshShaderEffect
////////////////////////////////////////////////////////////////////////////////
class TKMeshShaderEffect : public TKShaderResource
{
    bool InitEffect(void);
public:
#ifdef TK_D3D9
	D3DXHANDLE  hmatAniList;        // �ִϸ��̼� ��Ʈ���� �迭 �ڵ�
    D3DXHANDLE  hmatWVP;            // ������������� ��Ʈ���� �ڵ�
    D3DXHANDLE  hmatWorld;
    D3DXHANDLE  hmatView;
    D3DXHANDLE  hmatProj;
    D3DXHANDLE  hmatWIT;
    D3DXHANDLE  hmatWLP;
    D3DXHANDLE  hmatWLPB;
    D3DXHANDLE  hvecEyePos;         // ������ǥ

    D3DXHANDLE  hDiffuseMap;        // ��ǻ���ؽ��� �ڵ�
    D3DXHANDLE  hShadowMap;         // ������� �ڵ�
    D3DXHANDLE  hShadowAlphaMap;    // ��������ĸ� �ڵ�
    D3DXHANDLE  hSpecularMap;       // ����ŧ���ؽ��� �ڵ�
    D3DXHANDLE  hOpacityMap;        // ���۽�Ƽ�ؽ��� �ڵ�
    D3DXHANDLE  hBumpMap;           // �����ؽ��� �ڵ�

    D3DXHANDLE  hvLightDir;         // Ȯ�걤 ����
    D3DXHANDLE  hvDiffuseColor;     // Ȯ�걤 Į��
    D3DXHANDLE  hvAmbientColor;     // ȯ�汤 Į��
    D3DXHANDLE  hvSpecularColor;    // ���߱� Į��

    D3DXHANDLE  hBlendColor;        // ������ Į�� �ڵ�

    D3DXHANDLE  hLightCount;        // ��������Ʈ���� �ڵ�
    D3DXHANDLE  harrLightPos_Type;
    D3DXHANDLE  harrLightDir_Range;
    D3DXHANDLE  harrLightDiffuse_InCone;
    D3DXHANDLE  harrLightSpecular_OutCone;
    D3DXHANDLE  harrAtten;

    D3DXHANDLE  hFogEnable;
    D3DXHANDLE  hFogColor;
    D3DXHANDLE  hvecFog;
    D3DXHANDLE  hvecEye;
#endif

public:
    TKMeshShaderEffect(int ID);
    virtual ~TKMeshShaderEffect(void);

    void Init(void);
    void Release(void);
};

////////////////////////////////////////////////////////////////////////////////
// TKDepthShadowShaderEffect
////////////////////////////////////////////////////////////////////////////////
class TKDepthShadowShaderEffect : public TKShaderResource
{
    bool InitEffect(void);
public:
#ifdef TK_D3D9
    D3DXHANDLE  hmatAniList;
    D3DXHANDLE  hmatWLP;
    D3DXHANDLE  hmatWLPB;
#endif

public:
    TKDepthShadowShaderEffect(int ID);
    virtual ~TKDepthShadowShaderEffect(void);

    void Init(void);
    void Release(void);
};
