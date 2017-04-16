#pragma once

#include "TKEngineResource.h"

/////////////////////////////////////////////////////////////////////
// TKShaderResource
/////////////////////////////////////////////////////////////////////
class TKShaderResource : public TKEngineResource
{
private:
    string          ShaderFileName;    // 쉐이더 파일명
    string          TechniqueName;     // 테크닉 이름
protected:
    int             PassIndex;          // 테크닉 패스 인덱스

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
	D3DXHANDLE  hmatAniList;        // 애니메이션 매트릭스 배열 핸들
    D3DXHANDLE  hmatWVP;            // 월드뷰프로젝션 매트릭스 핸들
    D3DXHANDLE  hmatWorld;
    D3DXHANDLE  hmatView;
    D3DXHANDLE  hmatProj;
    D3DXHANDLE  hmatWIT;
    D3DXHANDLE  hmatWLP;
    D3DXHANDLE  hmatWLPB;
    D3DXHANDLE  hvecEyePos;         // 시점좌표

    D3DXHANDLE  hDiffuseMap;        // 디퓨즈텍스쳐 핸들
    D3DXHANDLE  hShadowMap;         // 섀도우맵 핸들
    D3DXHANDLE  hShadowAlphaMap;    // 섀도우알파맵 핸들
    D3DXHANDLE  hSpecularMap;       // 스펙큘러텍스쳐 핸들
    D3DXHANDLE  hOpacityMap;        // 오퍼시티텍스쳐 핸들
    D3DXHANDLE  hBumpMap;           // 범프텍스쳐 핸들

    D3DXHANDLE  hvLightDir;         // 확산광 방향
    D3DXHANDLE  hvDiffuseColor;     // 확산광 칼라
    D3DXHANDLE  hvAmbientColor;     // 환경광 칼라
    D3DXHANDLE  hvSpecularColor;    // 집중광 칼라

    D3DXHANDLE  hBlendColor;        // 블렌딩용 칼라 핸들

    D3DXHANDLE  hLightCount;        // 동적라이트갯수 핸들
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
