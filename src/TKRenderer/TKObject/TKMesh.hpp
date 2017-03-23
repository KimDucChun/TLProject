#pragma once

#include "TKDebug.h"
#include "TKCamera.h"
#include "TKLight.h"
#include "TKSGObject.h"
#include "TKTime.h"
#include "TKEngine.h"
#include "TKInput.h"
#include "TKSceneGraph.h"
#include "TKGeneralRender.h"
#include "TKDepthBufferShadow.h"
#include "TKTexture.h"

////////////////////////////////////////////////////////////////////////////////
// TKMeshRSObject
////////////////////////////////////////////////////////////////////////////////
template_R_FrameSide
TKMeshRS_Class::TKMeshRSObject(TKEngine *pEngine, TKEngineObject *pParent)
:BASECLASS(pEngine, pParent)
{
    IsEnable = false;

    D3DXMatrixIdentity(&GetData()->matWorld);

    GetData()->BlendColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
#if 0
    GetData()->CurrFrame = 1.0f;
    GetData()->CurrFirstFrame = 0;
    GetData()->CurrLastFrame = 0;
#endif    
}

template_R_FrameSide
TKMeshRS_Class::~TKMeshRSObject(void)
{
}

//void TKMESHRS_CLASS::SetStreamSourceOnRS(int & StreamSourceIndex,
//                                        LPDIRECT3DVERTEXBUFFER9 pVertexBuffer,
//                                        int Stride)
//{
//    if (pVertexBuffer)
//    {
//        R_D3DDEVICE->SetStreamSource( StreamSourceIndex, pVertexBuffer, 0, Stride );
//        ++StreamSourceIndex;
//    }
//}

template_R_FrameSide
void TKMeshRS_Class::SetRenderStateOnRS(TKMeshMaterial *pMaterial)
{
    R_D3DDEVICE->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    R_D3DDEVICE->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    R_D3DDEVICE->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );

    if (pMaterial)
    {
        R_D3DDEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);

        R_D3DDEVICE->SetRenderState( D3DRS_ALPHABLENDENABLE, pMaterial->GetAlphaBlend() );
        if ( pMaterial->GetAlphaBlend() && this->GetAlphaBlend() )
        {
            R_D3DDEVICE->SetRenderState( D3DRS_SRCBLEND, pMaterial->GetAlphaBlend_Src() );
            R_D3DDEVICE->SetRenderState( D3DRS_DESTBLEND, pMaterial->GetAlphaBlend_Dest() );
            R_D3DDEVICE->SetRenderState( D3DRS_BLENDOP, pMaterial->GetAlphaBlend_OP() );
        }

        R_D3DDEVICE->SetRenderState( D3DRS_ALPHATESTENABLE, pMaterial->GetAlphaTest() );
        if ( pMaterial->GetAlphaTest() )
        {
            R_D3DDEVICE->SetRenderState( D3DRS_ALPHAFUNC, pMaterial->GetAlphaTestFunc() );
            R_D3DDEVICE->SetRenderState( D3DRS_ALPHAREF, pMaterial->GetAlphaTestRef() );
        }
        if (pMaterial->GetTwoSided())
            R_D3DDEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        else
            R_D3DDEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

        R_D3DDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, pMaterial->GetZWriteEnable());
    }
}

template_R_FrameSide
void TKMeshRS_Class::SetTextureWithShader(TKMeshShaderEffect *pShaderEffect, TKMeshMaterial *pMaterial)
{
    LPDIRECT3DBASETEXTURE9  pBaseTexture;
    //D3DMATERIAL9 mtrl;

    GetRes().GetShaderEffect()->SetPassIndex(0);

    if (pMaterial)
    {
        TKTexture *pTexture;
        pTexture = pMaterial->GetTexture(emtDiffuse);
        if (pTexture)
        {
            R_D3DDEVICE->SetTexture( 0, pTexture->GetTexture() );
            R_D3DDEVICE->GetTexture( 0, &pBaseTexture);
            if ( pShaderEffect ) pShaderEffect->GetEffect()->SetTexture( pShaderEffect->hDiffuseMap, pBaseTexture );
            pBaseTexture->Release();
        }
        else
        {
            R_D3DDEVICE->SetTexture( 0, NULL );
            if ( pShaderEffect ) pShaderEffect->GetEffect()->SetTexture( pShaderEffect->hDiffuseMap, NULL );
        }
        //pTexture = pMaterial->GetTexture(emtOpacity);
        //if (pTexture)
        //{
        //    R_D3DDEVICE->SetTexture( 0, pTexture->GetTexture() );
        //    R_D3DDEVICE->GetTexture( 0, &pBaseTexture);
        //    if ( pShaderEffect ) pShaderEffect->GetEffect()->SetTexture( pShaderEffect->hOpacityMap, pBaseTexture );
        //    pBaseTexture->Release();
        //}
        //else
        //{
        //    R_D3DDEVICE->SetTexture( 0, NULL );
        //    if ( pShaderEffect ) pShaderEffect->GetEffect()->SetTexture( pShaderEffect->hOpacityMap, NULL );
        //}
    }
    else
    {
        R_D3DDEVICE->SetTexture( 0, NULL );
        if ( pShaderEffect ) pShaderEffect->GetEffect()->SetTexture( pShaderEffect->hDiffuseMap, NULL );
        if ( pShaderEffect ) pShaderEffect->GetEffect()->SetTexture( pShaderEffect->hOpacityMap, NULL );
    }
}

template_R_FrameSide
void TKMeshRS_Class::RenderDepthBufferShadow(GEOMLIST & GeomObjectList)
{
#if 0
    TKMeshGeometry *pGeom = NULL;
    TKDepthShadowShaderEffect *pSubEffect = NULL;

    D3DXVECTOR4 vecEyePos;
    D3DXVECTOR4 vecBlendColor;

    int SSIdx;
    //int SubMaterialCount;
    int i;
    //int j;
    //int iLightCount;

    const int iCount = (int)GeomObjectList.size();

    //D3DXMATRIX  matWLP;
    //matWLP = GetData()->matWorld * R_LIGHTGLOBAL->GetRS()->GetMatLP();

    for ( i = 0 ; i < iCount ; ++i )
    {
        if ( GeomObjectList[i]->GetPosList().size() <= 0) continue;

        pGeom = GeomObjectList[i];
        pSubEffect = GetEngine()->ShaderLoad<TKDepthShadowShaderEffect>( "Shadow_DepthBuffer.fx", "TShader" );
        pSubEffect->GetEffect()->SetMatrixArray( pSubEffect->hmatAniList, GetData()->pmatAniArray, GetData()->AniArrayCount );
        pSubEffect->GetEffect()->SetMatrix( pSubEffect->hmatWLP, &GetData()->matWLP );

        SSIdx = 0;
        SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pVertexBuffer, sizeof(D3DXVECTOR3));
        SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pMatrixWeight, sizeof(D3DXVECTOR4));
        SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pMatrixIndex, sizeof(ST_MESHMATRIXINDEX));
        if ( !pGeom->GetTexPosList().empty() )
            SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pTexelBuffer, sizeof(D3DXVECTOR2));
        if ( !pGeom->GetColorList().empty() )
            SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pColorBuffer, sizeof(D3DCOLOR));
        if ( !pGeom->GetNormalList().empty() )
            SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pNormalBuffer, sizeof(D3DXVECTOR3));

        R_D3DDEVICE->SetVertexDeclaration( pGeom->GetVDEC() );
        R_D3DDEVICE->SetIndices( pGeom->GetIB() );

        pSubEffect->GetEffect()->Begin(NULL, 0);
        pSubEffect->GetEffect()->BeginPass( 0 );
        pSubEffect->GetEffect()->CommitChanges();
        R_D3DDEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
                                         (int)pGeom->GetPosList().size(), 0,
                                         (int)pGeom->GetFaceList().size() );
        pSubEffect->GetEffect()->EndPass();
        pSubEffect->GetEffect()->End();
    }
#endif
}

template_R_FrameSide
void TKMeshRS_Class::RenderBoneMesh(GEOMLIST & GeomObjectList)
{
#if 0
#ifdef DEBUG_MODE
    if ( !GetEngine()->GetBoneVisible() ) return;

    TKMeshGeometry      *pGeom = NULL;
    TKMeshShaderEffect  *pSubEffect = NULL;

	D3DXVECTOR4 vecBlendColor;

    int SSIdx;

    //MATERIALLIST *pSubMaterialList;
    //int SubMaterialCount;
    int i;

    for ( i = 0 ; i < (int)GeomObjectList.size() ; ++i )
    {
        if ( GeomObjectList[i]->GetPosList().size() <= 0) continue;

        pGeom = GeomObjectList[i];

        //// OBB렌더링
        //D3DXMATRIX matOBBWorld;
        //matOBBWorld = GetData()->pmatAniArray[ pGeom->GetAnimationMatrixIndex() ] * GetData()->matWorld;
        //R_D3DDEVICE->SetTransform( D3DTS_WORLD, &matOBBWorld );
        //if ( pGeom->GetSelected() )
        //    R_D3DDEVICE->SetStreamSource(0, pGeom->GetVertexBuffers().pOBBVB_Selected, 0, sizeof(ST_VB_POSDIF));
        //else
        //    R_D3DDEVICE->SetStreamSource(0, pGeom->GetVertexBuffers().pOBBVB, 0, sizeof(ST_VB_POSDIF));
        //R_D3DDEVICE->SetFVF( ST_VB_POSDIF::FVF );
        //R_D3DDEVICE->SetIndices( pGeom->GetVertexBuffers().pOBBIB );
        //R_D3DDEVICE->DrawIndexedPrimitive( D3DPT_LINELIST, 0, 0, 8, 0, 12 );

        // Bone 렌더링
        pSubEffect = pGeom->GetShaderEffect();
        pSubEffect->GetEffect()->SetMatrix( pSubEffect->hmatWVP, &GetData()->matWVP );

        D3DXHANDLE hHandle = pSubEffect->hmatAniList;
        D3DXMATRIX *pMatrix = GetData()->pmatAniArray;
        int ArrayCount = GetData()->AniArrayCount;

        pSubEffect->GetEffect()->SetMatrixArray( hHandle, pMatrix, ArrayCount );
        pSubEffect->GetEffect()->SetMatrixArray( pSubEffect->hmatAniList, GetData()->pmatAniArray, GetData()->AniArrayCount );
        if ( pGeom->GetSelected() )
            vecBlendColor = D3DXVECTOR4( 1.0f, 0.5, 0.5f, 1.0f );
        else
            vecBlendColor = GetData()->BlendColor;
        pSubEffect->GetEffect()->SetVector( pSubEffect->hBlendColor , &vecBlendColor);

        SSIdx = 0;
        SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pVertexBuffer, sizeof(D3DXVECTOR3));
        SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pMatrixWeight, sizeof(D3DXVECTOR4));
        SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pMatrixIndex, sizeof(ST_MESHMATRIXINDEX));
        if ( !pGeom->GetTexPosList().empty() )
            SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pTexelBuffer, sizeof(D3DXVECTOR2));
        if ( !pGeom->GetColorList().empty() )
            SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pColorBuffer, sizeof(D3DCOLOR));
        if ( !pGeom->GetNormalList().empty() )
            SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pNormalBuffer, sizeof(D3DXVECTOR3));

        R_D3DDEVICE->SetVertexDeclaration( pGeom->GetVDEC() );
        R_D3DDEVICE->SetIndices( pGeom->GetIB() );

        SetRenderStateOnRS(NULL);
        SetTextureWithShader(pSubEffect, NULL);

        pSubEffect->GetEffect()->Begin(NULL, 0);
        pSubEffect->GetEffect()->BeginPass( pSubEffect->GetPassIndex() );
        pSubEffect->GetEffect()->CommitChanges();
        R_D3DDEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
                                         (int)pGeom->GetPosList().size(), 0,
                                         (int)pGeom->GetFaceList().size() );
        pSubEffect->GetEffect()->EndPass();
        pSubEffect->GetEffect()->End();
    }
#endif DEBUG_MODE
#endif
}

template_R_FrameSide
void TKMeshRS_Class::RenderGeometryWithShader(GEOMLIST & GeomObjectList)
{
#if 0
#ifdef DEBUG_MODE
    if ( !GetEngine()->GetMeshVisible() ) return;
#endif DEBUG_MODE

    TKMeshGeometry      *pGeom = NULL;
    TKMeshShaderEffect  *pSubEffect = NULL;

    D3DXVECTOR4 vecEyePos;
    D3DXVECTOR4 vecBlendColor;

    int SSIdx;

    vector<TKLight *> *pLightArray;
    //int SubMaterialCount;
    int i;
    int j;

    TKSGRenderSideNodeContainer *pNodeContainer;

    D3DXVECTOR4 arrLightPos_Type[5];
    D3DXVECTOR4 arrLightDir_Range[5];
    D3DXVECTOR4 arrLightDiffuse_InCone[5];
    D3DXVECTOR4 arrLightSpecular_OutCone[5];
    D3DXVECTOR4 arrAtten[5];

    const int iCount = (int)GeomObjectList.size();

    if (GetData()->pNode)
        pNodeContainer = GetData()->pNode->GetRSContainer();
    else
        pNodeContainer = NULL;

    vecEyePos.x = GetEngine()->GetCamera()->GetRS()->GetEyePt().x;
    vecEyePos.y = GetEngine()->GetCamera()->GetRS()->GetEyePt().y;
    vecEyePos.z = GetEngine()->GetCamera()->GetRS()->GetEyePt().z;
    vecEyePos.w = 1;

    for ( i = 0 ; i < iCount ; ++i )
    {
        if ( GeomObjectList[i]->GetPosList().size() <= 0) continue;

        pGeom = GeomObjectList[i];
        pSubEffect = pGeom->GetShaderEffect();

        switch ( GetEngine()->GetShadowType() )
        {
            // 깊이버퍼 그림자일때 작업
            case eestDepthBufferProjected:
                pSubEffect->GetEffect()->SetMatrix( pSubEffect->hmatWLP, &GetData()->matWLP );     // 라이트맵 변환행렬
                pSubEffect->GetEffect()->SetMatrix( pSubEffect->hmatWLPB, &GetData()->matWLPB );   // 텍스쳐공간 투영행렬
                // 섀도우맵과 섀도우알파맵
                pSubEffect->GetEffect()->SetTexture( pSubEffect->hShadowMap, R_DEPTHBUFFERSHADOW->GetShadowTexture() );
            break;
        }
        pSubEffect->GetEffect()->SetTexture( pSubEffect->hShadowAlphaMap, R_DEPTHBUFFERSHADOW->GetAlphaMap()->GetTexture() );

#ifdef DEBUG_MODE
        if ( pGeom->GetSelected() )
            vecBlendColor = D3DXVECTOR4( 1.0f, 0.5, 0.5f, 1.0f );
        else
            vecBlendColor = GetData()->BlendColor;
#else  DEBUG_MODE
        vecBlendColor = GetData()->BlendColor;
#endif DEBUG_MODE

        pSubEffect->GetEffect()->SetMatrix( pSubEffect->hmatWVP, &GetData()->matWVP );
        pSubEffect->GetEffect()->SetMatrix( pSubEffect->hmatWorld, &GetData()->matWorld );
        pSubEffect->GetEffect()->SetMatrix( pSubEffect->hmatView, GetEngine()->GetViewMatrix() );
        pSubEffect->GetEffect()->SetMatrix( pSubEffect->hmatProj, GetEngine()->GetProjectionMatrix() );
        pSubEffect->GetEffect()->SetMatrix( pSubEffect->hmatWIT, &GetData()->matWIT );
        pSubEffect->GetEffect()->SetVector( pSubEffect->hvecEyePos, &vecEyePos );
        pSubEffect->GetEffect()->SetVector( pSubEffect->hBlendColor, &vecBlendColor);
        pSubEffect->GetEffect()->SetMatrixArray( pSubEffect->hmatAniList, GetData()->pmatAniArray, GetData()->AniArrayCount );

        // 전역 라이트 정보 쉐이더에 전달
        pSubEffect->GetEffect()->SetVector( pSubEffect->hvLightDir  , &GetData()->vLightDir );
        pSubEffect->GetEffect()->SetVector( pSubEffect->hvDiffuseColor, &R_LIGHTGLOBAL->GetRS()->GetDiffuse() );
        pSubEffect->GetEffect()->SetVector( pSubEffect->hvAmbientColor, &R_LIGHTGLOBAL->GetRS()->GetAmbient() );
        pSubEffect->GetEffect()->SetVector( pSubEffect->hvSpecularColor, &R_LIGHTGLOBAL->GetRS()->GetSpecular() );

        // 동적 라이트 정보 쉐이더에 전달
        if ( pNodeContainer )
        {
            pLightArray = pNodeContainer->GetLightArray();
            int iLightCount;
            iLightCount = (int)pLightArray->size();
            if ( iLightCount > 5 ) iLightCount = 5;
            pSubEffect->GetEffect()->SetInt( pSubEffect->hLightCount, iLightCount );
            if ( iLightCount > 0 )
            {
                const int iLoopCount = iLightCount;
                for ( j = 0 ; j < iLoopCount ; ++j )
                {
                    arrLightPos_Type[j].x = (*pLightArray)[j]->GetRS()->GetPos().x;
                    arrLightPos_Type[j].y = (*pLightArray)[j]->GetRS()->GetPos().y;
                    arrLightPos_Type[j].z = (*pLightArray)[j]->GetRS()->GetPos().z;
                    arrLightPos_Type[j].w = (float)(*pLightArray)[j]->GetRS()->GetLightType();

                    arrLightDir_Range[j].x = (*pLightArray)[j]->GetRS()->GetDir().x;
                    arrLightDir_Range[j].y = (*pLightArray)[j]->GetRS()->GetDir().y;
                    arrLightDir_Range[j].z = (*pLightArray)[j]->GetRS()->GetDir().z;
                    arrLightDir_Range[j].w = (*pLightArray)[j]->GetRS()->GetRange();

                    arrLightDiffuse_InCone[j].x = (*pLightArray)[j]->GetRS()->GetDiffuse().x;
                    arrLightDiffuse_InCone[j].y = (*pLightArray)[j]->GetRS()->GetDiffuse().y;
                    arrLightDiffuse_InCone[j].z = (*pLightArray)[j]->GetRS()->GetDiffuse().z;
                    arrLightDiffuse_InCone[j].w = (*pLightArray)[j]->GetRS()->GetInnerConeTheta();

                    arrLightSpecular_OutCone[j].x = (*pLightArray)[j]->GetRS()->GetSpecular().x;
                    arrLightSpecular_OutCone[j].y = (*pLightArray)[j]->GetRS()->GetSpecular().y;
                    arrLightSpecular_OutCone[j].z = (*pLightArray)[j]->GetRS()->GetSpecular().z;
                    arrLightSpecular_OutCone[j].w = (*pLightArray)[j]->GetRS()->GetOuterConeTheta();

                    arrAtten[j].x = (*pLightArray)[j]->GetRS()->GetAttenuation0();
                    arrAtten[j].y = (*pLightArray)[j]->GetRS()->GetAttenuation1();
                    arrAtten[j].z = (*pLightArray)[j]->GetRS()->GetAttenuation2();
                    arrAtten[j].w = 0;
                }
                pSubEffect->GetEffect()->SetVectorArray( pSubEffect->harrLightPos_Type          , arrLightPos_Type          , iLightCount );
                pSubEffect->GetEffect()->SetVectorArray( pSubEffect->harrLightDir_Range         , arrLightDir_Range         , iLightCount );
                pSubEffect->GetEffect()->SetVectorArray( pSubEffect->harrLightDiffuse_InCone    , arrLightDiffuse_InCone    , iLightCount );
                pSubEffect->GetEffect()->SetVectorArray( pSubEffect->harrLightSpecular_OutCone  , arrLightSpecular_OutCone  , iLightCount );
                pSubEffect->GetEffect()->SetVectorArray( pSubEffect->harrAtten                  , arrAtten                  , iLightCount );
            }
        }

        SSIdx = 0;
        SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pVertexBuffer, sizeof(D3DXVECTOR3));
        SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pMatrixWeight, sizeof(D3DXVECTOR4));
        SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pMatrixIndex, sizeof(ST_MESHMATRIXINDEX));
        if ( !pGeom->GetTexPosList().empty() )
            SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pTexelBuffer, sizeof(D3DXVECTOR2));
        if ( !pGeom->GetColorList().empty() )
            SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pColorBuffer, sizeof(D3DCOLOR));
        if ( !pGeom->GetNormalList().empty() )
            SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pNormalBuffer, sizeof(D3DXVECTOR3));

        R_D3DDEVICE->SetVertexDeclaration( pGeom->GetVDEC() );
        R_D3DDEVICE->SetIndices( pGeom->GetIB() );

        // 메터리얼이 있을때
        if (pGeom->GetMaterial())
        {
            if ( pGeom->GetSubMaterial() )
            {
                SetRenderStateOnRS(pGeom->GetSubMaterial());
                SetTextureWithShader(pSubEffect, pGeom->GetSubMaterial());
            }
            else
            {
                SetRenderStateOnRS(pGeom->GetMaterial());
                SetTextureWithShader(pSubEffect, pGeom->GetMaterial());
            }
        }
        // 메터리얼이 없을 때
        else
        {
            SetRenderStateOnRS(NULL);
            SetTextureWithShader(pSubEffect, NULL);
        }

        pSubEffect->GetEffect()->Begin(NULL, 0);
        pSubEffect->GetEffect()->BeginPass( pSubEffect->GetPassIndex() );
        pSubEffect->GetEffect()->CommitChanges();
        R_D3DDEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
                                         (int)pGeom->GetPosList().size(), 0,
                                         (int)pGeom->GetFaceList().size() );
        pSubEffect->GetEffect()->EndPass();
        pSubEffect->GetEffect()->End();
    }
#endif
}

template_R_FrameSide
void TKMeshRS_Class::RenderGeometryPipeline(GEOMLIST & GeomObjectList)
{
#ifdef TK_D3D9
#ifdef DEBUG_MODE
    if ( !GetEngine()->GetMeshVisible() ) return;
#endif DEBUG_MODE

    TKMeshGeometry      *pGeom = NULL;

    D3DXVECTOR4 vecLight(1.0f, 1.0f, 1.0f, 1.0f);   // 임시로 라이트 최대.
    D3DXVECTOR4 vecBlendColor;

    int SSIdx;

    //MATERIALLIST *pSubMaterialList;
    //int SubMaterialCount;
    int i;

    const int iCount = (int)GeomObjectList.size();

    R_D3DDEVICE->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF );   // 임시로 엠비언트 최대
    R_D3DDEVICE->SetRenderState( D3DRS_LIGHTING, FALSE );
    
    R_D3DDEVICE->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );             // CULLMODE CCW
    //R_D3DDEVICE->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );       // 임시로 와이어프레임으로 그려보자.
    R_D3DDEVICE->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

    for ( i = 0 ; i < iCount ; ++i )
    {
        if ( GeomObjectList[i]->GetPosList().size() <= 0) continue;

        pGeom = GeomObjectList[i];

        R_D3DDEVICE->SetTransform( D3DTS_WORLD, &GetData()->matWorld );

        R_D3DDEVICE->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
        R_D3DDEVICE->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
        R_D3DDEVICE->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        R_D3DDEVICE->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        R_D3DDEVICE->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

        SSIdx = 0;
        SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pVertexBuffer, sizeof(D3DXVECTOR3));
        SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pMatrixWeight, sizeof(D3DXVECTOR4));
        SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pMatrixIndex, sizeof(ST_MESHMATRIXINDEX));

        if ( !pGeom->GetTexPosList().empty() )
            SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pTexelBuffer, sizeof(D3DXVECTOR2));
        if ( !pGeom->GetColorList().empty() )
            SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pColorBuffer, sizeof(D3DCOLOR));
        if ( !pGeom->GetNormalList().empty() )
            SetStreamSourceOnRS(SSIdx, pGeom->GetVertexBuffers().pNormalBuffer, sizeof(D3DXVECTOR3));

        R_D3DDEVICE->SetVertexDeclaration( pGeom->GetVDEC() );
        R_D3DDEVICE->SetIndices( pGeom->GetIB() );

        // 메터리얼이 있을때
        if (pGeom->GetMaterial())
        {
            if ( pGeom->GetSubMaterial() )
            {
                SetRenderStateOnRS(pGeom->GetSubMaterial());
                SetTextureWithShader(NULL, pGeom->GetSubMaterial());
            }
            else
            {
                SetRenderStateOnRS(pGeom->GetMaterial());
                SetTextureWithShader(NULL, pGeom->GetMaterial());
            }
        }
        // 메터리얼이 없을 때
        else
        {
            SetRenderStateOnRS(NULL);
            SetTextureWithShader(NULL, NULL);
        }

        R_D3DDEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
                                         (int)pGeom->GetPosList().size(), 0,
                                         (int)pGeom->GetFaceList().size() );
        
        //R_D3DDEVICE->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, (int)pGeom->GetPosList().size(), (int)pGeom->GetFaceList().size(), (void*)&pGeom->GetFaceList()[0], D3DFMT_INDEX16, 
        //                                       &pGeom->GetPosList()[0], sizeof(pGeom->GetPosList()[0]) );
        
        
    }
#endif
}

template_R_FrameSide
void TKMeshRS_Class::RenderGeometryTest(GEOMLIST & GeomObjectList)
{
#ifdef TK_D3D9
#ifdef DEBUG_MODE
    if ( !GetEngine()->GetMeshVisible() ) return;
#endif DEBUG_MODE

    TKMeshGeometry      *pGeom = NULL;

    D3DXVECTOR4 vecLight(1.0f, 1.0f, 1.0f, 1.0f);
    D3DXVECTOR4 vecBlendColor;

    const int iCount = (int)GeomObjectList.size();

    R_D3DDEVICE->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF );
    R_D3DDEVICE->SetRenderState( D3DRS_LIGHTING, TRUE );

    R_D3DDEVICE->SetFVF( ST_VB_POS::FVF );
    R_D3DDEVICE->SetStreamSource( 0, NULL, 0, sizeof(ST_VB_POS) );             // 스트림데이터 초기화
    R_D3DDEVICE->SetTexture( 0, NULL );                                           // 텍스쳐 초기화
    R_D3DDEVICE->SetIndices( NULL );                                              // 인덱스 버퍼 초기화

    R_D3DDEVICE->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_DISABLE );     // 텍스쳐 컬러 연산하지 않겠다고 정의
    R_D3DDEVICE->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );     // 텍스쳐 컬러 연산하지 않겠다고 정의

	R_D3DDEVICE->SetRenderState( D3DRS_LIGHTING, TRUE );                       // 라이트를 켠다.

    R_D3DDEVICE->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

    D3DMATERIAL9 mtrl;
    R_D3DDEVICE->SetTransform( D3DTS_WORLD, GetEngine()->GetMatrixIdentity() );

    // 파란색으로 상,하 평면을 그린다.
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );                                  // 질감을 설정한다
    mtrl.Emissive.r = mtrl.Diffuse.r = mtrl.Ambient.r = 0.5f;                                     // 빛이 쪼이는부분의 칼라를 파란색으로 한다.
    mtrl.Emissive.g = mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;                                     // 빛이 쪼이는부분의 칼라를 파란색으로 한다.
    mtrl.Emissive.b = mtrl.Diffuse.b = mtrl.Ambient.b = 0.5f;                                     // 빛이 쪼이는부분의 칼라를 파란색으로 한다.

    R_D3DDEVICE->SetMaterial( &mtrl );                                          // 질감을 현재 디바이스에 적용한다.

    //for ( i = 0 ; i < iCount ; ++i )
    {
        //if ( GeomObjectList[0]->GetPosList().size() <= 0) continue;

        static float fIndex = 0.0f;
        static int iIndex = 0;
        static int iLastIndex = 0;

        fIndex += R_TIMEDELTA;
        iIndex = (int)fIndex % (int)GeomObjectList.size();
        pGeom = GeomObjectList[iIndex];
        
        if ( iLastIndex != iIndex )
        {
            static char strTemp[255];
            sprintf(strTemp, "Index:%d\n", iIndex);
            OutputDebugString(strTemp);
        }
        
        iLastIndex = iIndex;
       
        const ST_IB_TRIANGLE * pIB = &(pGeom->GetFaceList()[0]);
        const D3DXVECTOR3 * pVB = &(pGeom->GetPosList()[0]);

        if ( D3D_OK == R_D3DDEVICE->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0,
                                                            (int)pGeom->GetPosList().size(),
                                                            (int)pGeom->GetFaceList().size(),
                                                            pIB,
                                                            D3DFMT_INDEX16,
                                                            pVB,
                                                            sizeof(ST_VB_POS) ) )
        {
            //OutputDebugString("성공\n");
        }
        else
        {
            //OutputDebugString("실패\n");
        }
    }

	R_D3DDEVICE->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    R_D3DDEVICE->SetRenderState( D3DRS_LIGHTING, FALSE );

    R_D3DDEVICE->SetRenderState( D3DRS_FILLMODE, GetEngine()->GetFillMode() );               // 라이트를 켠다.
#endif
}

template_R_FrameSide
void TKMeshRS_Class::Init(void)
{
    __super::Init();

#if 0
    GetData()->FirstFrame       = GetRes().GetFirstFrame();
    GetData()->LastFrame        = GetRes().GetLastFrame();
    GetData()->FrameSpeed       = GetRes().GetFrameSpeed();
    if ( GetData()->CurrFirstFrame == 0 )
        GetData()->CurrFirstFrame = GetRes().GetFirstFrame();
    if ( GetData()->CurrLastFrame == 0 )
        GetData()->CurrLastFrame = GetRes().GetLastFrame();
    GetData()->CurrFrame = 0;
    GetData()->pmatAniArray = GetRes().GetAnimationMatrix(0).second;
    GetData()->AniArrayCount = GetRes().GetMatArrColCnt();
#endif
}

template_R_FrameSide
void TKMeshRS_Class::Render(const ERenderFuncType RenderType)
{
    __super::Render(RenderType);
#ifdef DEBUG_MODE
    if ( RenderType != erftGENERAL ) return;
    if ( !GetEngine()->GetBoneVisible() ) return;
        RenderBoneMesh( GetRes().GetBoneList() );
#endif DEBUG_MODE
}

////////////////////////////////////////////////////////////////////////////////
// TKMeshFSObject
////////////////////////////////////////////////////////////////////////////////
template_R_RenderSide
TKMeshFS_Class::TKMeshFSObject(TKEngine *pEngine, TKEngineObject *pParent)
	:BASECLASS(pEngine, pParent)
	,pParentSGFS(NULL)
{
    SetObjectType( eotMesh );
    SetIsEnable(false);

    D3DXMatrixIdentity(&GetData()->matWorld);
    D3DXMatrixIdentity(&m_matLocal);

    GetData()->BlendColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
#if 0
    GetData()->Animate = false;
    GetData()->CurrFrame = 1.0f;
    GetData()->CurrFirstFrame = 0;
    GetData()->CurrLastFrame = 0;
#endif
}

template_R_RenderSide
TKMeshFS_Class::~TKMeshFSObject(void)
{
}

template_R_RenderSide
void TKMeshFS_Class::Init(void)
{
    __super::Init();

#if 0
    GetData()->FirstFrame       = GetRes().GetFirstFrame();
    GetData()->LastFrame        = GetRes().GetLastFrame();
    GetData()->FrameSpeed       = GetRes().GetFrameSpeed();
    if ( GetData()->CurrFirstFrame == 0 )
        GetData()->CurrFirstFrame = GetRes().GetFirstFrame();
    if ( GetData()->CurrLastFrame == 0 )
        GetData()->CurrLastFrame = GetRes().GetLastFrame();
    GetData()->CurrFrame = 0;
    GetData()->pmatAniArray = GetRes().GetAnimationMatrix(0).second;
    GetData()->AniArrayCount = GetRes().GetMatArrColCnt();

    // pParentBone가 정의되어 있을 경우, pParentMeshFS 도 반드시 정의되어 있어야 한다.
    if (!pParentMeshFS && pParentBone)
    {
        MessageBox(0, "pParentMeshFS가 정의되어 있지 않음", "TKMESHFS_CLASS::Init", MB_ICONERROR);
        _ASSERT(0);
    }
#endif    
}

template_R_RenderSide
void TKMeshFS_Class::Frame(void)
{
    __super::Frame();
    // 프레임 계산
#if 0
    if ( GetData()->Animate )
    {
        GetData()->CurrFrame += GetData()->FrameSpeed * R_TIMEDELTA;
    }
    if ( !(GetData()->CurrFirstFrame <= GetData()->CurrFrame && GetData()->CurrFrame <= GetData()->CurrLastFrame) )
    {
        GetData()->CurrFrame = (float)GetData()->CurrFirstFrame;
    }
#endif

#if 0
    // 매트릭스 연산
    if (pParentMeshFS && pParentBone)
    {
        D3DXMATRIX matTM;
        matTM =   pParentMeshFS->GetData()->pmatAniArray[ pParentBone->GetAnimationMatrixIndex() ] *
                  pParentMeshFS->GetData()->matWorld;
        GetData()->matWorld = matLocal * matTM;
    }
    else if (pParentMeshFS && !pParentBone)
    {   // 부모메쉬는 있되, 별도의 링크될 본애니메이션이 정의되어있지 않은 경우
        GetData()->matWorld = matLocal * pParentMeshFS->GetData()->matWorld;
    }
    else
#endif
    {   // 부모메쉬도, 링크될 본애니메이션 정보도 없는경우
        GetData()->matWorld = m_matLocal;
    }

#if 0
    // 애니메이션 매트릭스 배열 추출
    pair<int, D3DXMATRIX *> Temp;
    Temp = GetRes().GetAnimationMatrix( (int)GetData()->CurrFrame );
    GetData()->AniArrayCount = Temp.first;
    GetData()->pmatAniArray = Temp.second;
#endif

    // 객체와 카메라와의 거리 계산
	D3DXVECTOR3 vMeshPos(GetData()->matWorld._41, GetData()->matWorld._42, GetData()->matWorld._43);
	vMeshPos = vMeshPos - GetEngine()->GetCamera()->GetFS()->GetEyePt();
	SetCameraDistance( D3DXVec3Length(&vMeshPos) );

	// 신그래프 노드
	if (pParentSGFS)
	{
	    GetData()->pNode = pParentSGFS->GetSGNode();
	}

    // World * View * Projection
    GetData()->matWVP = GetData()->matWorld * (*GetEngine()->GetViewMatrix()) * (*GetEngine()->GetProjectionMatrix());

    // 픽셀 스펙큘러 조명을 위한 월드 역행렬
    D3DXMATRIX matTemp;
    matTemp = GetData()->matWorld;
    matTemp._41 = 0.0f; matTemp._42 = 0.0f; matTemp._43 = 0.0f;

    D3DXMatrixInverse(&GetData()->matWIT, NULL, &matTemp);
    D3DXVec3TransformCoord((D3DXVECTOR3 *)&GetData()->vLightDir, &R_LIGHTGLOBAL->GetFS()->GetDir(), &GetData()->matWIT);
    D3DXVec3Normalize((D3DXVECTOR3 *)&GetData()->vLightDir, (D3DXVECTOR3 *)&GetData()->vLightDir);

    // 깊이버퍼 그림자일때 작업
    switch ( GetEngine()->GetShadowType() )
    {
        case eestDepthBufferProjected:
            GetData()->matWLP = GetData()->matWorld * R_LIGHTGLOBAL->GetFS()->GetMatLP();       // 라이트맵의 변환행렬
            GetData()->matWLPB = GetData()->matWLP * R_DEPTHBUFFERSHADOW->GetMatScaleBias();    // 텍스처공간으로의 투영행렬을 곱해준다.
        break;
    }
}

template_R_RenderSide
void TKMeshFS_Class::Release(void)
{
    //pParentMeshFS = NULL;
    //pParentBone = NULL;
	__super::Release();
}

#if 0
RENDERFS_TEMPLATE
bool TKMESHFS_CLASS::SetMotion(pair<int, int> Index)
{
	this->GetData()->CurrFrame = Index.first * 1.0f;
	this->GetData()->CurrFirstFrame = Index.first;
	this->GetData()->CurrLastFrame = Index.second;
	return true;
}
#endif

////////////////////////////////////////////////////////////////////////////////
// TKMesh
////////////////////////////////////////////////////////////////////////////////
template_R_Object
TKMesh_Class::TKMesh( TKEngine *pEngine, TKEngineObject *pParent, int ID, int Seq )
:TKRenderObject<FRAMESIDEOBJECT, RENDERSIDEOBJECT, RENDERRESOURCE>(pEngine, pParent, ID, Seq )
{
}

template_R_Object
TKMesh_Class::~TKMesh(void)
{
}

template_R_Object
void TKMesh_Class::SetMeshRotationLeft(float ScaleValue)
{    
	D3DXMATRIX matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, -90.0f * D3DX_PI / 180.0f * ScaleValue);
	GetFS()->GetmatLocal() = matRot * GetFS()->GetmatLocal();
}

template_R_Object
void TKMesh_Class::SetMeshRotationRight(float ScaleValue)
{
	D3DXMATRIX matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, 90.0f * D3DX_PI / 180.0f * ScaleValue);
	GetFS()->GetmatLocal() = matRot * GetFS()->GetmatLocal();
}

template_R_Object
void TKMesh_Class::SetMeshPosition(D3DXVECTOR3 PositionValue)
{
    GetFS()->GetmatLocal()._41 = PositionValue.x;
    GetFS()->GetmatLocal()._42 = PositionValue.y;
    GetFS()->GetmatLocal()._43 = PositionValue.z;
}

template_R_Object
void TKMesh_Class::SetMeshScale(float ScaleValue)
{
	if(ScaleValue <= 0.1f)
	{
	    ScaleValue = 0.1f;
	}
    GetFS()->GetmatLocal()._11 = ScaleValue;
    GetFS()->GetmatLocal()._22 = ScaleValue;
    GetFS()->GetmatLocal()._33 = ScaleValue;
}