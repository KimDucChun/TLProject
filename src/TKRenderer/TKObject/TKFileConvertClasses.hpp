#include "TKFileConvertClasses.h"

#include "TKTexture.h"
#include "TKEngine.h"
#include "TKBaseObjectList.h"

using namespace FILESTRUCT;

template <typename OWNER>
void TKCustomFileConverter::ExtractMaterialFromFile(OWNER *pDestOwner, FILESTRUCT::ST_MATERIALS *pSrcList)
{
    // 메터리얼 정보
    TKMeshMaterial *pMaterial;
    ST_MATERIALS::iterator itMat;
    
    for ( itMat = pSrcList->begin() ; itMat != pSrcList->end() ; ++itMat )
    {
        pMaterial = pDestOwner->NewMaterial( string((*itMat)->NodeName) );
        // 텍스쳐 로딩
        for ( int i = 0 ; i < MATERIAL_TEXTURE_COUNT ; ++i ) 
        {
            if ( (*itMat)->TextureName[i][0] != 0 )
            {
                pMaterial->SetTexture(i, R_TEXTUREMANAGER->LoadTexture( string( (*itMat)->TextureName[i] ) ) );
            }        
        }
        // 메터리얼 정보
        pMaterial->SetAlphaBlend     ((*itMat)->AlphaBlend     );
        pMaterial->SetAlphaBlend_Src ((*itMat)->AlphaBlend_Src );
        pMaterial->SetAlphaBlend_Dest((*itMat)->AlphaBlend_Dest);
        pMaterial->SetAlphaBlend_OP  ((*itMat)->AlphaBlend_OP  );
        pMaterial->SetAlphaTest      ((*itMat)->AlphaTest      );
        pMaterial->SetAlphaTestFunc  ((*itMat)->AlphaTestFunc  );
        pMaterial->SetAlphaTestRef   ((*itMat)->AlphaTestRef   );
        pMaterial->SetTwoSided       ((*itMat)->TwoSided       );
        pMaterial->SetZWriteEnable   ((*itMat)->ZWriteEnable   );
        // 서브메터리얼
        ExtractMaterialFromFile<TKMeshMaterial>( pMaterial, &(*itMat)->SubMaterialList );
    }
}

template <typename OWNER>
void TKCustomFileConverter::ExtractMaterialToFile(FILESTRUCT::ST_MATERIALS *pDestList, OWNER *pSrcOwner)
{
    MATERIALLIST::iterator itMat;
    ST_MATERIAL *pSt_Material;
    TKMeshMaterial *pMaterial;
    MATERIALLIST * pSrcList = &pSrcOwner->GetMaterialList();
    
    for ( itMat = pSrcList->begin() ; itMat != pSrcList->end() ; ++itMat )
    {
        pMaterial = itMat->second;
        pSt_Material = new ST_MATERIAL;
        pSt_Material->Clear();
        strcpy( pSt_Material->NodeName, pMaterial->GetNodeName().data() );
        for ( int i = 0 ; i < MATERIAL_TEXTURE_COUNT ; ++i )
        {
            if ( pMaterial->GetTexture(i) )
            {
                strcpy(pSt_Material->TextureName[i], pMaterial->GetTexture(i)->GetTextureFileName().data() );
            }
        }
        pSt_Material->AlphaBlend      = pMaterial->GetAlphaBlend();
        pSt_Material->AlphaBlend_Src  = pMaterial->GetAlphaBlend_Src();
        pSt_Material->AlphaBlend_Dest = pMaterial->GetAlphaBlend_Dest();
        pSt_Material->AlphaBlend_OP   = pMaterial->GetAlphaBlend_OP();
        pSt_Material->AlphaTest       = pMaterial->GetAlphaTest();
        pSt_Material->AlphaTestFunc   = pMaterial->GetAlphaTestFunc();
        pSt_Material->AlphaTestRef    = pMaterial->GetAlphaTestRef();
        pSt_Material->TwoSided        = pMaterial->GetTwoSided();
        pSt_Material->ZWriteEnable    = pMaterial->GetZWriteEnable();
        
        ExtractMaterialToFile<TKMeshMaterial>( &pSt_Material->SubMaterialList, pMaterial );
        
        pDestList->push_back( pSt_Material );        
    }
}