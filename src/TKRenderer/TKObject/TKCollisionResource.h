#pragma once

#include "TKEngineResource.h"
#include "TKShaderResource.h"
#include "TKVertexResource.h"

/////////////////////////////////////////////////////////////////////
// ST_ORIGINCOLLISIONDATA
/////////////////////////////////////////////////////////////////////
struct ST_ORIGINCOLLISIONDATA
{
private:
    string NodeName;
    string BoneNodeName;
    
    void ExtractFromGEOMLIST(GEOMLIST & GeomList);
    
public:
    ETKObjectType           ObjectType;
    D3DXVECTOR3S            PosList;
    D3DXVECTOR3S            NormalList;
    vector<ST_IB_TRIANGLE>  FaceList;
    D3DXVECTOR3             vecCenter;      //OBB의 중심점
    float                   AxisLen[3];     //OBB의 축길이 3개 (축방향과 일대일 대응)
    D3DXVECTOR3             vecAxisDir[3];  //OBB의 축방향 3개 (단위벡터)
    D3DXVECTOR3             vecOBB[8];      //OBB의 8점    

    ST_ORIGINCOLLISIONDATA(void);
    ST_ORIGINCOLLISIONDATA(ST_ORIGINCOLLISIONDATA & st_OriginCollisionData);
    ~ST_ORIGINCOLLISIONDATA(void);

    const ST_ORIGINCOLLISIONDATA & operator = (const ST_ORIGINCOLLISIONDATA & st_ORIGINCOLLISIONDATA);

    // 메쉬리소스에서 충돌페이스를 추출해온다.
    void ExtractFromMeshRes (TKMeshRes * pMeshRes, ECollisionMeshSourceType CollisionMeshSourceType);

    string & GetNodeName(void)                      { return NodeName; }
    void SetNodeName(string & NodeName)             { this->NodeName = NodeName; }

    string & GetBoneNodeName(void)                  { return BoneNodeName; }
    void SetBoneNodeName(string & BoneNodeName)     { this->BoneNodeName = BoneNodeName; }
};

/////////////////////////////////////////////////////////////////////
// TKCollisionResource
/////////////////////////////////////////////////////////////////////
class TKCollisionResource : public TKEngineResource
{
    friend class TKCollisionChecker;
    friend class TKCollisionObjectRes;
private:
    string                  BoneNodeName;   // 바인딩될 TKMeshGeometry 객체의 노드네임
    bool                    IsSelected;     // (툴에서) 현재 선택된 지오메트리

protected:
    TKMeshGeometry*         pBoneMeshGeom;  // 바인딩될 애니메이션을 가지고 있는 지오메트리 객체
    COLLISIONRESLIST        CollisionResList;

    D3DXVECTOR3S            PosList;
    D3DXVECTOR3S            NormalList;
    vector<ST_IB_TRIANGLE>  FaceList;
    D3DXVECTOR3S            FaceNormalList;

    D3DXVECTOR3             vecCenter;
    D3DXVECTOR3             vecMin;
    D3DXVECTOR3             vecMax;
    float                   Radius;

    void BuildFaceNormal(void);
    
    D3DXMATRIX          matAni;         // 매트릭스 계산용
public:
    TKCollisionResource(int ID);
    virtual ~TKCollisionResource(void);

    virtual void Init(void);
    virtual void Release(void);

    D3DXVECTOR3S & GetPosList()                     { return PosList; }
    D3DXVECTOR3S & GetNormalList()                  { return NormalList; }
    vector<ST_IB_TRIANGLE> & GetFaceList()          { return FaceList; }
    D3DXVECTOR3S & GetFaceNormalList()              { return FaceNormalList; }

    virtual void BuildMinMax(void);
    virtual void BuildMinMaxByChild(COLLISIONRESLIST & CollisionResList);

    const D3DXVECTOR3 & GetMin(void)                { return vecMin; }
    void SetMin(const D3DXVECTOR3 & vecMin)         { this->vecMin = vecMin; }

    const D3DXVECTOR3 & GetMax(void)                { return vecMax; }
    void SetMax(const D3DXVECTOR3 & vecMax)         { this->vecMax = vecMax; }

    const D3DXVECTOR3 & GetCenter(void)             { return vecCenter; }
    void SetCenter(const D3DXVECTOR3 & vecCenter)   { this->vecCenter = vecCenter; }
    
    const float GetRadius(void)                     { return Radius; }
    void SetRadius(const float Radius)              { this->Radius = Radius; }

    COLLISIONRESLIST & GetCollisionResList(void)    { return CollisionResList; }

    virtual void SetTransform(D3DXMATRIX & matWorld, D3DXMATRIX * pmatArray);
    virtual TKCollisionResource & operator = (TKCollisionResource & CollisionResource);
    virtual void Revert(TKCollisionResource * pSourceRes);
    
    TKCollisionResource * GetParentCollisionResByID( int ID );
    TKCollisionResource * GetParentCollisionResByNodeName( string & NodeName );
    TKCollisionResource * GetCollisionResByNodeName( string & NodeName );
    TKCollisionResource * GetCollisionResByBoneNodeName( string & BoneNodeName );
    TKCollisionResource * GetCollisionResByMeshGeometry( TKMeshGeometry * pMeshGeometry );

    void DeleteCollisionResByID( int ID );
    void DeleteCollisionResByNodeName( string & NodeName );
    void DeleteCollisionResByBoneNodeName( string & BoneNodeName );
    void DeleteCollisionResByMeshGeometry( TKMeshGeometry * pMeshGeometry );
    void DeleteCollisionResByCollisionRes( TKCollisionResource * pCollisionRes );

    void SetBoneMeshGeom(TKMeshGeometry *pBoneMeshGeom) { this->pBoneMeshGeom = pBoneMeshGeom; }
    TKMeshGeometry * GetBoneMeshGeom(void)              { return this->pBoneMeshGeom; }

    void SetBoneNodeName(string & BoneNodeName)         { this->BoneNodeName = BoneNodeName; }
    string & GetBoneNodeName(void)                      { return BoneNodeName; }
    
    bool GetSelected(void)                              { return IsSelected; }
    void SetSelected(bool IsSelected);
};

/////////////////////////////////////////////////////////////////////
// TKFaceCollision
/////////////////////////////////////////////////////////////////////
class TKFaceCollision : public TKCollisionResource
{
    friend class TKCollisionChecker;

public:
    TKFaceCollision(int ID);
    virtual ~TKFaceCollision(void);

    virtual void SetTransform(D3DXMATRIX & matWorld, D3DXMATRIX * pmatArray);
    virtual TKFaceCollision & operator = (TKFaceCollision & CollisionResource);
    virtual void Revert(TKCollisionResource * pSourceRes);
};

/////////////////////////////////////////////////////////////////////
// TKSphereCollision
/////////////////////////////////////////////////////////////////////
class TKSphereCollision : public TKCollisionResource
{
    friend class TKCollisionChecker;
protected:

public:
    TKSphereCollision(int ID);
    virtual ~TKSphereCollision(void);
    
    void Init(void);
};

/////////////////////////////////////////////////////////////////////
// TKOBBCollision
/////////////////////////////////////////////////////////////////////
class TKOBBCollision : public TKCollisionResource
{
    friend class TKCollisionChecker;
protected:
    float       AxisLen[3];     //OBB의 축길이 3개 (축방향과 일대일 대응)
    D3DXVECTOR3 vecAxisDir[3];  //OBB의 축방향 3개 (단위벡터)
    D3DXVECTOR3 vecOBB[8];      //OBB의 8점

    bool CreateOBB(void);

public:
    TKOBBCollision(int ID);
    virtual ~TKOBBCollision(void);

    void Init(void);

    virtual void SetTransform(D3DXMATRIX & matWorld, D3DXMATRIX * pmatArray);
    virtual TKOBBCollision & operator = (TKOBBCollision & CollisionResource);
    virtual void Revert(TKCollisionResource * pSourceRes);

    const float * GetAxisLen(void)                  { return AxisLen; }
    void SetAxisLen(const float AxisLen[3])         { memcpy(this->AxisLen, AxisLen, sizeof(this->AxisLen) ); }
    
    const D3DXVECTOR3 * GetAxisDir(void)            { return vecAxisDir; }
    void SetAxisDir(const D3DXVECTOR3 AxisDir[3])   { memcpy(this->vecAxisDir, AxisDir, sizeof(this->vecAxisDir) ); }

    const D3DXVECTOR3 * GetOBB(void)                { return vecOBB; }
    void SetOBB(const D3DXVECTOR3 vecOBB[8])        { memcpy(this->vecOBB, vecOBB, sizeof(this->vecOBB) ); }
    
    void BuildMinMax(void);
};

/////////////////////////////////////////////////////////////////////
// TKRayCollision
/////////////////////////////////////////////////////////////////////
// Ray의 경우에는 사용되는 클래스에서 리얼타임으로 데이터를 변경시켜주어야 함
class TKRayCollision : public TKCollisionResource
{
    friend class TKCollisionChecker;
protected:
	float		m_fLength;		//Ray의 길이
    D3DXVECTOR3 vecDir;
    D3DXVECTOR3 vecStart;
    D3DXVECTOR3 vecEnd;
public:
    TKRayCollision(int ID);
    virtual ~TKRayCollision(void);

    virtual void SetTransform(D3DXMATRIX & matWorld, D3DXMATRIX * pmatArray);
    virtual TKRayCollision & operator = (TKRayCollision & CollisionResource);
    virtual void Revert(TKCollisionResource * pSourceRes);
    
    float GetLength(void)                   { return m_fLength; }
    void SetLength(float fLength)           { this->m_fLength = fLength; }

    D3DXVECTOR3 & GetDir(void)              { return vecDir; }
    void SetDir(D3DXVECTOR3 & vecDir)       { this->vecDir = vecDir; }

    D3DXVECTOR3 & GetStart(void)            { return vecStart; }
    void SetStart(D3DXVECTOR3 & vecStart)   { this->vecStart = vecStart; }

    void BuildMinMax(void);
};

////////////////////////////////////////////////////////////////////////////////
// TKCollisionObjectRes
////////////////////////////////////////////////////////////////////////////////
class TKCollisionObjectRes : public TKCollisionResource
{
protected:    
    bool            IsDynamicObject;    // 동적 데이터여부
    bool            IsMeshAnimation;    // 메쉬의 프레임당 월드매트릭스/애니메이션매트릭스 와의 계산 여부를 결정
    TKMeshRes       *pMeshRes;           // (IsMeshAnimation == true) 일 경우 애니메이션을 바인딩 할 메쉬리소스 객체
    
    // 충돌객체와 애니메이션을 바인드
    bool CollisionAnimationBind(COLLISIONRESLIST & CollisionResList);
    
    // 충돌객체 생성함수(오리진데이터에서 충돌객체를 생성)
    template <class COLLISIONCLASS> COLLISIONCLASS * CreateCollisionByOrigin
    ( COLLISIONRESLIST & ParentCollisionList, ST_ORIGINCOLLISIONDATA * pSrcCollision );    

public:
    TKCollisionObjectRes(int ID);
    virtual ~TKCollisionObjectRes(void);

    void Init(void);
    void Release(void);

    bool ClearDelete(void);
    
    void SetMeshRes(TKMeshRes *pMeshRes);   // 메쉬리소스를 바인딩
    TKMeshRes * GetMeshRes(void);

    bool GetIsDynamicObject(void);
    void SetIsDynamicObject(bool IsDynamicObject);

    bool GetIsMeshAnimation(void);
    void SetIsMeshAnimation(bool IsMeshAnimation);
    
    void SetTransform(D3DXMATRIX & matWorld, D3DXMATRIX * pmatArray);
    TKCollisionObjectRes & operator = (TKCollisionObjectRes & CollisionRes);
    
    void Revert(TKCollisionObjectRes * pSourceRes);
    
    // 충돌 데이터 파일 로딩 함수
    bool LoadFromFile(  string & FileName, ECollisionObjectType CollisionObjectType = ecotNULL );
    
    // 충돌 데이터 파일 저장 함수
    bool SaveToFile( string & FileName );
    
    // 충돌데이터 원본을 바탕으로 충돌객체를 생성
    void CreateCollisionTreeByOrigin( ORIGINCOLLISIONLIST *pOriginCollisionList, 
                                      ECollisionObjectType CollisionObjectType,
                                      ECollisionMeshSourceType CollisionMeshSourceType = ecmstNULL);
    // 메쉬리소스 정보를 바탕으로 충돌영역을 만든다.
    void CreateOriginByMeshRes( ETKObjectType ObjectType, 
                                ORIGINCOLLISIONLIST *pOriginCollisionList, 
                                ECollisionMeshSourceType CollisionMeshSourceType);
    // 충돌객체 생성함수(구 충돌객체를 만든다)
    TKSphereCollision * CreateSphereByParam
    ( COLLISIONRESLIST & ParentCollisionList, string & NodeName, string & BoneNodeName );
    
    // 지오메트리 정보를 읽어서 구충돌을 만든다.
    TKSphereCollision * CreateSphereByMeshGeom(TKMeshGeometry *pMeshGeom);

    // 지오메트리의 OBB 정보를 읽어서 OBB 충돌을 만든다.
    TKOBBCollision * CreateOBBByMeshGeom(TKMeshGeometry *pMeshGeom);
};

template <class COLLISIONCLASS> COLLISIONCLASS * TKCollisionObjectRes::CreateCollisionByOrigin( COLLISIONRESLIST & ParentCollisionList,
                                                                                                ST_ORIGINCOLLISIONDATA * pSrcCollision )
{   // 충돌객체 생성코드
    COLLISIONCLASS *pCollision;
    pCollision = new COLLISIONCLASS( GENID );
    ParentCollisionList.push_back ( pCollision );
    pCollision->SetNodeName( pSrcCollision->GetNodeName() );
    pCollision->SetBoneNodeName(pSrcCollision->GetBoneNodeName());

    pCollision->PosList     = pSrcCollision->PosList;
    pCollision->NormalList  = pSrcCollision->NormalList;
    pCollision->FaceList    = pSrcCollision->FaceList;
    
    if ( dynamic_cast<TKOBBCollision*>(pCollision) )
    {
        ((TKOBBCollision *)pCollision)->SetCenter( pSrcCollision->vecCenter );
        ((TKOBBCollision *)pCollision)->SetAxisLen( pSrcCollision->AxisLen );
        ((TKOBBCollision *)pCollision)->SetAxisDir( pSrcCollision->vecAxisDir );
        ((TKOBBCollision *)pCollision)->SetOBB( pSrcCollision->vecOBB );
    }
    
    return pCollision;
}

////////////////////////////////////////////////////////////////////////////////
// TKCollisionRenderRes
////////////////////////////////////////////////////////////////////////////////
#define COLLISION_MAXINDEXCOUNT 10000
#define COLLISION_MAXVERTEXCOUNT 30000

class TKCollisionRenderRes : public TKRenderResource
{
public:
    TKCollisionRenderRes(int ID);
    virtual ~TKCollisionRenderRes(void);

#ifdef TK_D3D9
    LPDIRECT3DVERTEXBUFFER9 pCircleVB;
    LPDIRECT3DINDEXBUFFER9  pCircleIB;
#endif

    virtual void Init(void);
    virtual void Release(void);
};