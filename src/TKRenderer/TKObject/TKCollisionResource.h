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
    D3DXVECTOR3             vecCenter;      //OBB�� �߽���
    float                   AxisLen[3];     //OBB�� ����� 3�� (������ �ϴ��� ����)
    D3DXVECTOR3             vecAxisDir[3];  //OBB�� ����� 3�� (��������)
    D3DXVECTOR3             vecOBB[8];      //OBB�� 8��    

    ST_ORIGINCOLLISIONDATA(void);
    ST_ORIGINCOLLISIONDATA(ST_ORIGINCOLLISIONDATA & st_OriginCollisionData);
    ~ST_ORIGINCOLLISIONDATA(void);

    const ST_ORIGINCOLLISIONDATA & operator = (const ST_ORIGINCOLLISIONDATA & st_ORIGINCOLLISIONDATA);

    // �޽����ҽ����� �浹���̽��� �����ؿ´�.
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
    string                  BoneNodeName;   // ���ε��� TKMeshGeometry ��ü�� ������
    bool                    IsSelected;     // (������) ���� ���õ� ������Ʈ��

protected:
    TKMeshGeometry*         pBoneMeshGeom;  // ���ε��� �ִϸ��̼��� ������ �ִ� ������Ʈ�� ��ü
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
    
    D3DXMATRIX          matAni;         // ��Ʈ���� ����
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
    float       AxisLen[3];     //OBB�� ����� 3�� (������ �ϴ��� ����)
    D3DXVECTOR3 vecAxisDir[3];  //OBB�� ����� 3�� (��������)
    D3DXVECTOR3 vecOBB[8];      //OBB�� 8��

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
// Ray�� ��쿡�� ���Ǵ� Ŭ�������� ����Ÿ������ �����͸� ��������־�� ��
class TKRayCollision : public TKCollisionResource
{
    friend class TKCollisionChecker;
protected:
	float		m_fLength;		//Ray�� ����
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
    bool            IsDynamicObject;    // ���� �����Ϳ���
    bool            IsMeshAnimation;    // �޽��� �����Ӵ� �����Ʈ����/�ִϸ��̼Ǹ�Ʈ���� ���� ��� ���θ� ����
    TKMeshRes       *pMeshRes;           // (IsMeshAnimation == true) �� ��� �ִϸ��̼��� ���ε� �� �޽����ҽ� ��ü
    
    // �浹��ü�� �ִϸ��̼��� ���ε�
    bool CollisionAnimationBind(COLLISIONRESLIST & CollisionResList);
    
    // �浹��ü �����Լ�(�����������Ϳ��� �浹��ü�� ����)
    template <class COLLISIONCLASS> COLLISIONCLASS * CreateCollisionByOrigin
    ( COLLISIONRESLIST & ParentCollisionList, ST_ORIGINCOLLISIONDATA * pSrcCollision );    

public:
    TKCollisionObjectRes(int ID);
    virtual ~TKCollisionObjectRes(void);

    void Init(void);
    void Release(void);

    bool ClearDelete(void);
    
    void SetMeshRes(TKMeshRes *pMeshRes);   // �޽����ҽ��� ���ε�
    TKMeshRes * GetMeshRes(void);

    bool GetIsDynamicObject(void);
    void SetIsDynamicObject(bool IsDynamicObject);

    bool GetIsMeshAnimation(void);
    void SetIsMeshAnimation(bool IsMeshAnimation);
    
    void SetTransform(D3DXMATRIX & matWorld, D3DXMATRIX * pmatArray);
    TKCollisionObjectRes & operator = (TKCollisionObjectRes & CollisionRes);
    
    void Revert(TKCollisionObjectRes * pSourceRes);
    
    // �浹 ������ ���� �ε� �Լ�
    bool LoadFromFile(  string & FileName, ECollisionObjectType CollisionObjectType = ecotNULL );
    
    // �浹 ������ ���� ���� �Լ�
    bool SaveToFile( string & FileName );
    
    // �浹������ ������ �������� �浹��ü�� ����
    void CreateCollisionTreeByOrigin( ORIGINCOLLISIONLIST *pOriginCollisionList, 
                                      ECollisionObjectType CollisionObjectType,
                                      ECollisionMeshSourceType CollisionMeshSourceType = ecmstNULL);
    // �޽����ҽ� ������ �������� �浹������ �����.
    void CreateOriginByMeshRes( ETKObjectType ObjectType, 
                                ORIGINCOLLISIONLIST *pOriginCollisionList, 
                                ECollisionMeshSourceType CollisionMeshSourceType);
    // �浹��ü �����Լ�(�� �浹��ü�� �����)
    TKSphereCollision * CreateSphereByParam
    ( COLLISIONRESLIST & ParentCollisionList, string & NodeName, string & BoneNodeName );
    
    // ������Ʈ�� ������ �о ���浹�� �����.
    TKSphereCollision * CreateSphereByMeshGeom(TKMeshGeometry *pMeshGeom);

    // ������Ʈ���� OBB ������ �о OBB �浹�� �����.
    TKOBBCollision * CreateOBBByMeshGeom(TKMeshGeometry *pMeshGeom);
};

template <class COLLISIONCLASS> COLLISIONCLASS * TKCollisionObjectRes::CreateCollisionByOrigin( COLLISIONRESLIST & ParentCollisionList,
                                                                                                ST_ORIGINCOLLISIONDATA * pSrcCollision )
{   // �浹��ü �����ڵ�
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