#pragma once

#include "./TKD3D.h"

template <class SHADEREFFECT> 
SHADEREFFECT * TKEngine::ShaderLoad( string FXFileName, string TechniqueName)
{
    SHADEREFFECT *pShaderEffect;

    pShaderEffect = this->GetEngineResourceList()->Item<SHADEREFFECT>(FXFileName);
    if (!pShaderEffect)
    {
        pShaderEffect = this->GetEngineResourceList()->New<SHADEREFFECT>(FXFileName);
        pShaderEffect->SetShaderFileName(FXFileName);
        pShaderEffect->SetTechniqueName(TechniqueName);
        pShaderEffect->Init();
    }
    return pShaderEffect;
}
