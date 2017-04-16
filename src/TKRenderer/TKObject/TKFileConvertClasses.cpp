#include "TKFileConvertClasses.h"

#include "TKTexture.h"
#include "TKEngine.h"
#include "TKBaseObjectList.h"

using namespace FILESTRUCT;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TKCustomFileConverter
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TKCustomFileConverter::TKCustomFileConverter(void)
{
    pEngine = NULL;
    pAniMeshRes = NULL;
    pCollisionRes = NULL;
    CollisionObjectType = ecotNULL;
    OriginCollisionList.clear();
}

TKCustomFileConverter::~TKCustomFileConverter(void)
{
}

bool TKCustomFileConverter::ExtractMaterialListFromMAX( MAX_MATERIALINFOS *pSrcList, TKMeshMaterial *pParentMaterial )
{
#if 0
	TKMeshMaterial  *pMaterial = NULL;
    MATERIALLIST::iterator itMaterial;

    for (int i = 0 ; i < pSrcList->Count() ; ++i )
    {
        //OutputDebugString(pSrcList->Items(i)->MaterialName);
        //OutputDebugString("\n");

        pMaterial = NULL;

        if (pParentMaterial)
        {
            // 서브메터리얼
            itMaterial = pParentMaterial->GetMaterialList().find( string(pSrcList->Items(i)->MaterialName) );
            if ( itMaterial == pParentMaterial->GetMaterialList().end() )
            {
                pMaterial = pParentMaterial->NewMaterial( string(pSrcList->Items(i)->MaterialName) );
            }
        }
        else
        {
            // 루트메터리얼
            itMaterial = pAniMeshRes->GetMaterialList().find( string(pSrcList->Items(i)->MaterialName) );
            if ( itMaterial == pAniMeshRes->GetMaterialList().end() )
            {
                pMaterial = pAniMeshRes->NewMaterial( string(pSrcList->Items(i)->MaterialName) );
            }
        }

        if (pMaterial)
        {
            pMaterial->SetNodeName( string(pSrcList->Items(i)->MaterialName) );
            //pMaterial->SetAmbient(pSrcList->Items(i)->MAmbient);
            //pMaterial->SetDiffuse(pSrcList->Items(i)->MDiffuse);
            //pMaterial->SetSpecular(pSrcList->Items(i)->MSpecular * pSrcList->Items(i)->ShineStrength / 100.0f  );
            //pMaterial->SetPower( pSrcList->Items(i)->Shine / 100.0f);
            pMaterial->SetAlphaBlend( pSrcList->Items(i)->AlphaBlending );
            pMaterial->SetAlphaTestRef( (int)pSrcList->Items(i)->Transparency );
            pMaterial->SetTwoSided( pSrcList->Items(i)->TwoSided );
            //pMaterial->SetEmissive(pSrcList->Items(i)->Selfillum);

            TKTexture *pTexture = NULL;

            // EMaterialTextureIndex 의 인덱스 순서에 맞게 Add한다.
            for ( int j = 0 ; j < MATERIAL_TEXTURE_COUNT ; ++j )
            {
                MAPPING_INFO *pMappingInfo = &(pSrcList->Items(i)->MappingInfo[j]);
                if (pMappingInfo->MapFileName[0] != 0)
                {
                    pTexture = R_TEXTUREMANAGER->LoadTexture( string(pMappingInfo->MapFileName), MASKCOLOR );
                    if ( !pTexture )
                    {
                        // 텍스쳐를 읽는 것이 실패했을경우 해당 메터리얼은 삭제.
                        MessageBox(0, "Load Texture fault", "TKCustomFileConverter::ExtractMaterialList", MB_ICONERROR);
                        _ASSERT(0);
                    }
                    pMaterial->SetTexture(j, pTexture);
                }
            }

            if ( pSrcList->Items(i)->Items.Count() > 0 )
            {
                ExtractMaterialListFromMAX(&(pSrcList->Items(i)->Items), pMaterial);
            }
        }
    }
#endif
    return true;
}

bool TKCustomFileConverter::ExtractBoneListFromMAX(MAX_GEOMOBJECTS *pSrcList)
{
    char strBone[10], strBip[10], strDummy[10];
    for ( int i = 0 ; i < pSrcList->Count() ; ++i )
    {
        // 노드 이름이 Bone, Bip, Dummy, 으로 시작되는 경우, 해당 지오메트리는 본 오브젝트
        strncpy(strBone , pSrcList->Items(i)->NodeName, 4);  strBone[4] = 0;    strupr(strBone);
        strncpy(strBip  , pSrcList->Items(i)->NodeName, 3);  strBip[3] = 0;     strupr(strBip);
        strncpy(strDummy, pSrcList->Items(i)->NodeName, 5);  strDummy[5] = 0;   strupr(strDummy);
        if (strcmp(strBone, "BONE") == 0 || strcmp(strBip, "BIP") == 0 || strcmp(strDummy, "DUMMY") == 0 )
        {
            ExtractGeometryFromMAX(pSrcList->Items(i), emvtBONE );
        }
    }
    return true;
}

bool TKCustomFileConverter::ExtractGeomListFromMAX(MAX_GEOMOBJECTS *pSrcList)
{
    char strBone[10], strBip[10], strDummy[10], strColl[10];
    for ( int i = 0 ; i < pSrcList->Count() ; ++i )
    {
        // 노드 이름에 Bone, Bip, Dummy, COLL_ 이 포함되어 있지 않은 경우, 해당 지오메트리는 메쉬오브젝트
        strncpy(strBone , pSrcList->Items(i)->NodeName, 4);  strBone[4] = 0;    strupr(strBone);
        strncpy(strBip  , pSrcList->Items(i)->NodeName, 3);  strBip[3] = 0;     strupr(strBip) ;
        strncpy(strDummy, pSrcList->Items(i)->NodeName, 5);  strDummy[5] = 0;   strupr(strDummy);
        strncpy(strColl , pSrcList->Items(i)->NodeName, 5);  strColl[5] = 0;    strupr(strColl);
        if (strcmp(strBone  , "BONE" ) != 0 &&
            strcmp(strBip   , "BIP"  ) != 0 &&
            strcmp(strDummy , "DUMMY") != 0 &&
            strcmp(strColl  , "COLL_") != 0)
        {
            ExtractGeometryFromMAX(pSrcList->Items(i), emvtMESH);
        }
    }
    return true;
}

bool TKCustomFileConverter::ExtractGeometryFromMAX(MAX_GEOMOBJECT *pSrcGeom, EMeshVertexesType MeshVertexesType)
{
    TKMeshGeometry *pChildMeshGeometry;
    TKMeshVertexes *pMV;
    TKMeshFaces  *pMI;
    
    pMV = pAniMeshRes->NewVertexes(string(pSrcGeom->NodeName), MeshVertexesType);
    pMI = pAniMeshRes->NewFaces(string(pSrcGeom->NodeName), pMV);
    pChildMeshGeometry = pAniMeshRes->NewGeometry(string(pSrcGeom->NodeName), pMV, pMI );
    
    pChildMeshGeometry->SetMeshVertexesType( MeshVertexesType );
    pChildMeshGeometry->SetNodeName( string(pSrcGeom->NodeName) );
    pChildMeshGeometry->SetBoneNodeName( string(pSrcGeom->ParentNodeName) );
    pChildMeshGeometry->SetMaterialName( string(pSrcGeom->MaterialName) );
    pChildMeshGeometry->SetSubMaterialName( string(pSrcGeom->SubMaterialName) );

    if (MeshVertexesType == emvtMESH)
    {
        ExtractOriginVertexListFromMAX(&pSrcGeom->Mesh.PosWeightList, &pChildMeshGeometry->GetOriginVertexList());
        ExtractTexelListFromMAX       (&pSrcGeom->Mesh.TexPosList   , &pChildMeshGeometry->GetTexPosList());
        ExtractVertexColorListFromMAX (&pSrcGeom->Mesh.ColorList    , &pChildMeshGeometry->GetColorList());
        ExtractNormalListFromMAX      (&pSrcGeom->Mesh.NormalList   , &pChildMeshGeometry->GetNormalList());
        ExtractFaceListFromMAX        (&pSrcGeom->Mesh.FaceList     , &pChildMeshGeometry->GetFaceList());
    }
#ifdef DEBUG_MODE
    else if (MeshVertexesType == emvtBONE)
    {
        ExtractBoneMeshOriginVertexListFromMAX(&pSrcGeom->Mesh.PosWeightList  , 
                                               &pChildMeshGeometry->GetOriginVertexList(), 
                                               pChildMeshGeometry->GetNodeName() );
        ExtractTexelListFromMAX       (&pSrcGeom->Mesh.TexPosList   , &pChildMeshGeometry->GetTexPosList());
        ExtractVertexColorListFromMAX (&pSrcGeom->Mesh.ColorList    , &pChildMeshGeometry->GetColorList());
        ExtractNormalListFromMAX      (&pSrcGeom->Mesh.NormalList   , &pChildMeshGeometry->GetNormalList());
        ExtractFaceListFromMAX        (&pSrcGeom->Mesh.FaceList     , &pChildMeshGeometry->GetFaceList());
    }
#endif DEBUG_MODE

    // 애니메이션 데이터를 변환한다.
    ExtractAniMatrixListFromMAX(&pSrcGeom->Tm_Animation.AniMatrix_List, &pChildMeshGeometry->GetAniMatrixList());

    // OBB 데이터를 변환한다.
    pChildMeshGeometry->SetOBBCenter( pSrcGeom->Mesh.vecCenter );
    pChildMeshGeometry->SetOBBAxis( pSrcGeom->Mesh.vecAxis );
    pChildMeshGeometry->SetOBBAxisLen( pSrcGeom->Mesh.AxisLen );
    pChildMeshGeometry->SetOBB( pSrcGeom->Mesh.vecOBB );
    pChildMeshGeometry->SetMin( pSrcGeom->Mesh.vecMin );
    pChildMeshGeometry->SetMax( pSrcGeom->Mesh.vecMax );
    
    return true;
}

bool TKCustomFileConverter::ExtractBoneMeshOriginVertexListFromMAX(MAX_VERTEXS *pSrcList, vector<ST_GEOM_VERTEX> *pDestList, string & ParentNodeName)
{
#if 0
	pDestList->resize(pSrcList->size());
    for ( int i = 0 ; i < (int)pSrcList->size() ; ++i )
    {
        (*pDestList)[i].Pos.x = (*pSrcList)[i].x;
        (*pDestList)[i].Pos.y = (*pSrcList)[i].y;
        (*pDestList)[i].Pos.z = (*pSrcList)[i].z;

        (*pDestList)[i].Weights[0].RefNodeIndex = 0;
        (*pDestList)[i].Weights[0].pRefNode = NULL;
        (*pDestList)[i].Weights[0].Weight = 1.0f;
        (*pDestList)[i].Weights[0].SetParentNodeName( ParentNodeName );
        for ( int j = 1 ; j < 4 ; ++j )
        {
            (*pDestList)[i].Weights[j].RefNodeIndex = 0;
            (*pDestList)[i].Weights[j].pRefNode = NULL;
            (*pDestList)[i].Weights[j].Weight = 0.0f;
            (*pDestList)[i].Weights[j].SetParentNodeName( string( "" ) );
        }
        /* 버텍스 애니메이션 인덱스는 각 메쉬의 MeshAnimationBind함수를 통해서 정의한다. */
    }
#endif
    return true;
}

bool TKCustomFileConverter::ExtractOriginVertexListFromMAX(MAX_VERTEXS *pSrcList, vector<ST_GEOM_VERTEX> *pDestList)
{
#if 0
	pDestList->resize(pSrcList->size());
    for ( int i = 0 ; i < (int)pSrcList->size() ; ++i )
    {
        (*pDestList)[i].Pos.x = (*pSrcList)[i].x;
        (*pDestList)[i].Pos.y = (*pSrcList)[i].y;
        (*pDestList)[i].Pos.z = (*pSrcList)[i].z;

        if ((*pSrcList)[i].MatrixWeights.size() <= 4 )
        {
            for ( int j = 0 ; j < 4 ; ++j )
            {
                (*pDestList)[i].Weights[j].RefNodeIndex = 0;
                (*pDestList)[i].Weights[j].pRefNode = NULL;
                (*pDestList)[i].Weights[j].Weight = 0.0f;
                (*pDestList)[i].Weights[j].SetParentNodeName( string( "" ) );
            }
            for ( int j = 0 ; j < (int)(*pSrcList)[i].MatrixWeights.size() ; ++j )
            {
                (*pDestList)[i].Weights[j].RefNodeIndex = 0;
                (*pDestList)[i].Weights[j].pRefNode = NULL;
                (*pDestList)[i].Weights[j].Weight = (*pSrcList)[i].MatrixWeights[j].Weight;
                (*pDestList)[i].Weights[j].SetParentNodeName( string( (*pSrcList)[i].MatrixWeights[j].ParentNodeName ) );
            }
        }
        else
        {
            float TotalWeight = 0.0f;
            for ( int j = 0 ; j < 4 ; ++j )
            {
                (*pDestList)[i].Weights[j].RefNodeIndex = 0;
                (*pDestList)[i].Weights[j].pRefNode = NULL;
                (*pDestList)[i].Weights[j].Weight = (*pSrcList)[i].MatrixWeights[j].Weight;
                (*pDestList)[i].Weights[j].SetParentNodeName( string( (*pSrcList)[i].MatrixWeights[j].ParentNodeName ) );

                TotalWeight += (*pDestList)[i].Weights[j].Weight;
            }

            (*pDestList)[i].Weights[0].Weight += 1.0f - TotalWeight;

        }
        /* 버텍스 애니메이션 인덱스는 각 메쉬의 MeshAnimationBind함수를 통해서 정의한다. */
    }
#endif
    return true;
}

bool TKCustomFileConverter::ExtractVertexListFromMAX(MAX_VERTEXS *pSrcList, D3DXVECTOR3S *pDestList)
{
    pDestList->resize(pSrcList->size());
    for ( int i = 0 ; i < (int)pSrcList->size() ; ++i )
    {
        (*pDestList)[i].x = (*pSrcList)[i].x;
        (*pDestList)[i].y = (*pSrcList)[i].y;
        (*pDestList)[i].z = (*pSrcList)[i].z;
    }
    return true;
}

bool TKCustomFileConverter::ExtractTexelListFromMAX(D3DXVECTOR3S *pSrcList , vector<D3DXVECTOR2> *pDestList)
{
    pDestList->resize(pSrcList->size());
    for ( int i = 0 ; i < (int)pSrcList->size() ; ++i )
    {
        (*pDestList)[i].x = (*pSrcList)[i].x;
        (*pDestList)[i].y = (*pSrcList)[i].y;
    }
    return true;
}

bool TKCustomFileConverter::ExtractVertexColorListFromMAX(D3DXVECTOR3S *pSrcList , vector<D3DCOLOR> *pDestList)
{
    pDestList->resize(pSrcList->size());
    for ( int i = 0 ; i < (int)pSrcList->size() ; ++i )
    {
        (*pDestList)[i] = D3DCOLOR_COLORVALUE(  (*pSrcList)[i].x, (*pSrcList)[i].y, (*pSrcList)[i].z, 1.0f);
    }
    return true;
}

bool TKCustomFileConverter::ExtractNormalListFromMAX(D3DXVECTOR3S *pSrcList , D3DXVECTOR3S *pDestList)
{
    (*pDestList) = (*pSrcList);
    return true;
}

bool TKCustomFileConverter::ExtractFaceListFromMAX(MAX_MESH_FACES *pSrcList , vector<ST_IB_TRIANGLE> *pDestList)
{
    pDestList->resize(pSrcList->size());
    for ( int i = 0 ; i < (int)pSrcList->size() ; ++i )
    {
        (*pDestList)[i]._0 = (*pSrcList)[i].a;
        (*pDestList)[i]._1 = (*pSrcList)[i].b;
        (*pDestList)[i]._2 = (*pSrcList)[i].c;
    }
    return true;
}

bool TKCustomFileConverter::ExtractAniMatrixListFromMAX(ANIMATRIX_LIST *pSrcList , vector<ST_ANIMATRIX> *pDestList)
{
    pDestList->resize( pSrcList->size() );
    if (!pSrcList->empty())
        memcpy( &(*pDestList)[0], &(*pSrcList)[0], sizeof(ST_ANIMATRIX) * pSrcList->size());

    return true;
}

bool TKCustomFileConverter::ExtractAniMotionFromMAX(ST_ANIMOTIONINFOS *pSrcList, MOTIONLIST *pDestList)
{
    ST_ANIMOTION DestData;

    int iSize = (int)pSrcList->size();

    pDestList->clear();
    for ( int i = 0 ; i < iSize ; ++i )
    {
        DestData.StartFrame = (*pSrcList)[i].StartFrame;
        DestData.EndFrame   = (*pSrcList)[i].EndFrame;
        DestData.MotionName = (*pSrcList)[i].AnimationName;
        pDestList->insert( make_pair( DestData.MotionName, DestData ) );
    }

    return true;
}


bool TKCustomFileConverter::ExtractCollisionListFromMAX(MAX_GEOMOBJECTS *pSrcList)
{
    char strColl[10];
    for ( int i = 0 ; i < pSrcList->Count() ; ++i )
    {
        // 노드 이름에 COLL_ 포함되어 있는 경우, 해당 지오메트리는 충돌 오브젝트
        strncpy(strColl , pSrcList->Items(i)->NodeName, 5);  strColl[5] = 0;    strupr(strColl);
        if (strcmp(strColl , "COLL_") == 0 )
        {
            ExtractCollisionFromMAX( pSrcList->Items(i) );
        }
    }
    return true;
}

bool TKCustomFileConverter::ExtractCollisionFromMAX(MAX_GEOMOBJECT *pSrcGeom )
{
    ST_ORIGINCOLLISIONDATA *pOriginCollision;

    if ( pSrcGeom->GeomObjectType == egotCollisionFace )
    {
        pOriginCollision = new ST_ORIGINCOLLISIONDATA();
        pOriginCollision->ObjectType = eotFaceCollision;
        pOriginCollision->SetNodeName( string(pSrcGeom->NodeName) );
        pOriginCollision->SetBoneNodeName( string(pSrcGeom->ParentNodeName) );
        ExtractVertexListFromMAX   (&pSrcGeom->Mesh.PosWeightList   , &pOriginCollision->PosList);
        ExtractNormalListFromMAX   (&pSrcGeom->Mesh.NormalList      , &pOriginCollision->NormalList);
        ExtractFaceListFromMAX     (&pSrcGeom->Mesh.FaceList        , &pOriginCollision->FaceList);
        pOriginCollision->vecCenter = pSrcGeom->Mesh.vecCenter;
        memcpy(pOriginCollision->AxisLen, pSrcGeom->Mesh.AxisLen, sizeof(pOriginCollision->AxisLen));
        memcpy(pOriginCollision->vecAxisDir, pSrcGeom->Mesh.vecAxis, sizeof(pOriginCollision->vecAxisDir));
        memcpy(pOriginCollision->vecOBB, pSrcGeom->Mesh.vecOBB, sizeof(pOriginCollision->vecOBB));

        OriginCollisionList.push_back(pOriginCollision);
    }
    else if ( pSrcGeom->GeomObjectType == egotCollisionOBB )
    {
        pOriginCollision = new ST_ORIGINCOLLISIONDATA();
        pOriginCollision->ObjectType = eotOBBCollision;
        pOriginCollision->SetNodeName( string(pSrcGeom->NodeName) );
        pOriginCollision->SetBoneNodeName( string(pSrcGeom->ParentNodeName) );
        ExtractVertexListFromMAX   (&pSrcGeom->Mesh.PosWeightList   , &pOriginCollision->PosList);
        ExtractNormalListFromMAX   (&pSrcGeom->Mesh.NormalList      , &pOriginCollision->NormalList);
        ExtractFaceListFromMAX     (&pSrcGeom->Mesh.FaceList    , &pOriginCollision->FaceList);
        pOriginCollision->vecCenter = pSrcGeom->Mesh.vecCenter;
        memcpy(pOriginCollision->AxisLen, pSrcGeom->Mesh.AxisLen, sizeof(pOriginCollision->AxisLen));
        memcpy(pOriginCollision->vecAxisDir, pSrcGeom->Mesh.vecAxis, sizeof(pOriginCollision->vecAxisDir));
        memcpy(pOriginCollision->vecOBB, pSrcGeom->Mesh.vecOBB, sizeof(pOriginCollision->vecOBB));

        OriginCollisionList.push_back(pOriginCollision);
    }
    return true;
}

void TKCustomFileConverter::ExtractGeometryFromFile(ST_GEOMETRIES *pSrcList )
{
#if 0
	// GeomList 정보
    TKMeshGeometry *pGeom;
    TKMeshVertexes *pVectexes;
    TKMeshFaces  *pFaces;
    ST_GEOMETRIES::iterator itGeom;

    ST_GEOM_VERTEX St_Geom_Vertex;
    for ( itGeom = pSrcList->begin() ; itGeom != pSrcList->end() ; ++itGeom )
    {
        pVectexes = pAniMeshRes->NewVertexes( string((*itGeom)->NodeName), (*itGeom)->MeshVertexesType );
        pFaces = pAniMeshRes->NewFaces( string((*itGeom)->NodeName), pVectexes );
        pGeom = pAniMeshRes->NewGeometry( string((*itGeom)->NodeName), pVectexes, pFaces );
        
        pGeom->SetBoneNodeName( string((*itGeom)->BoneNodeName) );
        pGeom->SetAnimationMatrixIndex( (*itGeom)->AnimationMatrixIndex );
        pGeom->SetOBBCenter( (*itGeom)->vecCenter );
        pGeom->SetOBBAxis( (*itGeom)->vecAxis );
        pGeom->SetOBBAxisLen( (*itGeom)->AxisLen );
        pGeom->SetOBB( (*itGeom)->vecOBB );
        pGeom->SetMin( (*itGeom)->vecMin );
        pGeom->SetMax( (*itGeom)->vecMax );
        pGeom->SetMeshVertexesType( (*itGeom)->MeshVertexesType );
        pGeom->SetMaterialName( string((*itGeom)->MaterialName) );
        pGeom->SetSubMaterialName( string( (*itGeom)->SubMaterialName ) );
        pGeom->SetShaderEffect( GetEngine()->ShaderLoad<TKMeshShaderEffect>((*itGeom)->ShaderName, "TShader") );
        if ( !pAniMeshRes->GetShaderEffect() )
        {
            pAniMeshRes->SetShaderEffect( pGeom->GetShaderEffect() );
        }

        // OriginVertexList
        for ( int i = 0 ; i < (int)(*itGeom)->OriginPosList.size() ; ++i )
        {
            St_Geom_Vertex.Pos = (*itGeom)->OriginPosList[i].Pos;
            for ( int j = 0 ; j < 4 ; ++j )
            {
                St_Geom_Vertex.Weights[j].SetParentNodeName( string( (*itGeom)->OriginPosList[i].Weights[j].ParentNode ) );
                St_Geom_Vertex.Weights[j].Weight = (*itGeom)->OriginPosList[i].Weights[j].Weight;
            }
            pGeom->GetOriginVertexList().push_back( St_Geom_Vertex );
        }
        // PosList
        pGeom->GetPosList()         = (*itGeom)->PosList;
        // MatrixWeightList
        pGeom->GetMatrixWeightList() = (*itGeom)->MatrixWeightList;
        // MatrixIndexList
        pGeom->GetMatrixIndexList() = (*itGeom)->MatrixIndexList;
        // TexPosList
        pGeom->GetTexPosList()      = (*itGeom)->TexPosList;
        // ColorList
        pGeom->GetColorList()       = (*itGeom)->ColorList;
        // NormalList
        pGeom->GetNormalList()      = (*itGeom)->NormalList;
        // FaceList
        pGeom->GetFaceList()        = (*itGeom)->FaceList;
        // AniMatrixList
        pGeom->GetAniMatrixList()   = (*itGeom)->AniMatrixList;
    }
#endif
}

void TKCustomFileConverter::ExtractGeometryToFile(ST_GEOMETRIES *pDestList, GEOMLIST *pSrcList )
{
#if 0
	ST_GEOMETRY *pSt_Geometry;
    GEOMLIST::iterator itGeom;

    ST_GEOMETRY::ST_VERTEX St_Geom_Vertex;
    for ( itGeom = pSrcList->begin() ; itGeom != pSrcList->end() ; ++itGeom )
    {
        pSt_Geometry = new ST_GEOMETRY;
        pSt_Geometry->Clear();
        
        strcpy(pSt_Geometry->NodeName, (*itGeom)->GetNodeName().data());
        strcpy(pSt_Geometry->BoneNodeName, (*itGeom)->GetBoneNodeName().data());
        pSt_Geometry->AnimationMatrixIndex = (*itGeom)->GetAnimationMatrixIndex();
        pSt_Geometry->vecCenter = (*itGeom)->GetOBBCenter();
        memcpy(pSt_Geometry->vecAxis, (*itGeom)->GetOBBAxis(), sizeof(pSt_Geometry->vecAxis));
        memcpy(pSt_Geometry->AxisLen, (*itGeom)->GetOBBAxisLen(), sizeof(pSt_Geometry->AxisLen));
        memcpy(pSt_Geometry->vecOBB, (*itGeom)->GetOBB(), sizeof(pSt_Geometry->vecOBB));                
        pSt_Geometry->vecMin = (*itGeom)->GetMin();
        pSt_Geometry->vecMax = (*itGeom)->GetMax();
        pSt_Geometry->MeshVertexesType = (*itGeom)->GetMeshVertexesType();
        strcpy(pSt_Geometry->MaterialName, (*itGeom)->GetMaterialName().data());
        strcpy(pSt_Geometry->SubMaterialName, (*itGeom)->GetSubMaterialName().data());
        strcpy(pSt_Geometry->ShaderName, (*itGeom)->GetShaderEffect()->GetShaderFileName().data());

        // OriginVertexList
        for ( int i = 0 ; i < (int)(*itGeom)->GetOriginVertexList().size() ; ++i )
        {
            St_Geom_Vertex.Pos = (*itGeom)->GetOriginVertexList()[i].Pos;
            for ( int j = 0 ; j < 4 ; ++j )
            {
                strcpy(St_Geom_Vertex.Weights[j].ParentNode, (*itGeom)->GetOriginVertexList()[i].Weights[j].GetParentNodeName().data());
                St_Geom_Vertex.Weights[j].Weight = (*itGeom)->GetOriginVertexList()[i].Weights[j].Weight;
            }
            pSt_Geometry->OriginPosList.push_back( St_Geom_Vertex );
        }
        // PosList
        pSt_Geometry->PosList           = (*itGeom)->GetPosList();
        // MatrixWeightList
        pSt_Geometry->MatrixWeightList  = (*itGeom)->GetMatrixWeightList();
        // MatrixIndexList
        pSt_Geometry->MatrixIndexList   = (*itGeom)->GetMatrixIndexList();
        // TexPosList
        pSt_Geometry->TexPosList        = (*itGeom)->GetTexPosList();
        // ColorList
        pSt_Geometry->ColorList         = (*itGeom)->GetColorList();
        // NormalList
        pSt_Geometry->NormalList        = (*itGeom)->GetNormalList();
        // FaceList
        pSt_Geometry->FaceList          = (*itGeom)->GetFaceList();
        // AniMatrixList
        pSt_Geometry->AniMatrixList     = (*itGeom)->GetAniMatrixList();
        
        pDestList->push_back( pSt_Geometry );
    }
#endif
}

void TKCustomFileConverter::ExtractLODDataFromFile(FILESTRUCT::ST_MESHLODINFOS *pSrcList)
{
    ST_MESHLODINFOS::iterator it;

    pAniMeshRes->ClearAllLODGeometroy();
    for ( it = pSrcList->begin() ; it != pSrcList->end() ; ++it )
    {
        pAniMeshRes->NewLODDistance( (*it).Level );
        pAniMeshRes->AddLODGeometry( (*it).Level,
                                  pAniMeshRes->GetGeomByNodeName( string( (*it).NodeName ) ) );
    }
}

void TKCustomFileConverter::ExtractLODDataToFile(FILESTRUCT::ST_MESHLODINFOS *pDestList, TKMeshRes *pSrcRes)
{
    ST_MESHLODINFO St_MeshLodInfo;
    LODLIST::iterator it;
    GEOMLIST::iterator itGeom;
    
    for ( it = pSrcRes->GetLodList().begin() ; it != pSrcRes->GetLodList().end() ; ++it )
    {
        St_MeshLodInfo.Level = it->first;
        for ( itGeom = it->second->begin(); itGeom != it->second->end(); ++itGeom )
        {
            strcpy( St_MeshLodInfo.NodeName, (*itGeom)->GetNodeName().data() );
            pDestList->push_back( St_MeshLodInfo );
        }
    }
}

void TKCustomFileConverter::ExtractAniMotionFromFile(FILESTRUCT::ST_ANIMOTIONINFOS *pSrcList)
{
    ST_ANIMOTION DestData;
    
    int iSize = (int)pSrcList->size();

    pAniMeshRes->GetMotionList().clear();
    for ( int i = 0 ; i < iSize ; ++i )
    {
        DestData.StartFrame = (*pSrcList)[i].StartFrame;
        DestData.EndFrame   = (*pSrcList)[i].EndFrame;
        DestData.MotionName = (*pSrcList)[i].AnimationName;
        pAniMeshRes->GetMotionList().insert( make_pair( DestData.MotionName, DestData ) );
    }
}

void TKCustomFileConverter::ExtractAniMotionToFile(ST_ANIMOTIONINFOS *pDestList, MOTIONLIST *pSrcList)
{
    ST_ANIMOTIONINFO Max_AniMotionInfo;
    MOTIONLIST::iterator it;
    for ( it = pSrcList->begin() ;
          it != pSrcList->end() ;
          ++it )
    {
        strcpy(Max_AniMotionInfo.AnimationName, it->second.MotionName.data());
        Max_AniMotionInfo.StartFrame = it->second.StartFrame;
        Max_AniMotionInfo.EndFrame = it->second.EndFrame;
        pDestList->push_back( Max_AniMotionInfo );
        ++Max_AniMotionInfo.Index;
    }
}

bool TKCustomFileConverter::ExtractCollisionFromFile(ST_COLLISION *pSrcData, TKCollisionResource *pDestData)
{
#if 0
	if ( dynamic_cast<TKCollisionObjectRes*>(pDestData) == NULL )
    {
        pDestData->SetNodeName( string(pSrcData->NodeName) );
    }
    pDestData->SetBoneNodeName( string(pSrcData->BoneNodeName) );

    int iSize;

    iSize = (int)pSrcData->PosList.size();
    pDestData->GetPosList().resize( iSize );
    if ( iSize > 0 ) memcpy( &pDestData->GetPosList()[0], &pSrcData->PosList[0], sizeof(D3DXVECTOR3) * iSize );

    iSize = (int)pSrcData->NormalList.size();
    pDestData->GetNormalList().resize( iSize );
    if ( iSize > 0 ) memcpy( &pDestData->GetNormalList()[0], &pSrcData->NormalList[0], sizeof(D3DXVECTOR3) * iSize );

    iSize = (int)pSrcData->FaceList.size();
    pDestData->GetFaceList().resize( iSize );
    if ( iSize > 0 ) memcpy( &pDestData->GetFaceList()[0], &pSrcData->FaceList[0], sizeof(ST_IB_TRIANGLE) * iSize );

    iSize = (int)pSrcData->FaceNormalList.size();
    pDestData->GetFaceNormalList().resize( iSize );
    if ( iSize > 0 ) memcpy( &pDestData->GetFaceNormalList()[0], &pSrcData->FaceNormalList[0], sizeof(D3DXVECTOR3) * iSize );

    pDestData->SetCenter( pSrcData->Center );
    pDestData->SetMin   ( pSrcData->Min    );
    pDestData->SetMax   ( pSrcData->Max    );
    pDestData->SetRadius( pSrcData->Radius );

    if ( dynamic_cast<TKCollisionObjectRes*>(pDestData) )
    {
        ((TKCollisionObjectRes*)pDestData)->SetIsMeshAnimation( pSrcData->IsMeshAnimation );
        ((TKCollisionObjectRes*)pDestData)->SetIsDynamicObject( pSrcData->IsDynamicObject );
    }
    if ( dynamic_cast<TKOBBCollision*>(pDestData) )
    {
        ((TKOBBCollision*)pDestData)->SetAxisLen( pSrcData->AxisLen );
        ((TKOBBCollision*)pDestData)->SetAxisDir( pSrcData->vecAxisDir );
        ((TKOBBCollision*)pDestData)->SetOBB    ( pSrcData->vecOBB );
    }

    TKCollisionResource *pNewCollisionRes;
    vector<ST_COLLISION*>::iterator it;

    for ( it = pSrcData->Items.begin() ; it != pSrcData->Items.end() ; ++it )
    {
        switch ( (*it)->ObjectType )
        {
            case eotFaceCollision:
            {
                pNewCollisionRes = new TKFaceCollision(pEngine, GENID);
                pDestData->GetCollisionResList().push_back ( pNewCollisionRes );
                ExtractCollisionFromFile( (*it), pNewCollisionRes );
            }
            break;
            case eotSphereCollision:
            {
                pNewCollisionRes = new TKSphereCollision(pEngine, GENID);
                pDestData->GetCollisionResList().push_back ( pNewCollisionRes );
                ExtractCollisionFromFile( (*it), pNewCollisionRes );
            }
            break;
            case eotOBBCollision:
            {
                pNewCollisionRes = new TKOBBCollision(pEngine, GENID);
                pDestData->GetCollisionResList().push_back ( pNewCollisionRes );
                ExtractCollisionFromFile( (*it), pNewCollisionRes );
            }
            break;
            default:
            {
                MessageBox(0, "pSrcData->ObjectType fault", "TKCollisionFileConverter::ExtractCollisionFromFile", MB_ICONERROR);
                _ASSERT(0);
            }
        }
    }
#endif
    return true;
}


bool TKCustomFileConverter::ExtractCollisionToFile(TKCollisionResource *pSrcData, ST_COLLISION *pDestData)
{
    pDestData->ObjectType = pSrcData->GetObjectType();

    strcpy(pDestData->NodeName      , pSrcData->GetNodeName().data() );
    strcpy(pDestData->BoneNodeName  , pSrcData->GetBoneNodeName().data() );

    int iSize;

    iSize = (int)pSrcData->GetPosList().size();
    pDestData->PosList.resize( iSize );
    if ( iSize > 0 ) memcpy( &pDestData->PosList[0], &pSrcData->GetPosList()[0], sizeof(D3DXVECTOR3) * iSize );

    iSize = (int)pSrcData->GetNormalList().size();
    pDestData->NormalList.resize( iSize );
    if ( iSize > 0 ) memcpy( &pDestData->NormalList[0], &pSrcData->GetNormalList()[0], sizeof(D3DXVECTOR3) * iSize );

    iSize = (int)pSrcData->GetFaceList().size();
    pDestData->FaceList.resize( iSize );
    if ( iSize > 0 ) memcpy( &pDestData->FaceList[0], &pSrcData->GetFaceList()[0], sizeof(ST_IB_TRIANGLE) * iSize );

    iSize = (int)pSrcData->GetFaceNormalList().size();
    pDestData->FaceNormalList.resize( iSize );
    if ( iSize > 0 ) memcpy( &pDestData->FaceNormalList[0], &pSrcData->GetFaceNormalList()[0], sizeof(D3DXVECTOR3) * iSize );

    pDestData->Center   = pSrcData->GetCenter();
    pDestData->Min      = pSrcData->GetMin();
    pDestData->Max      = pSrcData->GetMax();
    pDestData->Radius   = pSrcData->GetRadius();

    if ( dynamic_cast<TKCollisionObjectRes*>(pSrcData) )
    {
        pDestData->IsMeshAnimation = ((TKCollisionObjectRes*)pSrcData)->GetIsMeshAnimation();
        pDestData->IsDynamicObject = ((TKCollisionObjectRes*)pSrcData)->GetIsDynamicObject();
    }
    if ( dynamic_cast<TKOBBCollision*>(pSrcData) )
    {
        memcpy( pDestData->AxisLen   , ((TKOBBCollision*)pSrcData)->GetAxisLen(), sizeof(pDestData->AxisLen) );
        memcpy( pDestData->vecAxisDir, ((TKOBBCollision*)pSrcData)->GetAxisDir(), sizeof(pDestData->vecAxisDir) );
        memcpy( pDestData->vecOBB    , ((TKOBBCollision*)pSrcData)->GetOBB()    , sizeof(pDestData->vecOBB) );
    }

    ST_COLLISION *pNewCollisionExport;
    COLLISIONRESLIST::iterator it;

    for ( it = pSrcData->GetCollisionResList().begin() ; it != pSrcData->GetCollisionResList().end() ; ++it )
    {
        pNewCollisionExport = new ST_COLLISION;
        pDestData->Items.push_back( pNewCollisionExport );
        ExtractCollisionToFile( (*it), pNewCollisionExport );
    }

    return true;
}

bool TKCustomFileConverter::BindMeshAndMaterial(void)
{
    // 메터리얼이름으로 메터리얼객체를 찾는다.
    pAniMeshRes->MaterialBind( pAniMeshRes->GetGeomList() );
    return true;
}


bool TKCustomFileConverter::CheckValidateIOData(FILESTRUCT::ST_MESHLODINFOS *pSrcList)
{
    ST_MESHLODINFOS::iterator it;
    for ( it  = pSrcList->begin();
          it != pSrcList->end();
          ++it )
    {
        // 메쉬리소스에서 지오메트리를 검색한다.
        if ( pAniMeshRes->GetGeomByNodeName( string(it->NodeName) ) == NULL )
        {
            // 데이터가 없으면 false
            return false;
        }
    }
    return true;
}









/////////////////////////////////////////////////////////////////////
// TKMAXFileConverter
TKMAXFileConverter::TKMAXFileConverter(void)
:TKCustomFileConverter()
{
}

TKMAXFileConverter::~TKMAXFileConverter(void)
{
}

bool TKMAXFileConverter::ConvertFromKMF(string & FileName, TKEngine *pEngine, TKAniMeshRes *pAniMeshRes)
{
    this->pEngine = pEngine;
    this->pAniMeshRes = pAniMeshRes;

    if (!MAXFileIO.LoadFromFile(FileName.data()))
    {
        _ASSERT(0);
        return false;
    }

    // Scene 정보
    pAniMeshRes->m_FirstFrame = MAXFileIO.Scene.FirstFrame;
    pAniMeshRes->m_LastFrame  = MAXFileIO.Scene.LastFrame;
    pAniMeshRes->m_FrameSpeed = MAXFileIO.Scene.FrameSpeed;

    // Material 정보
    ExtractMaterialListFromMAX(&(MAXFileIO.Material_List.Items), NULL);

    // Bone/BoneAnimation 정보
    ExtractBoneListFromMAX( &(MAXFileIO.GeomObject_List.Items) );

    // Mesh 정보
    ExtractGeomListFromMAX( &(MAXFileIO.GeomObject_List.Items) );

    // Mesh와 Material을 바인드
    BindMeshAndMaterial();

    return true;
}

/////////////////////////////////////////////////////////////////////
// TKModelFileConverter

TKModelFileConverter::TKModelFileConverter(void)
:TKCustomFileConverter()
{
}

TKModelFileConverter::~TKModelFileConverter(void)
{
}

bool TKModelFileConverter::ConvertFromKMT(string & FileName, TKEngine *pEngine, TKAniMeshRes *pAniMeshRes)
{
    this->pEngine = pEngine;
    this->pAniMeshRes = pAniMeshRes;

    if (!ModelFileIO.LoadFromFile(FileName.data()))
    {
        _ASSERT(0);
        return false;
    }
    
    pAniMeshRes->ClearDelete();

    // 기본정보
    pAniMeshRes->m_FirstFrame      = ModelFileIO.MeshResData.FirstFrame;
    pAniMeshRes->m_LastFrame       = ModelFileIO.MeshResData.LastFrame;
    pAniMeshRes->m_FrameSpeed      = ModelFileIO.MeshResData.FrameSpeed;
    
    // 메터리얼 정보
    ExtractMaterialFromFile<TKMeshRes>( pAniMeshRes, &ModelFileIO.MeshResData.MaterialList );
    // 메쉬 정보
    ExtractGeometryFromFile( &ModelFileIO.MeshResData.GeomList );    
    // 본 정보
    ExtractGeometryFromFile( &ModelFileIO.MeshResData.BoneList );    
    // Mesh와 Material을 바인드
    BindMeshAndMaterial();
    // 애니메이션 파일 읽기
    ExtractAniMotionFromFile( &(ModelFileIO.AniMotionList.Items) );
    // LOD 정보 변환
    ExtractLODDataFromFile( &(ModelFileIO.MeshLodList.Items) );
    
    return true;
}

bool TKModelFileConverter::ConvertToKMT(string & FileName, TKEngine *pEngine, TKAniMeshRes *pAniMeshRes)
{
    this->pEngine = pEngine;
    this->pAniMeshRes = pAniMeshRes;

    ModelFileIO.Clear();
    
    // 기본정보
    ModelFileIO.MeshResData.FirstFrame      = pAniMeshRes->m_FirstFrame;
    ModelFileIO.MeshResData.LastFrame       = pAniMeshRes->m_LastFrame;
    ModelFileIO.MeshResData.FrameSpeed      = pAniMeshRes->m_FrameSpeed;

    // 메터리얼 정보
    ExtractMaterialToFile<TKMeshRes>( &ModelFileIO.MeshResData.MaterialList, pAniMeshRes );
    // 메쉬 정보
    ExtractGeometryToFile( &ModelFileIO.MeshResData.GeomList, &pAniMeshRes->GeomList );
    // 본 정보
    ExtractGeometryToFile( &ModelFileIO.MeshResData.BoneList, &pAniMeshRes->BoneList );
    // 애니메이션 파일 읽기
    ExtractAniMotionToFile( &(ModelFileIO.AniMotionList.Items), &(pAniMeshRes->GetMotionList()) );
    // LOD 정보 변환
    ExtractLODDataToFile( &ModelFileIO.MeshLodList.Items, pAniMeshRes );
    
    return ModelFileIO.SaveToFile( FileName.data() );
}

/////////////////////////////////////////////////////////////////////
// TKLODFileConverter
TKLODFileConverter::TKLODFileConverter(void)
:TKCustomFileConverter()
{
}

TKLODFileConverter::~TKLODFileConverter(void)
{
}

bool TKLODFileConverter::ConvertFromKLF(string & FileName, TKEngine *pEngine, TKAniMeshRes *pAniMeshRes)
{
    this->pEngine = pEngine;
    this->pAniMeshRes = pAniMeshRes;

    if (!LODFileIO.LoadFromFile(FileName.data()))
    {
        _ASSERT(0);
        return false;
    }
    
    if ( !CheckValidateIOData( &(LODFileIO.MeshLodList.Items) ) )
    {
        return false;
    }

    ExtractLODDataFromFile( &(LODFileIO.MeshLodList.Items) );

    return true;
}

bool TKLODFileConverter::ConvertToKLF(string & FileName, TKEngine *pEngine, TKAniMeshRes *pAniMeshRes)
{
    this->pEngine = pEngine;
    this->pAniMeshRes = pAniMeshRes;
    
    LODFileIO.Clear();
    
    ExtractLODDataToFile( &LODFileIO.MeshLodList.Items, pAniMeshRes );
    
    return LODFileIO.SaveToFile( FileName.data() );
}

/////////////////////////////////////////////////////////////////////
// TKMotionFileConverter
TKMotionFileConverter::TKMotionFileConverter(void)
:TKCustomFileConverter()
{
}

TKMotionFileConverter::~TKMotionFileConverter(void)
{
}

bool TKMotionFileConverter::ConvertFromKNF(string & FileName, TKEngine *pEngine, TKAniMeshRes *pAniMeshRes)
{
    this->pEngine = pEngine;
    this->pAniMeshRes = pAniMeshRes;

    if (!MotionFileIO.LoadFromFile(FileName.data()))
    {
        _ASSERT(0);
        return false;
    }

    // AniMotion 정보
    ExtractAniMotionFromFile( &(MotionFileIO.AniMotionList.Items) );
    
    return true;
}

bool TKMotionFileConverter::ConvertToKNF(string & FileName, TKEngine *pEngine, TKAniMeshRes *pAniMeshRes)
{
    this->pEngine = pEngine;
    this->pAniMeshRes = pAniMeshRes;
    
    MotionFileIO.Clear();
    ExtractAniMotionToFile( &(MotionFileIO.AniMotionList.Items), &(pAniMeshRes->GetMotionList()) );
    
    return MotionFileIO.SaveToFile( FileName.data() );
}

/////////////////////////////////////////////////////////////////////////////////
//TKCollisionFileConverter
/////////////////////////////////////////////////////////////////////////////////
TKCollisionFileConverter::TKCollisionFileConverter(void)
:TKCustomFileConverter()
{
}

TKCollisionFileConverter::~TKCollisionFileConverter(void)
{
}

bool TKCollisionFileConverter::ConvertFromKCF( string & FileName,
                                        TKEngine *pEngine,
                                        TKCollisionObjectRes *pCollisionRes,
                                        ECollisionObjectType CollisionObjectType)
{
    this->pEngine = pEngine;
    this->pCollisionRes = pCollisionRes;
    this->CollisionObjectType = CollisionObjectType;

    if (!MAXFileIO.LoadFromFile(FileName.data()))
    {
        _ASSERT(0);
    }

    // Collision 정보
    ExtractCollisionListFromMAX( &(MAXFileIO.GeomObject_List.Items) );
    pCollisionRes->CreateCollisionTreeByOrigin(&OriginCollisionList, CollisionObjectType );

    // 임시로 사용하던 오리진 정보 삭제
    for ( int i = 0 ; i < (int)OriginCollisionList.size() ; ++i )
    {
        delete OriginCollisionList[i];
    }
    OriginCollisionList.clear();

    return true;
}

bool TKCollisionFileConverter::ConvertFromKCT( string & FileName,
                                        TKEngine *pEngine,
                                        TKCollisionObjectRes *pCollisionRes)
{
    this->pEngine = pEngine;
    this->pCollisionRes = pCollisionRes;
    KCTFileIO.Clear();
    KCTFileIO.LoadFromFile( FileName.data() );
    pCollisionRes->ClearDelete();
    ExtractCollisionFromFile( &KCTFileIO.Collision, pCollisionRes );
    return true;
}


bool TKCollisionFileConverter::ConvertToKCT( string & FileName,
                                      TKEngine *pEngine,
                                      TKCollisionObjectRes *pCollisionRes)
{
    this->pEngine = pEngine;
    this->pCollisionRes = pCollisionRes;
    KCTFileIO.Clear();
    ExtractCollisionToFile( pCollisionRes, &KCTFileIO.Collision );
    KCTFileIO.SaveToFile( FileName.data() );
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
//TKSGObjectFileConverter
/////////////////////////////////////////////////////////////////////////////////
TKSGObjectFileConverter::TKSGObjectFileConverter(void)
:TKCustomFileConverter()
{
}

TKSGObjectFileConverter::~TKSGObjectFileConverter(void)
{
}

pair<TKMeshRes *, TKCollisionObjectRes *> TKSGObjectFileConverter::ConvertFromKSG
(string & FileName, TKEngine *pEngine)
{
	this->pEngine = pEngine;
	this->pAniMeshRes = NULL;
	this->pCollisionRes = NULL;

	pair<TKMeshRes *, TKCollisionObjectRes *> RtnValue;
	char strTemp[SBUFSIZE], strFileName[SBUFSIZE];
#if 0    
	ExtractFileNameWithExt(FileName.data(), strFileName);
	strcpy(strTemp, strFileName);
	StringReplace(strTemp, ".KSG", "_MESH");
	string MeshResName(strTemp);

	strcpy(strTemp, strFileName);
	StringReplace(strTemp, ".KSG", "_COLL");
	string CollResName(strTemp);

	pAniMeshRes = R_ENGINERESOURCEMANAGER->Item<TKAniMeshRes>(MeshResName);
	pCollisionRes = R_ENGINERESOURCEMANAGER->Item<TKCollisionObjectRes>(CollResName);
	if (pAniMeshRes || pCollisionRes)
	{
		RtnValue.first = pAniMeshRes;
		RtnValue.second = pCollisionRes;
		return RtnValue;
	}

	if (!pAniMeshRes)
	{
		pAniMeshRes = R_ENGINERESOURCEMANAGER->New<TKAniMeshRes>(MeshResName);
	}
	if (!pCollisionRes)
	{
		pCollisionRes = R_ENGINERESOURCEMANAGER->New<TKCollisionObjectRes>(CollResName);
	}

	pAniMeshRes->ClearDelete();
	pCollisionRes->ClearDelete();

	SGObjectFileIO.LoadFromFile(FileName.data());

	// 기본정보
	pAniMeshRes->m_FirstFrame = SGObjectFileIO.MeshResData.FirstFrame;
	pAniMeshRes->m_LastFrame = SGObjectFileIO.MeshResData.LastFrame;
	pAniMeshRes->m_FrameSpeed = SGObjectFileIO.MeshResData.FrameSpeed;

	// 메터리얼 정보
	ExtractMaterialFromFile<TKMeshRes>(pAniMeshRes, &SGObjectFileIO.MeshResData.MaterialList);
	// 메쉬 정보
	ExtractGeometryFromFile(&SGObjectFileIO.MeshResData.GeomList);
	// 본 정보
	ExtractGeometryFromFile(&SGObjectFileIO.MeshResData.BoneList);
	// Mesh와 Material을 바인드
	BindMeshAndMaterial();
	// 애니메이션 파일 읽기
	ExtractAniMotionFromFile(&(SGObjectFileIO.AniMotionList.Items));
	// LOD 정보 변환
	ExtractLODDataFromFile(&(SGObjectFileIO.MeshLodList.Items));
	// 충돌데이터 변환
	ExtractCollisionFromFile(&SGObjectFileIO.Collision, pCollisionRes);

	pCollisionRes->SetMeshRes(pAniMeshRes);

	RtnValue.first = pAniMeshRes;
	RtnValue.second = pCollisionRes;

	RtnValue.first->Init();
	RtnValue.second->Init();
#endif    
	return RtnValue;
}

bool TKSGObjectFileConverter::ConvertToKSG( string & FileName, 
                                            TKEngine *pEngine, 
                                            TKAniMeshRes *pAniMeshRes,
                                            TKCollisionObjectRes *pCollisionRes)
{
    this->pEngine = pEngine;
    this->pAniMeshRes = pAniMeshRes;
    this->pCollisionRes = pCollisionRes;
    
    SGObjectFileIO.Clear();
    
    // 기본정보
    SGObjectFileIO.MeshResData.FirstFrame      = pAniMeshRes->m_FirstFrame;
    SGObjectFileIO.MeshResData.LastFrame       = pAniMeshRes->m_LastFrame;
    SGObjectFileIO.MeshResData.FrameSpeed      = pAniMeshRes->m_FrameSpeed;

    // 메터리얼 정보
    ExtractMaterialToFile<TKMeshRes>( &SGObjectFileIO.MeshResData.MaterialList, pAniMeshRes );
    // 메쉬 정보
    ExtractGeometryToFile( &SGObjectFileIO.MeshResData.GeomList, &pAniMeshRes->GeomList );
    // 본 정보
    ExtractGeometryToFile( &SGObjectFileIO.MeshResData.BoneList, &pAniMeshRes->BoneList );
    // 애니메이션 파일 읽기
    ExtractAniMotionToFile( &(SGObjectFileIO.AniMotionList.Items), &(pAniMeshRes->GetMotionList()) );
    // LOD 정보 변환
    ExtractLODDataToFile( &SGObjectFileIO.MeshLodList.Items, pAniMeshRes );
    // 충돌데이터 변환
    ExtractCollisionToFile( pCollisionRes, &SGObjectFileIO.Collision );   
    
    SGObjectFileIO.SaveToFile( FileName.data() );
    
    return true;
}