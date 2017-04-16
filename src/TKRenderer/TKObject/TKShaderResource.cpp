#include "TKShaderResource.h"

#include "TKEngine.h"

/////////////////////////////////////////////////////////////////////
// TKShaderResource
/////////////////////////////////////////////////////////////////////
TKShaderResource::TKShaderResource(int ID)
:TKEngineResource(ID)
	,PassIndex(0)
#ifdef TK_D3D9
	,pEffect(NULL)
	,hTechnique(0)
#endif
{
    ShaderFileName.erase();
    TechniqueName.erase();
}

TKShaderResource::~TKShaderResource(void)
{
}

void TKShaderResource::Init(void)
{
    __super::Init();

#if 0
    
    string ShaderFullPath;
    ShaderFullPath = GetEngine()->GetFXDir() + ShaderFileName;

#ifdef TK_D3D9
    if (FAILED(D3DXCreateEffectFromFile(R_D3DDEVICE,
                                        ShaderFullPath.data(),
                                        NULL,
                                        NULL,
                                        ENGINE_SHADERFLAG,
                                        NULL,
                                        &pEffect,
                                        &pErr)))
    {
        return;
    }

    hTechnique = pEffect->GetTechniqueByName(TechniqueName.data());
#endif
#endif
}

void TKShaderResource::Release(void)
{
#ifdef TK_D3D9
	SAFERELEASE(pEffect);
#endif
    
	__super::Release();
}

////////////////////////////////////////////////////////////////////////////////
// TKMeshShaderEffect
////////////////////////////////////////////////////////////////////////////////
TKMeshShaderEffect::TKMeshShaderEffect(int ID)
    :TKShaderResource(ID)
#ifdef TK_D3D9
    ,hmatAniList(0)
    ,hmatWVP(0)
    ,hDiffuseMap(0)
    ,hSpecularMap(0)
    ,hOpacityMap(0)
    ,hBumpMap(0)
    ,hvLightDir(0)
    ,hvDiffuseColor(0)
    ,hvAmbientColor(0)
    ,hvSpecularColor(0)
    ,hBlendColor(0)
    ,hFogEnable(0)
    ,hFogColor(0)
    ,hvecFog(0)
    ,hvecEye(0)
#endif
{
    SetObjectType( eotMeshShaderEffect );
}

TKMeshShaderEffect::~TKMeshShaderEffect(void)
{
}

bool TKMeshShaderEffect::InitEffect(void)
{
#ifdef TK_D3D9
	hmatAniList     = pEffect->GetParameterByName(NULL, "matAniList");
    hmatWVP         = pEffect->GetParameterByName(NULL, "matWVP");
    hmatWorld       = pEffect->GetParameterByName(NULL, "matWorld");
    hmatView        = pEffect->GetParameterByName(NULL, "matView");
    hmatProj        = pEffect->GetParameterByName(NULL, "matProj");

    hmatWIT         = pEffect->GetParameterByName(NULL, "matWIT");
    hmatWLPB        = pEffect->GetParameterByName(NULL, "matWLPB");
    hmatWLP         = pEffect->GetParameterByName(NULL, "matWLP");

    hvecEyePos      = pEffect->GetParameterByName(NULL, "vecEyePos");

    hDiffuseMap     = pEffect->GetParameterByName(NULL, "DiffuseMap");
    hShadowMap      = pEffect->GetParameterByName(NULL, "ShadowMap");
    hShadowAlphaMap = pEffect->GetParameterByName(NULL, "ShadowAlphaMap");
    hSpecularMap    = pEffect->GetParameterByName(NULL, "SpecularMap");
    hOpacityMap     = pEffect->GetParameterByName(NULL, "OpacityMap");
    hBumpMap        = pEffect->GetParameterByName(NULL, "BumpMap");

    hvLightDir      = pEffect->GetParameterByName(NULL, "vLightDir");
    hvDiffuseColor  = pEffect->GetParameterByName(NULL, "vDiffuseColor");
    hvAmbientColor  = pEffect->GetParameterByName(NULL, "vAmbientColor");
    hvSpecularColor = pEffect->GetParameterByName(NULL, "vSpecularColor");

    hBlendColor     = pEffect->GetParameterByName(NULL, "BlendColor");

    hFogEnable      = pEffect->GetParameterByName(NULL, "bFogEnable");
    hFogColor       = pEffect->GetParameterByName(NULL, "vecFogColor");
    hvecFog         = pEffect->GetParameterByName(NULL, "vecFog");
    hvecEye         = pEffect->GetParameterByName(NULL, "vecEye");

    hLightCount                 = pEffect->GetParameterByName(NULL, "LightCount");
    harrLightPos_Type           = pEffect->GetParameterByName(NULL, "arrLightPos_Type");
    harrLightDir_Range          = pEffect->GetParameterByName(NULL, "arrLightDir_Range");
    harrLightDiffuse_InCone     = pEffect->GetParameterByName(NULL, "arrLightDiffuse_InCone");
    harrLightSpecular_OutCone   = pEffect->GetParameterByName(NULL, "arrLightSpecular_OutCone");
    harrAtten                   = pEffect->GetParameterByName(NULL, "arrAtten");
	
    pEffect->SetTechnique(hTechnique);
#endif

    return true;
}

void TKMeshShaderEffect::Init(void)
{
    __super::Init();

    // 이펙트 객체를 생성한다
    if (InitEffect() == false)
    {
        MessageBox(0, "InitEffect fail", "TKMeshShaderEffect::Init", MB_ICONERROR);
        _ASSERT(0);
    }
}

void TKMeshShaderEffect::Release(void)
{
    __super::Release();
}

////////////////////////////////////////////////////////////////////////////////
// TKMeshShaderEffect
////////////////////////////////////////////////////////////////////////////////
TKDepthShadowShaderEffect::TKDepthShadowShaderEffect(int ID)
    :TKShaderResource(ID)
#ifdef TK_D3D9
	,hmatAniList(0)
    ,hmatWLP(0)
    ,hmatWLPB(0)
#endif
{
    SetObjectType( eotDepthShadowShaderEffect );
}

TKDepthShadowShaderEffect::~TKDepthShadowShaderEffect(void)
{
}

bool TKDepthShadowShaderEffect::InitEffect(void)
{
#ifdef TK_D3D9
	hmatAniList     = pEffect->GetParameterByName(NULL, "matAniList");
    hmatWLP         = pEffect->GetParameterByName(NULL, "matWLP");
    hmatWLPB        = pEffect->GetParameterByName(NULL, "matWLPB");

    pEffect->SetTechnique(hTechnique);
#endif
    return true;
}

void TKDepthShadowShaderEffect::Init(void)
{
    __super::Init();

#ifdef TK_D3D9
    // 이펙트 객체를 생성한다
    if (InitEffect() == false)
    {
        MessageBox(0, "InitEffect fail", "TKMeshShaderEffect::Init", MB_ICONERROR);
        _ASSERT(0);
    }
#endif
}

void TKDepthShadowShaderEffect::Release(void)
{
    __super::Release();
}
