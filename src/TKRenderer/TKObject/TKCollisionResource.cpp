#include "TKCollisionResource.h"

#include "TKMeshResource.h"
#include "TKFileConvertClasses.h"

/////////////////////////////////////////////////////////////////////
// ST_ORIGINCOLLISIONDATA
/////////////////////////////////////////////////////////////////////
ST_ORIGINCOLLISIONDATA::ST_ORIGINCOLLISIONDATA(void)
{
    ObjectType = eotNULL;
    NodeName = "";
    BoneNodeName = "";
}

ST_ORIGINCOLLISIONDATA::ST_ORIGINCOLLISIONDATA(ST_ORIGINCOLLISIONDATA & st_OriginCollisionData)
{
    ObjectType = eotNULL;
    NodeName = "";
    BoneNodeName = "";
    (*this) = st_OriginCollisionData;
}

ST_ORIGINCOLLISIONDATA::~ST_ORIGINCOLLISIONDATA(void)
{
}

const ST_ORIGINCOLLISIONDATA & ST_ORIGINCOLLISIONDATA::operator = (const ST_ORIGINCOLLISIONDATA & st_ORIGINCOLLISIONDATA)
{
    this->ObjectType = st_ORIGINCOLLISIONDATA.ObjectType;
    this->NodeName = st_ORIGINCOLLISIONDATA.NodeName;
    this->BoneNodeName = st_ORIGINCOLLISIONDATA.BoneNodeName;
    this->PosList = st_ORIGINCOLLISIONDATA.PosList;
    this->NormalList = st_ORIGINCOLLISIONDATA.NormalList;
    this->FaceList = st_ORIGINCOLLISIONDATA.FaceList;
    
    this->vecCenter = st_ORIGINCOLLISIONDATA.vecCenter;
    memcpy(this->AxisLen, st_ORIGINCOLLISIONDATA.AxisLen, sizeof(this->AxisLen));
    memcpy(this->vecAxisDir, st_ORIGINCOLLISIONDATA.vecAxisDir, sizeof(this->vecAxisDir));
    memcpy(this->vecOBB, st_ORIGINCOLLISIONDATA.vecOBB, sizeof(this->vecOBB));

    return st_ORIGINCOLLISIONDATA;
}


void ST_ORIGINCOLLISIONDATA::ExtractFromGEOMLIST(GEOMLIST & GeomList)
{
    int VertexCount = 0, FaceCount = 0;
    int i = 0, j = 0, iVertexOffset = 0, iFaceOffset = 0, iStructSize = 0;
    iStructSize = sizeof(D3DXVECTOR3);

    // 메쉬리소스의 버텍스 카운트와, 페이스 카운트를 카운팅한다.
    for ( i = 0 ; i < (int)GeomList.size() ; ++i )
    {
        VertexCount += (int)GeomList[i]->GetPosList().size();
        FaceCount += (int)GeomList[i]->GetFaceList().size();
    }

    // 카운팅된 버텍스/페이스 카운트로 컨테이너 크기를 재정의한다.
    this->PosList.resize( VertexCount );
    this->NormalList.resize( VertexCount );
    this->FaceList.resize( FaceCount );

    // 메쉬리소스에서 버텍스/노말/페이스 정보를 복사해온다.
    for ( i = 0 ; i < (int)GeomList.size() ; ++i )
    {
        D3DXVECTOR3S & MeshPosList = GeomList[i]->GetPosList();
        D3DXVECTOR3S & MeshNormalList = GeomList[i]->GetNormalList();
        vector<ST_IB_TRIANGLE> & MeshFaceList = GeomList[i]->GetFaceList();

        if (!MeshPosList.empty())
            memcpy(&this->PosList[iVertexOffset], &MeshPosList[0], iStructSize * MeshPosList.size() );
        if (!MeshNormalList.empty())
            memcpy(&this->NormalList[iVertexOffset], &MeshNormalList[0], iStructSize * MeshNormalList.size() );
        if (!MeshFaceList.empty())
            memcpy(&this->FaceList[iFaceOffset], &MeshFaceList[0], sizeof(ST_IB_TRIANGLE) * MeshFaceList.size() );
        for ( j = 0 ; j < (int)MeshFaceList.size() ; ++j )
        {
            this->FaceList[j + iFaceOffset]._0 += iVertexOffset;
            this->FaceList[j + iFaceOffset]._1 += iVertexOffset;
            this->FaceList[j + iFaceOffset]._2 += iVertexOffset;
        }

        iVertexOffset += (int)MeshPosList.size();
        iFaceOffset += (int)MeshFaceList.size();
    }
    
    // OBB정보 생성
    BuildOBB(PosList, vecCenter, vecAxisDir, AxisLen, vecOBB);
}

// 메쉬리소스에서 충돌페이스를 추출해온다.
void ST_ORIGINCOLLISIONDATA::ExtractFromMeshRes(TKMeshRes * pMeshRes, ECollisionMeshSourceType CollisionMeshSourceType)
{
    // 메쉬 리소스가 있으면, 메쉬리소스를 가져다가 쓰고.
    // 메쉬 리소스마저 없으면 충돌영역은 텅 빈 깡통이다.
    if (!pMeshRes) return;
    
    switch ( CollisionMeshSourceType )
    {
        case ecmstNULL :
        {
        }
        break;
        case ecmstGeom : 
        {
            LODLIST::iterator itLOD = pMeshRes->GetLodList().begin();
            ExtractFromGEOMLIST( (*itLOD->second) ); 
        }
        break;
        case ecmstBone :
        {
            ExtractFromGEOMLIST( pMeshRes->GetBoneList() );
        }
        break;
    }
}

/////////////////////////////////////////////////////////////////////
// TKCollisionResource
/////////////////////////////////////////////////////////////////////
TKCollisionResource::TKCollisionResource(int ID)
	:TKEngineResource(ID)
	, BoneNodeName("")
	, pBoneMeshGeom(NULL)
	, IsSelected(false)
{
    SetObjectType( eotCollisionResource );

    PosList.clear();
    NormalList.clear();
    FaceList.clear();
    FaceNormalList.clear();

    memset(&vecCenter, 0, sizeof(D3DXVECTOR3));
    memset(&vecMin, 0, sizeof(D3DXVECTOR3));
    memset(&vecMax, 0, sizeof(D3DXVECTOR3));
}

TKCollisionResource::~TKCollisionResource(void)
{
    for (int i = 0 ; i < (int)CollisionResList.size() ; ++i ) delete CollisionResList[i];
    CollisionResList.clear();
}

void TKCollisionResource::BuildMinMax(void)
{
    if (!PosList.empty())
    {
        vecMin = PosList[0];
        vecMax = PosList[0];
    }

    D3DXVECTOR3S::iterator it;
    for ( it = PosList.begin() ; it != PosList.end() ; ++it )
    {
        vecMax.x = max( (*it).x, vecMax.x );
        vecMax.y = max( (*it).y, vecMax.y );
        vecMax.z = max( (*it).z, vecMax.z );

        vecMin.x = min( (*it).x, vecMin.x );
        vecMin.y = min( (*it).y, vecMin.y );
        vecMin.z = min( (*it).z, vecMin.z );
    }
    vecCenter = (vecMax + vecMin) / 2.0f;
    
    D3DXVECTOR3 vecTemp;
    vecTemp = vecMax - vecMin;
    Radius = D3DXVec3Length(&vecTemp);
    Radius /= 2.0f;
    
    //float fx, fy, fz;
    //Radius = 0.0f;

    //fx = abs(vecMax.x - vecMin.x);
    //fy = abs(vecMax.y - vecMin.y);
    //fz = abs(vecMax.z - vecMin.z);

    //Radius = ( fx > fy )? fx : fy ;
    //Radius = ( Radius > fz )? Radius : fz ;

    //Radius /= 2.0f;
}

void TKCollisionResource::BuildMinMaxByChild(COLLISIONRESLIST & CollisionResList)
{
    if (!CollisionResList.empty())
    {
        vecMin = CollisionResList[0]->GetMin();
        vecMax = CollisionResList[0]->GetMax();
    }

    for ( int i = 0 ; i < (int)CollisionResList.size() ; ++i )
    {
        vecMax.x = max( CollisionResList[i]->GetMax().x, vecMax.x );
        vecMax.y = max( CollisionResList[i]->GetMax().y, vecMax.y );
        vecMax.z = max( CollisionResList[i]->GetMax().z, vecMax.z );

        vecMin.x = min( CollisionResList[i]->GetMin().x, vecMin.x );
        vecMin.y = min( CollisionResList[i]->GetMin().y, vecMin.y );
        vecMin.z = min( CollisionResList[i]->GetMin().z, vecMin.z );
    }
    vecCenter = (vecMax + vecMin) / 2.0f;

    D3DXVECTOR3 vecTemp;
    vecTemp = vecMax - vecMin;
    Radius = D3DXVec3Length(&vecTemp);
    Radius /= 2.0f;

    //float fx, fy, fz;
    //Radius = 0.0f;

    //fx = abs(vecMax.x - vecMin.x);
    //fy = abs(vecMax.y - vecMin.y);
    //fz = abs(vecMax.z - vecMin.z);

    //Radius = ( fx > fy )? fx : fy ;
    //Radius = ( Radius > fz )? Radius : fz ;

    //Radius /= 2.0f;
}

void TKCollisionResource::BuildFaceNormal(void)
{
    D3DXVECTOR3 vec[3], vecA, vecB, FaceNormal;

    FaceNormalList.clear();
    FaceNormalList.reserve( FaceList.size() );
    for ( int i = 0 ; i < (int)FaceList.size() ; ++i )
    {
        vec[0] = PosList[ (int)FaceList[i]._0 ];
        vec[1] = PosList[ (int)FaceList[i]._1 ];
        vec[2] = PosList[ (int)FaceList[i]._2 ];
        vecA = vec[1] - vec[0];
        vecB = vec[2] - vec[0];
        D3DXVec3Cross(&FaceNormal, &vecA, &vecB);
        D3DXVec3Normalize(&FaceNormal, &FaceNormal);
        FaceNormalList.push_back(FaceNormal);
    }
}

void TKCollisionResource::Init(void)
{
    __super::Init();

    for ( int i = 0 ; i < (int)CollisionResList.size() ; ++i )
    {
        CollisionResList[i]->Init();
    }
    BuildFaceNormal();
    if ( CollisionResList.size() == 0 )
    {
        BuildMinMax();
    }
    else
    {
        BuildMinMaxByChild(CollisionResList);
    }
}

void TKCollisionResource::Release(void)
{
    for ( int i = 0 ; i < (int)CollisionResList.size() ; ++i )
    {
        CollisionResList[i]->Release();
    }
    __super::Release();
}

void TKCollisionResource::SetTransform(D3DXMATRIX & matWorld, D3DXMATRIX * pmatArray)
{
    for ( int i = 0 ; i < (int)CollisionResList.size() ; ++i )
    {
        CollisionResList[i]->SetTransform( matWorld, pmatArray );
    }

    if ( pBoneMeshGeom && pmatArray )
    {
        matAni = pmatArray[pBoneMeshGeom->GetAnimationMatrixIndex()] * matWorld;
    }
    else
    {
        matAni = matWorld;
    }

    D3DXVec3TransformCoord( &vecCenter, &vecCenter, &matAni );
    vecMin = D3DXVECTOR3( vecCenter.x-Radius, vecCenter.y-Radius, vecCenter.z-Radius );
    vecMax = D3DXVECTOR3( vecCenter.x+Radius, vecCenter.y+Radius, vecCenter.z+Radius );
}

TKCollisionResource & TKCollisionResource::operator = (TKCollisionResource & CollisionResource)
{
    __super::operator=(CollisionResource);

#if 0

    this->SetBoneNodeName( CollisionResource.GetBoneNodeName() );
    this->pBoneMeshGeom = CollisionResource.pBoneMeshGeom;
    this->IsSelected = CollisionResource.IsSelected;

    // 컬리젼리소스 복사하는 부분 시작
    // 복사를 위해 기존 데이터를 지운다.
    for ( int i = 0 ; i < (int)this->CollisionResList.size() ; ++i )
    {
        delete this->CollisionResList[i];
    }

    COLLISIONRESLIST & SrcCollResList = CollisionResource.GetCollisionResList();
    TKCollisionResource * pCollisionResource;

    // 복사를 시작한다.
    this->CollisionResList.resize( SrcCollResList.size() );
    for ( int i = 0 ; i < (int)SrcCollResList.size() ; ++i )
    {
        pCollisionResource = NULL;
        switch ( SrcCollResList[i]->GetObjectType() )
        {
            case eotNULL:
                break;
            case eotFaceCollision:
                pCollisionResource = new TKFaceCollision(GetEngine(), SrcCollResList[i]->GetID());
                *((TKFaceCollision*)pCollisionResource) = *((TKFaceCollision*)SrcCollResList[i]);
                break;
            case eotSphereCollision:
                pCollisionResource = new TKSphereCollision(GetEngine(), SrcCollResList[i]->GetID());
                *((TKSphereCollision*)pCollisionResource) = *((TKSphereCollision*)SrcCollResList[i]);
                break;
            case eotOBBCollision:
                pCollisionResource = new TKOBBCollision(GetEngine(), SrcCollResList[i]->GetID());
                *((TKOBBCollision*)pCollisionResource) = *((TKOBBCollision*)SrcCollResList[i]);
                break;
            case eotRayCollision:
                pCollisionResource = new TKRayCollision(GetEngine(), SrcCollResList[i]->GetID());
                *((TKRayCollision*)pCollisionResource) = *((TKRayCollision*)SrcCollResList[i]);
                break;
        }
        this->CollisionResList[i] = pCollisionResource;
    }
    // 컬리젼리소스 복사하는 부분 종료

    this->PosList        = CollisionResource.PosList;
    this->NormalList     = CollisionResource.NormalList;
    this->FaceList       = CollisionResource.FaceList;
    this->FaceNormalList = CollisionResource.FaceNormalList;

    this->vecMin = CollisionResource.vecMin;
    this->vecMax = CollisionResource.vecMax;
    this->vecCenter = CollisionResource.vecCenter;
    this->Radius = CollisionResource.Radius;
#endif

    return CollisionResource;
}

void TKCollisionResource::Revert(TKCollisionResource * pSourceRes)
{
    if (this->CollisionResList.size() != pSourceRes->CollisionResList.size() )
    {
        MessageBox(0, "Revert Dest/Source Count fault", "__super::Revert", MB_ICONERROR);
        _ASSERT(0);
    }

    for ( int i = 0 ; i < (int)this->CollisionResList.size() ; ++i )
    {
        if (this->CollisionResList[i]->GetObjectType() != pSourceRes->CollisionResList[i]->GetObjectType() )
        {
            MessageBox(0, "Revert Dest/Source Type fault", "__super::Revert", MB_ICONERROR);
            _ASSERT(0);
        }

        this->CollisionResList[i]->Revert ( pSourceRes->CollisionResList[i] );
    }

    this->IsSelected    = pSourceRes->IsSelected;
    this->vecMin        = pSourceRes->vecMin;
    this->vecMax        = pSourceRes->vecMax;
    this->vecCenter     = pSourceRes->vecCenter;
    this->Radius        = pSourceRes->Radius;
}

TKCollisionResource * TKCollisionResource::GetParentCollisionResByID( int ID )
{
    COLLISIONRESLIST::iterator it;
    TKCollisionResource *pRtnColl = NULL;
    
    for ( it = CollisionResList.begin() ; it != CollisionResList.end() ; ++it )
    {
        if ((*it)->GetID() == ID )
        {
            pRtnColl = this;
            break;
        }
        pRtnColl = (*it)->GetParentCollisionResByID( ID );
        if ( pRtnColl ) return pRtnColl;
    }
    return pRtnColl;
}

TKCollisionResource * TKCollisionResource::GetParentCollisionResByNodeName( string & NodeName )
{
    COLLISIONRESLIST::iterator it;
    TKCollisionResource *pRtnColl = NULL;
    
    for ( it = CollisionResList.begin() ; it != CollisionResList.end() ; ++it )
    {
        if ((*it)->GetNodeName() == NodeName )
        {
            pRtnColl = this;
            break;
        }
        pRtnColl = (*it)->GetParentCollisionResByNodeName( NodeName );
        if ( pRtnColl ) return pRtnColl;
    }
    return pRtnColl;
}

TKCollisionResource * TKCollisionResource::GetCollisionResByNodeName( string & NodeName )
{
    COLLISIONRESLIST::iterator it;
    TKCollisionResource *pRtnColl = NULL;
    
    if ( this->GetNodeName() == NodeName )
    {
        return this;
    }
    for ( it = CollisionResList.begin() ; it != CollisionResList.end() ; ++it )
    {
        pRtnColl = (*it)->GetCollisionResByNodeName( NodeName );
        if ( pRtnColl )
        {
            return pRtnColl;
        }
    }
    return pRtnColl;
}

TKCollisionResource * TKCollisionResource::GetCollisionResByBoneNodeName( string & BoneNodeName )
{
    COLLISIONRESLIST::iterator it;
    TKCollisionResource *pRtnColl = NULL;
    
    if ( this->GetBoneNodeName() == BoneNodeName )
    {
        return this;
    }
    for ( it = CollisionResList.begin() ; it != CollisionResList.end() ; ++it )
    {
        pRtnColl = (*it)->GetCollisionResByBoneNodeName( BoneNodeName );
        if ( pRtnColl )
        {
            return pRtnColl;
        }
    }
    return pRtnColl;
}

TKCollisionResource * TKCollisionResource::GetCollisionResByMeshGeometry( TKMeshGeometry * pMeshGeometry )
{
    COLLISIONRESLIST::iterator it;
    TKCollisionResource *pRtnColl = NULL;
    
    if ( this->GetBoneMeshGeom() == pMeshGeometry )
    {
        return this;
    }
    for ( it = CollisionResList.begin() ; it != CollisionResList.end() ; ++it )
    {
        pRtnColl = (*it)->GetCollisionResByMeshGeometry( pMeshGeometry );
        if ( pRtnColl )
        {
            return pRtnColl;
        }
    }
    return pRtnColl;
}

void TKCollisionResource::DeleteCollisionResByID( int ID )
{
    COLLISIONRESLIST::iterator it;
    TKCollisionResource *pRtnColl = NULL;
    
    it = CollisionResList.begin();
    while( it != CollisionResList.end() )
    {
        if ( (*it)->GetID() == ID )
        {
            delete (*it);
            it = CollisionResList.erase( it );        
        }
        else
        {
            (*it)->DeleteCollisionResByID( ID );
            ++it;
        }
    }
}

void TKCollisionResource::DeleteCollisionResByNodeName( string & NodeName )
{
    COLLISIONRESLIST::iterator it;
    TKCollisionResource *pRtnColl = NULL;
    
    it = CollisionResList.begin();
    while( it != CollisionResList.end() )
    {
        if ( (*it)->GetNodeName() == NodeName )
        {
            delete (*it);
            it = CollisionResList.erase( it );        
        }
        else
        {
            (*it)->DeleteCollisionResByNodeName( NodeName );
            ++it;
        }
    }
}

void TKCollisionResource::DeleteCollisionResByBoneNodeName( string & BoneNodeName )
{
    COLLISIONRESLIST::iterator it;
    TKCollisionResource *pRtnColl = NULL;
    
    it = CollisionResList.begin();
    while( it != CollisionResList.end() )
    {
        if ( (*it)->GetBoneNodeName() == BoneNodeName )
        {
            delete (*it);
            it = CollisionResList.erase( it );        
        }
        else
        {
            (*it)->DeleteCollisionResByBoneNodeName( BoneNodeName );
            ++it;
        }
    }
}

void TKCollisionResource::DeleteCollisionResByMeshGeometry( TKMeshGeometry * pMeshGeometry )
{
    COLLISIONRESLIST::iterator it;
    TKCollisionResource *pRtnColl = NULL;
    
    it = CollisionResList.begin();
    while( it != CollisionResList.end() )
    {
        if ( (*it)->GetBoneMeshGeom() == pMeshGeometry )
        {
            delete (*it);
            it = CollisionResList.erase( it );        
        }
        else
        {
            (*it)->DeleteCollisionResByMeshGeometry( pMeshGeometry );
            ++it;
        }
    }
}

void TKCollisionResource::DeleteCollisionResByCollisionRes( TKCollisionResource * pCollisionRes )
{
    COLLISIONRESLIST::iterator it;
    TKCollisionResource *pRtnColl = NULL;
    
    it = CollisionResList.begin();
    while( it != CollisionResList.end() )
    {
        if ( (*it) == pCollisionRes )
        {
            delete (*it);
            it = CollisionResList.erase( it );        
        }
        else
        {
            (*it)->DeleteCollisionResByCollisionRes( pCollisionRes );
            ++it;
        }
    }
}

void TKCollisionResource::SetSelected(bool IsSelected)
{
    this->IsSelected = IsSelected;
    
    COLLISIONRESLIST::iterator it;
    for ( it = CollisionResList.begin() ; it != CollisionResList.end() ; ++it )
    {
        (*it)->SetSelected( IsSelected );
    }
}

/////////////////////////////////////////////////////////////////////
// TKFaceCollision
/////////////////////////////////////////////////////////////////////
TKFaceCollision::TKFaceCollision(int ID)
:TKCollisionResource(ID)
{
    SetObjectType( eotFaceCollision );
}

TKFaceCollision::~TKFaceCollision(void)
{
}

void TKFaceCollision::SetTransform(D3DXMATRIX & matWorld, D3DXMATRIX * pmatArray)
{
    __super::SetTransform(matWorld, pmatArray);

    UINT uStride = sizeof(D3DXVECTOR3);

    if (!PosList.empty())
        D3DXVec3TransformCoordArray(&PosList[0], uStride, &PosList[0], uStride, &matAni, (UINT)PosList.size());
    if (!NormalList.empty())
        D3DXVec3TransformCoordArray(&NormalList[0], uStride, &NormalList[0], uStride, &matAni, (UINT)NormalList.size());
    if (!FaceNormalList.empty())
        D3DXVec3TransformNormalArray(&FaceNormalList[0], uStride, &FaceNormalList[0], uStride, &matAni, (UINT)FaceNormalList.size());
}

TKFaceCollision & TKFaceCollision::operator = (TKFaceCollision & CollisionResource)
{
    __super::operator=(CollisionResource);
    return CollisionResource;
}

void TKFaceCollision::Revert(TKCollisionResource * pSourceRes)
{
    __super::Revert(pSourceRes);

    this->GetPosList()          = pSourceRes->GetPosList();
    this->GetNormalList()       = pSourceRes->GetNormalList();
    this->GetFaceNormalList()   = pSourceRes->GetFaceNormalList();
}

/////////////////////////////////////////////////////////////////////
// TKSphereCollision
/////////////////////////////////////////////////////////////////////
TKSphereCollision::TKSphereCollision(int ID)
:TKCollisionResource(ID)
{
    SetObjectType( eotSphereCollision );
    Radius = 0.0f;
}

TKSphereCollision::~TKSphereCollision(void)
{
}

void TKSphereCollision::Init(void)
{
    __super::Init();
    
    //// 버텍스/페이스 갯수를 최적화
    //if ( VertexList.size() > 3 )
    //{
    //    VertexList.resize(3);
    //    VertexList[0] = vecMin;
    //    VertexList[1] = vecCenter;
    //    VertexList[2] = vecMax;
    //    
    //    // Sphere에서 VertexNormal은 사용하지 않지만 데이터 규격을 맞추기 위해 크기를 잡아준다.
    //    VertexNormal.resize(3);
    //    memcpy(&VertexNormal[0], &VertexList[0], sizeof(D3DXVECTOR3) * 3);
    //    
    //    // Sphere에서 FaceList은 사용하지 않지만 데이터 규격을 맞추기 위해 크기를 잡아준다.
    //    FaceList.resize(1);
    //    FaceList[0 ]._0 = 0;    FaceList[0 ]._1 = 1;    FaceList[0 ]._2 = 2;
    //    
    //    // Sphere에서 FaceNormal은 사용하지 않지만 데이터 규격을 맞추기 위해 크기를 잡아준다.
    //    FaceNormalList.resize(1);
    //    memset(&FaceNormalList[0], 0, sizeof(D3DXVECTOR3));
    //}
}


/////////////////////////////////////////////////////////////////////
// TKOBBCollision
/////////////////////////////////////////////////////////////////////
TKOBBCollision::TKOBBCollision(int ID)
:TKCollisionResource(ID)
{
    SetObjectType( eotOBBCollision );

    memset(AxisLen, 0, sizeof(AxisLen));
    memset(vecAxisDir, 0, sizeof(vecAxisDir));
    memset(&vecMax, 0, sizeof(vecMax));
    memset(&vecMin, 0, sizeof(vecMin));
}

TKOBBCollision::~TKOBBCollision(void)
{
}

bool TKOBBCollision::CreateOBB(void)
{
   // size_t FaceNum = FaceList.size();
   // size_t i, j;
   // float DotRes;
   // bool IsValid = false;

   // for( i = 0 ; ( (i < FaceNum) && (!IsValid) ) ; ++i )
   // {
   //     for( j = 0 ; j < 3 ; ++j )
   //     {
   //         vecAxisDir[0] = VertexList[ (((WORD*)&FaceList[i])[(j+1)%3]) ] - VertexList[ (((WORD*)&FaceList[i])[j]) ];
   //         vecAxisDir[1] = VertexList[ (((WORD*)&FaceList[i])[(j+2)%3]) ] - VertexList[ (((WORD*)&FaceList[i])[j]) ];

   //         DotRes = D3DXVec3Dot( &vecAxisDir[0], &vecAxisDir[1] );

   //         if( -0.0001< DotRes && DotRes < 0.0001 )
   //         {
   //             D3DXVec3Cross( &vecAxisDir[2], &vecAxisDir[0], &vecAxisDir[1] );

   //             IsValid = true;
   //             break;
   //         }
   //     }
   // }

   //if( !IsValid )
   //    return false;

   // AxisLen[0] = D3DXVec3Length( &vecAxisDir[0] ) / 2.0f;
   // AxisLen[1] = D3DXVec3Length( &vecAxisDir[1] ) / 2.0f;

   // D3DXVec3Normalize( &vecAxisDir[0], &vecAxisDir[0] );
   // D3DXVec3Normalize( &vecAxisDir[1], &vecAxisDir[1] );
   // D3DXVec3Normalize( &vecAxisDir[2], &vecAxisDir[2] );

   // D3DXVECTOR3 vecCenterToPoint = VertexList[ FaceList[i]._0 ] - this->Center;

   // AxisLen[2] = D3DXVec3Dot( &vecAxisDir[2], &vecCenterToPoint );
    
    return true;
}

void TKOBBCollision::Init(void)
{
    __super::Init();

    // 버텍스/페이스를 없앤다.
    PosList.clear();
    NormalList.clear();
    FaceList.clear();
    FaceNormalList.clear();
    //if ( VertexList.size() > 3 )
    //{
    //    VertexList.resize(3);
    //    VertexList[0] = vecMin;
    //    VertexList[1] = vecCenter;
    //    VertexList[2] = vecMax;
    //    
    //    // OBB에서 VertexNormal은 사용하지 않지만 데이터 규격을 맞추기 위해 크기를 잡아준다.
    //    VertexNormal.resize(3);
    //    memcpy(&VertexNormal[0], &VertexList[0], sizeof(D3DXVECTOR3) * 3);
    //    
    //    // OBB에서 FaceList은 사용하지 않지만 데이터 규격을 맞추기 위해 크기를 잡아준다.
    //    FaceList.resize(1);
    //    FaceList[0 ]._0 = 0;    FaceList[0 ]._1 = 1;    FaceList[0 ]._2 = 2;
    //    
    //    // OBB에서 FaceNormal은 사용하지 않지만 데이터 규격을 맞추기 위해 크기를 잡아준다.
    //    FaceNormalList.resize(1);
    //    memset(&FaceNormalList[0], 0, sizeof(D3DXVECTOR3));
    //}
}

void TKOBBCollision::SetTransform(D3DXMATRIX & matWorld, D3DXMATRIX * pmatArray )
{
    UINT uStride = sizeof(D3DXVECTOR3);
    
    __super::SetTransform( matWorld, pmatArray );

    D3DXMATRIX matAni2;
    matAni2 = matAni;
    matAni2._41 = 0.0f;
    matAni2._42 = 0.0f;
    matAni2._43 = 0.0f;

    D3DXVec3TransformNormalArray(&vecAxisDir[0], uStride, &vecAxisDir[0], uStride, &matAni2, 3 );

#ifdef DEBUG_MODE
    D3DXVec3TransformCoordArray(&vecOBB[0], uStride, &vecOBB[0], uStride, &matAni, 8 );
#endif DEBUG_MODE
}

TKOBBCollision & TKOBBCollision::operator = (TKOBBCollision & CollisionResource)
{
    __super::operator=(CollisionResource);

    memcpy( this->AxisLen   , CollisionResource.AxisLen   , sizeof(this->AxisLen) );
    memcpy( this->vecAxisDir, CollisionResource.vecAxisDir, sizeof(this->vecAxisDir) );
    memcpy( this->vecOBB    , CollisionResource.vecOBB    , sizeof(this->vecOBB) );
    
    return CollisionResource;
}

void TKOBBCollision::Revert(TKCollisionResource * pSourceRes)
{
    __super::Revert(pSourceRes);
    memcpy( this->vecAxisDir, ((TKOBBCollision*)pSourceRes)->vecAxisDir , sizeof(this->vecAxisDir) );
#ifdef DEBUG_MODE
    memcpy( this->vecOBB    , ((TKOBBCollision*)pSourceRes)->vecOBB     , sizeof(this->vecOBB) );
#endif DEBUG_MODE    
}

void TKOBBCollision::BuildMinMax(void)
{
    vecMin = vecOBB[0];
    vecMax = vecOBB[0];
    
    for ( int i = 0 ; i < 8 ; ++i )
    {
        vecMax.x = max(vecOBB[i].x, vecMax.x);
        vecMax.y = max(vecOBB[i].y, vecMax.y);
        vecMax.z = max(vecOBB[i].z, vecMax.z);
        
        vecMin.x = min(vecOBB[i].x, vecMin.x);
        vecMin.y = min(vecOBB[i].y, vecMin.y);
        vecMin.z = min(vecOBB[i].z, vecMin.z);
    }
    
    vecCenter = (vecMax + vecMin) / 2.0f;
    
    D3DXVECTOR3 vecTemp;
    vecTemp = vecMax - vecMin;
    Radius = D3DXVec3Length(&vecTemp);
    Radius /= 2.0f;
    
    //float fx, fy, fz;
    //Radius = 0.0f;

    //fx = abs(vecMax.x - vecMin.x);
    //fy = abs(vecMax.y - vecMin.y);
    //fz = abs(vecMax.z - vecMin.z);

    //Radius = ( fx > fy )? fx : fy ;
    //Radius = ( Radius > fz )? Radius : fz ;

    //Radius /= 2.0f;
}

/////////////////////////////////////////////////////////////////////
// TKRayCollision
/////////////////////////////////////////////////////////////////////
TKRayCollision::TKRayCollision(int ID)
:TKCollisionResource(ID)
,m_fLength(0)
,vecDir(0.0f, 0.0f, 0.0f)
,vecStart(0.0f, 0.0f, 0.0f)
,vecEnd(0.0f, 0.0f, 0.0f)
{
    SetObjectType( eotRayCollision );
    m_fLength = 0.0f;
    vecDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    vecStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

TKRayCollision::~TKRayCollision(void)
{
}

void TKRayCollision::SetTransform(D3DXMATRIX & matWorld, D3DXMATRIX * pmatArray)
{
    __super::SetTransform( matWorld, pmatArray );
}

TKRayCollision & TKRayCollision::operator = (TKRayCollision & CollisionResource)
{
    __super::operator=(CollisionResource);

    this->m_fLength = CollisionResource.m_fLength;

    return CollisionResource;
}

void TKRayCollision::Revert(TKCollisionResource * pSourceRes)
{
    __super::Revert(pSourceRes);
    this->m_fLength = ((TKRayCollision*)pSourceRes)->m_fLength;
}

void TKRayCollision::BuildMinMax(void)
{
    vecEnd = vecStart + vecDir * m_fLength;

    vecMin = vecStart;
    vecMax = vecEnd;

    if (vecMax.x < vecStart.x)    vecMax.x = vecStart.x;
    if (vecMax.y < vecStart.y)    vecMax.y = vecStart.y;
    if (vecMax.z < vecStart.z)    vecMax.z = vecStart.z;

    if (vecMin.x > vecEnd.x)    vecMin.x = vecEnd.x;
    if (vecMin.y > vecEnd.y)    vecMin.y = vecEnd.y;
    if (vecMin.z > vecEnd.z)    vecMin.z = vecEnd.z;

    vecCenter = (vecMax + vecMin) / 2.0f;

    D3DXVECTOR3 vecTemp;
    vecTemp = vecMax - vecMin;
    Radius = D3DXVec3Length(&vecTemp);
    Radius /= 2.0f;
    
    //float fx, fy, fz;
    //Radius = 0.0f;

    //fx = abs(vecMax.x - vecMin.x);
    //fy = abs(vecMax.y - vecMin.y);
    //fz = abs(vecMax.z - vecMin.z);

    //Radius = ( fx > fy )? fx : fy ;
    //Radius = ( Radius > fz )? Radius : fz ;

    //Radius /= 2.0f;
}

////////////////////////////////////////////////////////////////////////////////
// TKCollisionObjectRes
////////////////////////////////////////////////////////////////////////////////
TKCollisionObjectRes::TKCollisionObjectRes(int ID)
:TKCollisionResource(ID)
,pMeshRes(NULL)
,IsDynamicObject(false)
,IsMeshAnimation(false)
{
    SetObjectType( eotCollisionObjectRes );
}

TKCollisionObjectRes::~TKCollisionObjectRes(void)
{
}

bool TKCollisionObjectRes::CollisionAnimationBind(COLLISIONRESLIST & CollisionResList)
{
    TKMeshGeometry *pGeom = NULL;
    TKCollisionResource *pCollision = NULL;
    vector<ST_GEOM_VERTEX>::iterator it;
    
    bool Result = true;
    
    if ( IsDynamicObject )
    {
        if ( IsMeshAnimation )
        {
            if ( pMeshRes )
            {
                for ( int i = 0 ; i < (int)CollisionResList.size() ; ++i )
                {
                    pGeom = pMeshRes->GetGeomByNodeName( CollisionResList[i]->GetBoneNodeName() );
                    if (pGeom)
                    {
                        CollisionResList[i]->SetBoneMeshGeom( pGeom );
                    }
                    else
                    {
                        CollisionResList[i]->SetBoneMeshGeom( NULL );
                        CollisionResList[i]->SetBoneNodeName( string("") );
                        MessageBox( 0, "바인딩 실패!, 애니메이션 본을 찾지 못했음", "TKCollisionObjectRes::CollisionAnimationBind", MB_ICONERROR);
                        Result = false;
                    }
                    if ( !CollisionAnimationBind( CollisionResList[i]->GetCollisionResList() ) )
                    {
                        Result = false;
                    }
                }
            }
            else
            {
                MessageBox(0, "pMeshRes not defined", "TKCollisionObjectRes::CollisionAnimationBind", MB_ICONERROR);
                IsMeshAnimation = false;
                Result = false;
            }
        }
    }
    
    return Result;
}

void TKCollisionObjectRes::Init(void)
{
    // 충돌영역 정보 초기화
    __super::Init();

    // 메쉬 리소스와 충돌영역의 애니메이션을 바인드
    CollisionAnimationBind( CollisionResList );
}

void TKCollisionObjectRes::Release(void)
{
    // 충돌영역리스트 해제
    __super::Release();
}

bool TKCollisionObjectRes::ClearDelete(void)
{
    // 모든 충돌데이터를 삭제한다.
    for ( int i = 0 ; i < (int)CollisionResList.size() ; ++i )
    {
        delete CollisionResList[i];
    }
    CollisionResList.clear();
    return true;
}

void TKCollisionObjectRes::SetMeshRes(TKMeshRes *pMeshRes)
{
    this->pMeshRes = pMeshRes;
}

TKMeshRes * TKCollisionObjectRes::GetMeshRes(void)
{
    return pMeshRes;
}

bool TKCollisionObjectRes::GetIsDynamicObject(void)
{
    return IsDynamicObject;
}

void TKCollisionObjectRes::SetIsDynamicObject(bool IsDynamicObject)
{
    this->IsDynamicObject = IsDynamicObject;
}

bool TKCollisionObjectRes::GetIsMeshAnimation(void)
{
    return IsMeshAnimation;
}

void TKCollisionObjectRes::SetIsMeshAnimation(bool IsMeshAnimation)
{
    this->IsMeshAnimation = IsMeshAnimation;
}

void TKCollisionObjectRes::SetTransform(D3DXMATRIX & matWorld, D3DXMATRIX * pmatArray)
{
    __super::SetTransform( matWorld, pmatArray );
}

TKCollisionObjectRes & TKCollisionObjectRes::operator = (TKCollisionObjectRes & CollisionRes)
{
    __super::operator =(CollisionRes);

    this->IsDynamicObject = CollisionRes.IsDynamicObject;
    this->IsMeshAnimation = CollisionRes.IsMeshAnimation;
    this->pMeshRes = CollisionRes.pMeshRes;

    return CollisionRes;
}

void TKCollisionObjectRes::Revert(TKCollisionObjectRes * pSourceRes)
{
    __super::Revert( pSourceRes );

    this->IsDynamicObject   = ((TKCollisionObjectRes*)pSourceRes)->IsDynamicObject;
    this->IsMeshAnimation   = ((TKCollisionObjectRes*)pSourceRes)->IsMeshAnimation;
    this->pMeshRes          = ((TKCollisionObjectRes*)pSourceRes)->pMeshRes;
}

// 충돌 데이터 파일 로딩 함수
bool TKCollisionObjectRes::LoadFromFile( string & FileName, ECollisionObjectType CollisionObjectType)
{
#if 0
	TKCollisionFileConverter CollisionFileConverter;
    if (strstr(FileName.data(), ".KCF"))
    {
        return CollisionFileConverter.ConvertFromKCF(FileName, GetEngine(), this, CollisionObjectType );
    }
    else if (strstr(FileName.data(), ".KCT"))
    {
        return CollisionFileConverter.ConvertFromKCT( FileName, GetEngine(), this );
    }
	else
#endif
    {
        return false;
    }
}

// 충돌 데이터 파일 저장 함수
bool TKCollisionObjectRes::SaveToFile( string & FileName )
{
#if 0
	TKCollisionFileConverter CollisionFileConverter;
    return CollisionFileConverter.ConvertToKCT( FileName, GetEngine(), this );
#else
	return false;
#endif
}

// 충돌데이터 원본을 바탕으로 충돌객체를 생성
void TKCollisionObjectRes::CreateCollisionTreeByOrigin(ORIGINCOLLISIONLIST *pOriginCollisionList, 
                                                       ECollisionObjectType CollisionObjectType, 
                                                       ECollisionMeshSourceType CollisionMeshSourceType)
{
    TKSphereCollision *pSphereCollision0, *pSphereCollision1;
    ORIGINCOLLISIONLIST::iterator it;
    
    // 기존의 충돌데이터를 삭제
    for ( int i = 0 ; i < (int)CollisionResList.size() ; ++i )
    {
        delete CollisionResList[i];
    }
    CollisionResList.clear();
    
    // 충돌데이터를 새로 만든다.
    switch(CollisionObjectType)
    {
        case ecotNULL:
            break;
        case ecotFace:
            CreateOriginByMeshRes( eotFaceCollision, pOriginCollisionList, CollisionMeshSourceType );
            for ( it = pOriginCollisionList->begin() ; it != pOriginCollisionList->end() ; ++it )
            {
                CreateCollisionByOrigin<TKFaceCollision>( CollisionResList, (*it) );
            }
            break;
        case ecotOBB:
            // CreateOriginByMeshRes( eotOBBCollision, pOriginCollisionList, CollisionMeshSourceType );
            // OBB는 메쉬정보만으로 충돌영역을 만들 수 없고, 반드시 충돌영역 기본 데이터가 있어야 함.
            // Min, Max를 바탕으로 OBB가 아닌 AABB를 생성할 수는 있으나 이는 차후에 보완되어야 할 듯.
            for ( it = pOriginCollisionList->begin() ; it != pOriginCollisionList->end() ; ++it )
            {
                CreateCollisionByOrigin<TKOBBCollision>( CollisionResList, (*it) );
            }
            break;
        case ecotSphere:
            CreateOriginByMeshRes( eotSphereCollision, pOriginCollisionList, CollisionMeshSourceType );
            for ( it = pOriginCollisionList->begin() ; it != pOriginCollisionList->end() ; ++it )
            {
                CreateCollisionByOrigin<TKSphereCollision>( CollisionResList, (*it) );
            }
            break;
        case ecotRay:
            CreateOriginByMeshRes( eotRayCollision, pOriginCollisionList, CollisionMeshSourceType );
            for ( it = pOriginCollisionList->begin() ; it != pOriginCollisionList->end() ; ++it )
            {
                CreateCollisionByOrigin<TKRayCollision>( CollisionResList, (*it) );
            }
            break;
        case ecotSphereFace:
            pSphereCollision0 = CreateSphereByParam( CollisionResList, string("ecotSphereFace"), this->GetBoneNodeName() );
            for ( it = pOriginCollisionList->begin() ; it != pOriginCollisionList->end() ; ++it )
            {
                CreateCollisionByOrigin<TKFaceCollision>( pSphereCollision0->GetCollisionResList(), (*it) );
            }
            break;
        case ecotSphereOBB:
            pSphereCollision0 = CreateSphereByParam( CollisionResList, string("ecotSphereOBB"), this->GetBoneNodeName() );
            for ( it = pOriginCollisionList->begin() ; it != pOriginCollisionList->end() ; ++it )
            {
                CreateCollisionByOrigin<TKOBBCollision>( pSphereCollision0->GetCollisionResList(), (*it) );
            }
            break;
        case ecotSphereSphere:
            pSphereCollision0 = CreateSphereByParam( CollisionResList, string("ecotSphereSphere"), this->GetBoneNodeName() );
            for ( it = pOriginCollisionList->begin() ; it != pOriginCollisionList->end() ; ++it )
            {
                CreateCollisionByOrigin<TKSphereCollision>( pSphereCollision0->GetCollisionResList(), (*it) );
            }
            break;
        case ecotSphereRay:
            pSphereCollision0 = CreateSphereByParam( CollisionResList, string("ecotSphereRay"), this->GetBoneNodeName() );
            for ( it = pOriginCollisionList->begin() ; it != pOriginCollisionList->end() ; ++it )
            {
                CreateCollisionByOrigin<TKRayCollision>( pSphereCollision0->GetCollisionResList(), (*it) );
            }
            break;
        case ecotSphereSphereFace:
            pSphereCollision0 = CreateSphereByParam( CollisionResList, string("ecotSphereSphereFace"), this->GetBoneNodeName() );
            for ( it = pOriginCollisionList->begin() ; it != pOriginCollisionList->end() ; ++it )
            {
                pSphereCollision1 = CreateSphereByParam( pSphereCollision0->GetCollisionResList(), (*it)->GetNodeName(), (*it)->GetBoneNodeName() );
                CreateCollisionByOrigin<TKFaceCollision>( pSphereCollision1->GetCollisionResList(), (*it) );
            }
            break;
        case ecotSphereSphereOBB:
            pSphereCollision0 = CreateSphereByParam( CollisionResList, string("ecotSphereSphereOBB"), this->GetBoneNodeName() );
            for ( it = pOriginCollisionList->begin() ; it != pOriginCollisionList->end() ; ++it )
            {
                pSphereCollision1 = CreateSphereByParam( pSphereCollision0->GetCollisionResList(), (*it)->GetNodeName(), (*it)->GetBoneNodeName() );
                CreateCollisionByOrigin<TKOBBCollision>( pSphereCollision1->GetCollisionResList(), (*it) );
            }
            break;
        case ecotSphereSphereSphere:
            pSphereCollision0 = CreateSphereByParam( CollisionResList, string("ecotSphereSphereSphere"), this->GetBoneNodeName() );
            for ( it = pOriginCollisionList->begin() ; it != pOriginCollisionList->end() ; ++it )
            {
                pSphereCollision1 = CreateSphereByParam( pSphereCollision0->GetCollisionResList(), (*it)->GetNodeName(), (*it)->GetBoneNodeName() );
                CreateCollisionByOrigin<TKSphereCollision>( pSphereCollision1->GetCollisionResList(), (*it) );
            }
            break;
        case ecotSphereSphereRay:
            pSphereCollision0 = CreateSphereByParam( CollisionResList, string("ecotSphereSphereRay"), this->GetBoneNodeName() );
            for ( it = pOriginCollisionList->begin() ; it != pOriginCollisionList->end() ; ++it )
            {
                pSphereCollision1 = CreateSphereByParam( pSphereCollision0->GetCollisionResList(), (*it)->GetNodeName(), (*it)->GetBoneNodeName() );
                CreateCollisionByOrigin<TKRayCollision>( pSphereCollision1->GetCollisionResList(), (*it) );
            }
            break;
    }
}

void TKCollisionObjectRes::CreateOriginByMeshRes(   ETKObjectType ObjectType, 
                                                    ORIGINCOLLISIONLIST *pOriginCollisionList, 
                                                    ECollisionMeshSourceType CollisionMeshSourceType)
{
    // 충돌데이터를 만들때 원본충돌데이터가 없을경우, 메쉬리소스를 바탕으로 원본충돌데이터를 만들어 준다.
    if ( pOriginCollisionList->size() <= 0 )
    {
        ST_ORIGINCOLLISIONDATA *pOriginCollision;
        pOriginCollision = new ST_ORIGINCOLLISIONDATA;
        pOriginCollision->ObjectType = ObjectType;
        pOriginCollision->SetNodeName( string("ROOTNODE") );
        pOriginCollision->SetBoneNodeName( this->GetBoneNodeName() );
        // 메쉬 리소스가 있으면, 메쉬리소스를 가져다가 쓰고.
        // 메쉬 리소스마저 없으면 충돌영역은 텅 빈 깡통이다.
        pOriginCollision->ExtractFromMeshRes( pMeshRes, CollisionMeshSourceType );
        pOriginCollisionList->push_back( pOriginCollision );
    }
}

TKSphereCollision * TKCollisionObjectRes::CreateSphereByParam
( COLLISIONRESLIST & ParentCollisionList, string & NodeName, string & BoneNodeName )
{
#if 0
	TKSphereCollision * pSphereCollision = new TKSphereCollision(GetEngine(), GENID );
    ParentCollisionList.push_back( pSphereCollision );
    pSphereCollision->SetNodeName( NodeName );
    pSphereCollision->SetBoneNodeName( BoneNodeName );
    
    return pSphereCollision;
#else
	return nullptr;
#endif
}

TKSphereCollision * TKCollisionObjectRes::CreateSphereByMeshGeom(TKMeshGeometry *pMeshGeom)
{
#if 0
	string              NodeName;
    string              BoneNodeName;
    ST_IB_TRIANGLE      St_IB_Triangle;
    TKSphereCollision * pCollision = NULL;
    
    float       fLen = (D3DXVec3Length(&pMeshGeom->GetMax()) - 
                        D3DXVec3Length(&pMeshGeom->GetMin())) / 2.0f;
    D3DXVECTOR3 vecCenter(  pMeshGeom->GetMin().x + fLen, 
                            pMeshGeom->GetMin().y + fLen, 
                            pMeshGeom->GetMin().z + fLen );
    
    NodeName = "COLL_" + pMeshGeom->GetNodeName();
    BoneNodeName = pMeshGeom->GetNodeName();
    
    pCollision = new TKSphereCollision( GetEngine(), GENID );
    this->GetCollisionResList().push_back( pCollision );
    pCollision->SetNodeName( NodeName );
    pCollision->SetBoneNodeName( BoneNodeName );   
    pCollision->SetBoneMeshGeom( pMeshGeom );
    
    // 버텍스 정보를 정의
    // 버텍스 노말은 사용하지 않지만 데이터 포맷을 맞춰주기 위해서 임의의 값을 삽입한다.
    pCollision->GetPosList().push_back( pMeshGeom->GetMin() );
    pCollision->GetNormalList().push_back( pMeshGeom->GetMin() );
    pCollision->GetPosList().push_back( vecCenter );
    pCollision->GetNormalList().push_back( vecCenter );
    pCollision->GetPosList().push_back( pMeshGeom->GetMax() );
    pCollision->GetNormalList().push_back( pMeshGeom->GetMax() );

    // 페이스 인덱스를 정의
    St_IB_Triangle._0 = 0;  St_IB_Triangle._1 = 1;  St_IB_Triangle._2 = 2;
    pCollision->GetFaceList().push_back( St_IB_Triangle );

    pCollision->Init();    

    return pCollision;
#else
	return nullptr;
#endif
}

// 지오메트리의 OBB 정보를 읽어서 OBB 충돌을 만든다.
TKOBBCollision * TKCollisionObjectRes::CreateOBBByMeshGeom(TKMeshGeometry *pMeshGeom)
{
#if 0
	GEOMLIST    GeomList;
    D3DXVECTOR3 vecCenter;
    D3DXVECTOR3 vecAxis[3];
    float       AxisLen[3];
    D3DXVECTOR3 vecOBB[8];
    string      NodeName;
    string      BoneNodeName;
    ST_IB_TRIANGLE  St_IB_Triangle;
    TKOBBCollision *pCollision = NULL;
    
    if ( BuildOBB( pMeshGeom->GetPosList(), vecCenter, vecAxis, AxisLen, vecOBB ) )
    {
        NodeName = "COLL_" + pMeshGeom->GetNodeName();
        BoneNodeName = pMeshGeom->GetNodeName();
        
        pCollision = new TKOBBCollision( GetEngine(), GENID );
        this->GetCollisionResList().push_back( pCollision );
        pCollision->SetNodeName( NodeName );
        pCollision->SetBoneNodeName( BoneNodeName );   
        pCollision->SetBoneMeshGeom( pMeshGeom );
        
        // 버텍스 정보를 복사
        pCollision->GetPosList()    = pMeshGeom->GetPosList();
        pCollision->GetNormalList() = pMeshGeom->GetNormalList();
        pCollision->GetFaceList()   = pMeshGeom->GetFaceList();
        
        pCollision->SetAxisDir( vecAxis );
        pCollision->SetAxisLen( AxisLen );
        pCollision->SetOBB( vecOBB );

        pCollision->Init();    
    }
    GeomList.clear();

    return pCollision;
#else
	return nullptr;
#endif
}

////////////////////////////////////////////////////////////////////////////////
// TKCollisionRenderRes
////////////////////////////////////////////////////////////////////////////////
TKCollisionRenderRes::TKCollisionRenderRes(int ID)
	:TKRenderResource(ID)
#ifdef TK_D3D9
	,pCircleVB(NULL)
	,pCircleIB(NULL)
#endif
{
    SetObjectType( eotCollisionRenderRes );
}

TKCollisionRenderRes::~TKCollisionRenderRes(void)
{
}

void TKCollisionRenderRes::Init(void)
{
    __super::Init();
#ifdef TK_D3D9    
    int iVertexBufferSize;
    int iIndexBufferSize;
    float RADIAN = 0.017453292619943f;
    
    if (!pVB)
    {
        iVertexBufferSize = sizeof(ST_VB_POS) * COLLISION_MAXVERTEXCOUNT;
        if (FAILED(R_D3DDEVICE->CreateVertexBuffer( iVertexBufferSize,
                                                    D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
                                                    ST_VB_POS::FVF,
                                                    D3DPOOL_DEFAULT,
                                                    &pVB,
                                                    NULL)))
        {
            MessageBox(0, "CreateVertexBuffer fault", "TKCollisionRenderRes::Init", MB_ICONERROR);
            _ASSERT(0);
        }
    }

    if (!pIB)
    {
        iIndexBufferSize = sizeof(ST_IB_TRIANGLE) * COLLISION_MAXINDEXCOUNT;
        if (FAILED(R_D3DDEVICE->CreateIndexBuffer(  iIndexBufferSize,
                                                    D3DUSAGE_DYNAMIC |D3DUSAGE_WRITEONLY,
                                                    D3DFMT_INDEX16,
                                                    D3DPOOL_DEFAULT,
                                                    &pIB,
                                                    NULL)))
        {
            MessageBox(0, "CreateIndexBuffer fault", "TKCollisionRenderRes::Init", MB_ICONERROR);
            _ASSERT(0);
        }
    }
    
    if (!pCircleVB)
    {
        //360/6
        iVertexBufferSize = sizeof(ST_VB_POS) * 60;
        if (FAILED(R_D3DDEVICE->CreateVertexBuffer( iVertexBufferSize,
                                                    D3DUSAGE_WRITEONLY,
                                                    ST_VB_POS::FVF,
                                                    D3DPOOL_DEFAULT,
                                                    &pCircleVB,
                                                    NULL)))
        {
            MessageBox(0, "CreateVertexBuffer 2 fault", "TKCollisionRenderRes::Init", MB_ICONERROR);
            _ASSERT(0);
        }
        
        ST_VB_POS *pVertex;
        pCircleVB->Lock(0, iVertexBufferSize, (void**)&pVertex, 0);
        for ( int i = 0 ; i < 360 ; i+=6 )
        {
            pVertex->Pos.x = cos((float)i * (float)RADIAN);
            pVertex->Pos.y = sin((float)i * (float)RADIAN);
            pVertex->Pos.z = 0.0f;
            ++pVertex;
        }
        pCircleVB->Unlock();
    }
    
    if (!pCircleIB)
    {
        iIndexBufferSize = sizeof(ST_IB_LINE) * 60;
        if (FAILED(R_D3DDEVICE->CreateIndexBuffer(  iIndexBufferSize,
                                                    D3DUSAGE_WRITEONLY,
                                                    D3DFMT_INDEX16,
                                                    D3DPOOL_DEFAULT,
                                                    &pCircleIB,
                                                    NULL)))
        {
            MessageBox(0, "CreateIndexBuffer 2 fault", "TKCollisionRenderRes::Init", MB_ICONERROR);
            _ASSERT(0);
        }
        ST_IB_LINE *pIndex;
        pCircleIB->Lock(0, iIndexBufferSize, (void**)&pIndex, 0);        
        for ( int i = 0 ; i < 60 ; ++i )
        {
            pIndex->_0 = i;
            if ( i == 59 )
                pIndex->_1 = 0;
            else
                pIndex->_1 = i+1;
            ++pIndex;
        }
        pCircleIB->Unlock();        
    }
#endif
}

void TKCollisionRenderRes::Release(void)
{
#ifdef TK_D3D9
    SAFERELEASE(pCircleIB);
    SAFERELEASE(pCircleVB);    
#endif

    __super::Release();
}
