#pragma once

#include "TKEngineResource.h"
#include "TKShaderResource.h"
#include "TKVertexResource.h"
#include "TKTexture.h"

///////////////////////////////////////////////////////////////////////////////
struct ST_MESHVERTEXBUFFER
{
#ifdef TK_D3D9
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;
    LPDIRECT3DVERTEXBUFFER9 pMatrixIndex;
    LPDIRECT3DVERTEXBUFFER9 pMatrixWeight;
    LPDIRECT3DVERTEXBUFFER9 pTexelBuffer;
    LPDIRECT3DVERTEXBUFFER9 pColorBuffer;
    LPDIRECT3DVERTEXBUFFER9 pNormalBuffer;
#ifdef DEBUG_MODE
    LPDIRECT3DVERTEXBUFFER9 pOBBVB;
    LPDIRECT3DVERTEXBUFFER9 pOBBVB_Selected;
    LPDIRECT3DINDEXBUFFER9  pOBBIB;
#endif DEBUG_MODE
#endif

    ST_MESHVERTEXBUFFER(void){  Clear(); }
    virtual ~ST_MESHVERTEXBUFFER(void)
    {
        Release();
    }
    void Clear(void)
    {
#ifdef TK_D3D9
		pVertexBuffer = NULL;
        pMatrixIndex = NULL;
        pMatrixWeight = NULL;
        pTexelBuffer = NULL;
        pColorBuffer = NULL;
        pNormalBuffer = NULL;
#ifdef DEBUG_MODE
        pOBBVB = NULL;
        pOBBVB_Selected = NULL;
        pOBBIB = NULL;
#endif DEBUG_MODE
#endif
    }
    void Release(void)
    {
#ifdef TK_D3D9
        SAFERELEASE(pVertexBuffer);
        SAFERELEASE(pMatrixIndex);
        SAFERELEASE(pMatrixWeight);
        SAFERELEASE(pTexelBuffer);
        SAFERELEASE(pColorBuffer);
        SAFERELEASE(pNormalBuffer);
#ifdef DEBUG_MODE
        SAFERELEASE(pOBBVB);
        SAFERELEASE(pOBBVB_Selected);
        SAFERELEASE(pOBBIB);
#endif DEBUG_MODE
#endif
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// 메쉬 데이터 구조체
///////////////////////////////////////////////////////////////////////////////////////////////////
struct ST_ANIMOTION
{
public:
    string  MotionName;
    int     StartFrame;
    int     EndFrame;

    ST_ANIMOTION(void):StartFrame(0),EndFrame(0)
    {   MotionName = "";  }
    ST_ANIMOTION(const ST_ANIMOTION & st_AniMotion):StartFrame(0),EndFrame(0)
    {   MotionName = ""; (*this) = st_AniMotion; }
    ~ST_ANIMOTION(void)                         {;}

    const ST_ANIMOTION & operator = (const ST_ANIMOTION & st_AniMotion)
    {
        this->MotionName = st_AniMotion.MotionName;
        this->StartFrame = st_AniMotion.StartFrame;
        this->EndFrame = st_AniMotion.EndFrame;
        return st_AniMotion;
    }
};
typedef unordered_map<string, ST_ANIMOTION> MOTIONLIST;

struct ST_GEOM_VERTEX_WEIGHT
{
private:
    string          ParentNodeName;     // 가중치를 참조할 Bone 노드 네임
public:
    float           Weight;             // 가중치
    int             RefNodeIndex;       // 참조된 BoneIndex
    TKMeshVertexes* pRefNode;           // 참조된 Bone노드

    ST_GEOM_VERTEX_WEIGHT(void)
        :Weight(0),RefNodeIndex(0),pRefNode(NULL)
    {   ParentNodeName = "";  }

    ST_GEOM_VERTEX_WEIGHT(const ST_GEOM_VERTEX_WEIGHT & st_Geom_Vertex_Weight)
        :Weight(0),RefNodeIndex(0),pRefNode(NULL)
    {   ParentNodeName = ""; (*this)=st_Geom_Vertex_Weight;  }

    ~ST_GEOM_VERTEX_WEIGHT(void)
    { ; }

    string GetParentNodeName(void)                      { return ParentNodeName; }
    void SetParentNodeName(string ParentNodeName)       { this->ParentNodeName = ParentNodeName; }

    const ST_GEOM_VERTEX_WEIGHT & operator = (const ST_GEOM_VERTEX_WEIGHT & st_Geom_Vertex_Weight)
    {
        this->ParentNodeName    = st_Geom_Vertex_Weight.ParentNodeName;
        this->Weight            = st_Geom_Vertex_Weight.Weight;
        this->RefNodeIndex      = st_Geom_Vertex_Weight.RefNodeIndex;
        this->pRefNode          = st_Geom_Vertex_Weight.pRefNode;
        return st_Geom_Vertex_Weight;
    }
};

struct ST_GEOM_VERTEX
{
    D3DXVECTOR3             Pos;
    ST_GEOM_VERTEX_WEIGHT   Weights[4];

    ST_GEOM_VERTEX(void):Pos(0.0f,0.0f,0.0f){ ; }
    ST_GEOM_VERTEX(const ST_GEOM_VERTEX & st_Geom_Vertex):Pos(0.0f,0.0f,0.0f){ (*this) = st_Geom_Vertex; }
    ~ST_GEOM_VERTEX(void){;}

    void ResetWeights(void)
    {
        Weights[0].SetParentNodeName(string(""));
        Weights[0].Weight = 1.0f;
        Weights[0].RefNodeIndex = -1;
        Weights[0].pRefNode = NULL;
        for ( int i = 1 ; i < 4 ; ++i )
        {
            Weights[i].SetParentNodeName(string(""));
            Weights[i].Weight = 0.0f;
            Weights[i].RefNodeIndex = -1;
            Weights[i].pRefNode = NULL;
        }
    }

    const ST_GEOM_VERTEX & operator = (const ST_GEOM_VERTEX & st_Geom_Vertex)
    {
        this->Pos = st_Geom_Vertex.Pos;
        this->Weights[0] = st_Geom_Vertex.Weights[0];
        this->Weights[1] = st_Geom_Vertex.Weights[1];
        this->Weights[2] = st_Geom_Vertex.Weights[2];
        this->Weights[3] = st_Geom_Vertex.Weights[3];
        return st_Geom_Vertex;
    }
};

////////////////////////////////////////////////////////////////////////////////
// TKMeshMaterial
////////////////////////////////////////////////////////////////////////////////
class TKMeshMaterial : public TKEngineResource
{
    typedef TKEngineResource BASECLASS;

    friend class TKMeshRes;
    friend class TKLODTerrainRes;
protected:
    TKTexture       *pTextures[MATERIAL_TEXTURE_COUNT];

    bool            AlphaBlend;
	D3DBLEND        AlphaBlend_Src;
    D3DBLEND        AlphaBlend_Dest;
    D3DBLENDOP      AlphaBlend_OP;
    bool            AlphaTest;
    D3DCMPFUNC      AlphaTestFunc;
    DWORD           AlphaTestRef;
    bool            TwoSided;
    bool            ZWriteEnable;

    MATERIALLIST    MaterialList;

    TKMeshMaterial(TKEngine *pEngine, string NodeName);
    virtual ~TKMeshMaterial(void);
public:

    const TKMeshMaterial & operator = (const TKMeshMaterial & MeshMaterial);

    void Init(void)     { TKEngineResource::Init(); }
    void Release(void)  { TKEngineResource::Release(); }

    void SetAlphaBlend(const bool AlphaBlend)       { this->AlphaBlend = AlphaBlend; }
    bool GetAlphaBlend(void)                        { return AlphaBlend; }

    void SetAlphaBlend_Src(const D3DBLEND Value)    { this->AlphaBlend_Src = Value; }
    D3DBLEND GetAlphaBlend_Src(void)                { return AlphaBlend_Src; }

    void SetAlphaBlend_Dest(const D3DBLEND Value)   { this->AlphaBlend_Dest = Value; }
    D3DBLEND GetAlphaBlend_Dest(void)               { return AlphaBlend_Dest; }

    void SetAlphaBlend_OP(const D3DBLENDOP Value)   { this->AlphaBlend_OP = Value; }
    D3DBLENDOP GetAlphaBlend_OP(void)               { return AlphaBlend_OP; }

    void SetAlphaTest(const bool AlphaTest)         { this->AlphaTest = AlphaTest; }
    bool GetAlphaTest(void)                         { return AlphaTest; }

    void SetAlphaTestFunc(const D3DCMPFUNC AlphaTestFunc){ this->AlphaTestFunc = AlphaTestFunc; }
    D3DCMPFUNC GetAlphaTestFunc(void)                    { return AlphaTestFunc; }

    void SetAlphaTestRef(const DWORD AlphaTestRef)  { this->AlphaTestRef = AlphaTestRef; }
    DWORD GetAlphaTestRef(void)                     { return AlphaTestRef; }

    void SetTwoSided(const bool TwoSided)           { this->TwoSided = TwoSided; }
    bool GetTwoSided(void)                          { return TwoSided; }

    void SetZWriteEnable(const bool ZWriteEnable)   { this->ZWriteEnable = ZWriteEnable; }
    bool GetZWriteEnable(void)                      { return ZWriteEnable; }

    TKMeshMaterial * GetMaterial(const string MaterialName)
    {
        MATERIALLIST::iterator it;
        it = MaterialList.find(MaterialName);
        if ( it != MaterialList.end() )
        {
            return it->second;
        }
        else
        {
            return NULL;
        }
    }
    MATERIALLIST & GetMaterialList(void)            { return MaterialList; }

    TKMeshMaterial * NewMaterial(string MaterialName);            // 새로운 메터리얼 생성
    void DeleteMaterial(TKMeshMaterial * pMaterial);                // 메터리얼 삭제
    void DeleteMaterial(string MaterialName);                     // 메터리얼 삭제

    TKTexture ** GetTextures(void)                  { return pTextures; }
    TKTexture * GetTexture(const int Index)         { return pTextures[Index]; }
    void SetTexture(const int Index, TKTexture *pTexture)   { this->pTextures[Index] = pTexture; }
};

////////////////////////////////////////////////////////////////////////////////
// TKMeshVertexes
////////////////////////////////////////////////////////////////////////////////
class TKMeshVertexes : public TKVertexResource
{
    typedef TKVertexResource BASECLASS;

    friend class TKMeshRes;
    friend class TKLODTerrainRes;
protected:
    string      BoneNodeName;                       // 엑스포팅된 파일에서 읽어옴. 애니메이션 부모 노드이름.
    int         AnimationMatrixIndex;               // 이 객체의 애니메이션 매트릭스 인덱스(BoneNodeName 에 의해 결정된다.)
    bool        IsSelected;                         // (툴에서) 현재 선택된 버텍스 집합

    // 오리지널 버텍스 정보(포지션 정보와, 부모노드의 네임과, 가중치를 가지고 있다. Release 시에 초기화하면 안된다.)
    vector<ST_GEOM_VERTEX>      OriginVertexList;   // 오리지널 버텍스 정보와, 가중치 정보를 가지고 있는 구조체

    // 이하 리스트들은 Release 시에 초기화 하면 안된다.
    D3DXVECTOR3S                PosList;            // 버텍스 Pos 리스트
    vector<D3DXVECTOR4>         MatrixWeightList;   // 버텍스당 매트릭스 애니메이션 가중치
    vector<ST_MESHMATRIXINDEX>  MatrixIndexList;    // 버텍스당 매트릭스 애니메이션 인덱스
    vector<D3DXVECTOR2>         TexPosList;         // 텍스쳐 좌표 리스트
    vector<D3DCOLOR>            ColorList;          // 버텍스 칼라 리스트
    D3DXVECTOR3S                NormalList;         // 버텍스 노말 정보
    vector<ST_ANIMATRIX>        AniMatrixList;      // 애니메이션 매트릭스 리스트

    // 이 부분 순서 바꾸면 안됨(파일로딩이 이 형태/순서대로 memcpy함)
    D3DXVECTOR3                 vecCenter;          // 바운딩박스의 중점
    D3DXVECTOR3                 vecAxis[3];         // 바운딩박스의 고유축
    float                       AxisLen[3];         // 고유축 길이
    D3DXVECTOR3                 vecOBB[8];          // OBB를 정의하는 점 8개
    D3DXVECTOR3                 vecMin;             // AABB 기준 최소값
    D3DXVECTOR3                 vecMax;             // AABB 기준 최대값

    // 이부분은 Init 시에 정의된다. Release 시에 초기화 한다.
    ST_MESHVERTEXBUFFER         VertexBuffers;      // 버텍스버퍼 <- 복사 금지!
    EMeshVertexesType           MeshVertexesType;   // 지오메트리 타입

    bool InitVertexFormat(void);
    bool InitVertexBuffers(void);
    
    TKMeshVertexes(TKEngine *pEngine, int ID);
    virtual ~TKMeshVertexes(void);
public:

    void Init(void);
    void Release(void);

    const TKMeshVertexes & operator = (const TKMeshVertexes & MeshVertexes);

    // 부모 객체 이름
    const string GetBoneNodeName(void)                    { return BoneNodeName; }
    void SetBoneNodeName(string BoneNodeName)             { this->BoneNodeName = BoneNodeName; }

    // 특정 틱에 해당되는 애니메이션 행렬을 가져온다.
    ST_ANIMATRIX * GetCurrentAniMat(const float CurrentTick)
    {
        if ( !AniMatrixList.empty() )
            return &AniMatrixList[(int)CurrentTick];
        else
            return NULL;
    }

    // 현재객체의 애니메이션매트릭스배열 인덱스.
    int GetAnimationMatrixIndex(void)                   { return this->AnimationMatrixIndex; }
    void SetAnimationMatrixIndex(int Index)             { this->AnimationMatrixIndex = Index; }

    // (툴에서) 현재 선택된 지오메트리
    bool GetSelected(void)                              { return IsSelected; }
    void SetSelected(bool IsSelected)                   { this->IsSelected = IsSelected; }

    // 버텍스군 타입
    EMeshVertexesType GetMeshVertexesType(void)                 { return MeshVertexesType; }
    void SetMeshVertexesType(EMeshVertexesType MeshVertexesType){ this->MeshVertexesType = MeshVertexesType; }

    // 오리지널 버텍스 정보와, 가중치 정보
    vector<ST_GEOM_VERTEX> & GetOriginVertexList(void)  { return OriginVertexList; }
    // 현재 메쉬의 정점 위치
    D3DXVECTOR3S & GetPosList(void)                     { return PosList; }
    // 버텍스당 매트릭스 애니메이션 가중치
    vector<D3DXVECTOR4> & GetMatrixWeightList(void)     { return MatrixWeightList; }
    // 버텍스당 매트릭스 애니메이션 인덱스
    vector<ST_MESHMATRIXINDEX> & GetMatrixIndexList(void) { return MatrixIndexList; }
    // 현재 메쉬의 텍셀 리스트
    vector<D3DXVECTOR2> & GetTexPosList(void)           { return TexPosList; }
    // 버텍스 칼라 리스트
    vector<D3DCOLOR> & GetColorList(void)               { return ColorList; }
    // 현재 메쉬의 버텍스노말
    D3DXVECTOR3S & GetNormalList(void)                  { return NormalList; }
    // 애니메이션 매트릭스 리스트
    vector<ST_ANIMATRIX> & GetAniMatrixList(void)       { return AniMatrixList; }

    // OBB 바운딩박스의 중점
    const D3DXVECTOR3 & GetOBBCenter(void)              { return vecCenter; }
    void SetOBBCenter(const D3DXVECTOR3 & vecCenter)    { this->vecCenter = vecCenter; }
    // 바운딩박스의 고유축
    const D3DXVECTOR3 * GetOBBAxis(void)                { return vecAxis; }
    void SetOBBAxis(const D3DXVECTOR3 vecAxis[3])       { memcpy(this->vecAxis, vecAxis, sizeof(vecAxis)); }
    // 고유축 길이
    const float * GetOBBAxisLen(void)                   { return AxisLen; }
    void SetOBBAxisLen(const float AxisLen[3])          { memcpy(this->AxisLen, AxisLen, sizeof(AxisLen)); }
    // OBB를 정의하는 점 8개
    const D3DXVECTOR3 * GetOBB(void)                    { return vecOBB; }
    void SetOBB(const D3DXVECTOR3 vecOBB[8])            { memcpy(this->vecOBB, vecOBB, sizeof(vecOBB)); }
    // AABB 기준 최소값
    const D3DXVECTOR3 & GetMin(void)                    { return vecMin; }
    void SetMin(const D3DXVECTOR3 & vecMin)             { this->vecMin = vecMin; }
    // AABB 기준 최대값
    const D3DXVECTOR3 & GetMax(void)                    { return vecMax; }
    void SetMax(const D3DXVECTOR3 & vecMax)             { this->vecMax = vecMax; }

    // 버텍스버퍼 구조체
    ST_MESHVERTEXBUFFER & GetVertexBuffers(void)        { return VertexBuffers; }

    static void Copy_BaseInfo( TKMeshVertexes & Dest, TKMeshVertexes & Src );
};

////////////////////////////////////////////////////////////////////////////////
// TKMeshFaces
////////////////////////////////////////////////////////////////////////////////
class TKMeshFaces : public TKIndexResource
{
    typedef TKIndexResource BASECLASS;

    friend class TKMeshRes;
    friend class TKLODTerrainRes;
protected:
    TKMeshVertexes          *pMeshVertexes;

    // 이하 리스트는 Release 시에 초기화 하면 안된다.
    vector<ST_IB_TRIANGLE>      FaceList;           // 페이스 정보

    bool InitIndexBuffers(void);
    bool InitNormalSmoothing(void); //노말 벡터의 합을 정규화하고, FaceNormal정보를 생성함.

    TKMeshFaces(TKEngine *pEngine, int ID);
    virtual ~TKMeshFaces(void);
public:

    void Init(void);
    void Release(void);

    void SetMeshVertexes(TKMeshVertexes *pMeshVertexes) { this->pMeshVertexes = pMeshVertexes; }
    TKMeshVertexes * GetMeshVertexes(void)              { return this->pMeshVertexes; }

    const TKMeshFaces & operator = (const TKMeshFaces & MeshFaces);

    // 현재 메쉬의 페이스 리스트
    vector<ST_IB_TRIANGLE> & GetFaceList(void)          { return FaceList; }

    static void Copy_BaseInfo( TKMeshFaces & Dest, TKMeshFaces & Src );
};

////////////////////////////////////////////////////////////////////////////////
// TKMeshGeometry
////////////////////////////////////////////////////////////////////////////////
class TKMeshGeometry : public TKEngineResource
{
    typedef TKEngineResource BASECLASS;

    friend class TKMeshRes;
    friend class TKLODTerrainRes;
protected:
    TKMeshVertexes  *pMeshVertexes;                 // 메쉬 버텍스
    TKMeshFaces     *pMeshFaces;                    // 메쉬 인덱스

    // TKMAXFileConverter/TKModelFileConverter 클래스에서 정의한다. Release시에 초기화 하지 않는다.
    string                      MaterialName;       // 자신 객체의 메터리얼 이름
    string                      SubMaterialName;    // 자신 객체의 서브메터리얼 이름
    TKMeshMaterial             *pMaterial;          // 자신 객체의 메터리얼 객체 포인터 <- 복사 금지!
    TKMeshMaterial             *pSubMaterial;       // 자신 객체의 서브메터리얼 객체 포인터 <- 복사 금지!

    // Init 하기 전에 사용자가 메서드를 이용하여 직접 값을 넣어야 하는 값들.
    TKMeshShaderEffect         *pShaderEffect;      // 렌더링시 사용할 이펙트쉐이더 객체포인터 <- 포인터 복사 가능

    TKMeshGeometry(TKEngine *pEngine, string NodeName);
    virtual ~TKMeshGeometry(void);
public:

    void Init(void);
    void Release(void);

    const TKMeshGeometry & operator = (const TKMeshGeometry & MeshGeometry);

    void SetMeshVertexes(TKMeshVertexes *pMeshVertexes) { this->pMeshVertexes = pMeshVertexes; }
    TKMeshVertexes * GetMeshVertexes(void)              { return this->pMeshVertexes; }
    void SetMeshFaces(TKMeshFaces *pMeshFaces)          { this->pMeshFaces = pMeshFaces; pMeshFaces->SetMeshVertexes(pMeshVertexes); }
    TKMeshFaces * GetMeshFaces(void)                    { return this->pMeshFaces; }

    // 부모 객체 이름
    const string GetBoneNodeName(void)                { return pMeshVertexes->GetBoneNodeName(); }
    void SetBoneNodeName(string BoneNodeName)         { pMeshVertexes->SetBoneNodeName( BoneNodeName ); }

    // 쉐이더 객체
    TKMeshShaderEffect * GetShaderEffect(void)              { return pShaderEffect; }
    void SetShaderEffect(TKMeshShaderEffect *pShaderEffect) { this->pShaderEffect = pShaderEffect; }

    // 특정 틱에 해당되는 애니메이션 행렬을 가져온다.
    ST_ANIMATRIX * GetCurrentAniMat(const float CurrentTick){ return pMeshVertexes->GetCurrentAniMat(CurrentTick); }

    // 현재객체의 애니메이션매트릭스배열 인덱스.
    int GetAnimationMatrixIndex(void)                   { return pMeshVertexes->GetAnimationMatrixIndex(); }
    void SetAnimationMatrixIndex(int Index)             { pMeshVertexes->SetAnimationMatrixIndex(Index); }

    // (툴에서) 현재 선택된 지오메트리
    bool GetSelected(void)                              { return pMeshVertexes->GetSelected(); }
    void SetSelected(bool IsSelected)                   { pMeshVertexes->SetSelected(IsSelected); }

    // 버텍스군 타입
    EMeshVertexesType GetMeshVertexesType(void)         { return pMeshVertexes->GetMeshVertexesType(); }
    void SetMeshVertexesType(EMeshVertexesType MeshVertexesType){ pMeshVertexes->SetMeshVertexesType(MeshVertexesType); }

    // 메터리얼 이름
    string GetMaterialName(void)                        { return MaterialName; }
    void SetMaterialName(string Name)                   { this->MaterialName = Name; }

    string GetSubMaterialName(void)                     { return SubMaterialName; }
    void SetSubMaterialName(string Name)                { this->SubMaterialName = Name; }

    // 메터리얼 객체 정의
    TKMeshMaterial * GetMaterial(void)                  { return pMaterial; }
    void SetMaterial(TKMeshMaterial* pMaterial)         { this->pMaterial = pMaterial; }
    // 서브메터리얼 객체 정의
    TKMeshMaterial * GetSubMaterial(void)               { return pSubMaterial; }
    void SetSubMaterial(TKMeshMaterial* pSubMaterial)   { this->pSubMaterial = pSubMaterial; }

    // 오리지널 버텍스 정보와, 가중치 정보
    vector<ST_GEOM_VERTEX> & GetOriginVertexList(void)  { return pMeshVertexes->GetOriginVertexList(); }
    // 현재 메쉬의 정점 위치
    D3DXVECTOR3S & GetPosList(void)                     { return pMeshVertexes->GetPosList(); }
    // 버텍스당 매트릭스 애니메이션 가중치
    vector<D3DXVECTOR4> & GetMatrixWeightList(void)     { return pMeshVertexes->GetMatrixWeightList(); }
    // 버텍스당 매트릭스 애니메이션 인덱스
    vector<ST_MESHMATRIXINDEX> & GetMatrixIndexList(void) { return pMeshVertexes->GetMatrixIndexList(); }
    // 현재 메쉬의 텍셀 리스트
    vector<D3DXVECTOR2> & GetTexPosList(void)           { return pMeshVertexes->GetTexPosList(); }
    // 버텍스 칼라 리스트
    vector<D3DCOLOR> & GetColorList(void)               { return pMeshVertexes->GetColorList(); }
    // 현재 메쉬의 버텍스노말
    D3DXVECTOR3S & GetNormalList(void)                  { return pMeshVertexes->GetNormalList(); }
    // 현재 메쉬의 페이스 리스트
    vector<ST_IB_TRIANGLE> & GetFaceList(void)          { return pMeshFaces->GetFaceList(); }
    // 애니메이션 매트릭스 리스트
    vector<ST_ANIMATRIX> & GetAniMatrixList(void)       { return pMeshVertexes->GetAniMatrixList(); }

    // 바운딩박스의 중점
    const D3DXVECTOR3 & GetOBBCenter(void)              { return pMeshVertexes->GetOBBCenter(); }
    void SetOBBCenter(const D3DXVECTOR3 & vecCenter)    { pMeshVertexes->SetOBBCenter(vecCenter); }
    // 바운딩박스의 고유축
    const D3DXVECTOR3 * GetOBBAxis(void)                { return pMeshVertexes->GetOBBAxis(); }
    void SetOBBAxis(const D3DXVECTOR3 vecAxis[3])       { pMeshVertexes->SetOBBAxis(vecAxis); }
    // 고유축 길이
    const float * GetOBBAxisLen(void)                   { return pMeshVertexes->GetOBBAxisLen(); }
    void SetOBBAxisLen(const float AxisLen[3])          { pMeshVertexes->SetOBBAxisLen(AxisLen); }
    // OBB를 정의하는 점 8개
    const D3DXVECTOR3 * GetOBB(void)                    { return pMeshVertexes->GetOBB(); }
    void SetOBB(const D3DXVECTOR3 vecOBB[8])            { pMeshVertexes->SetOBB(vecOBB); }
    // AABB 기준 최소값
    const D3DXVECTOR3 & GetMin(void)                    { return pMeshVertexes->GetMin(); }
    void SetMin(const D3DXVECTOR3 & vecMin)             { pMeshVertexes->SetMin(vecMin); }
    // AABB 기준 최대값
    const D3DXVECTOR3 & GetMax(void)                    { return pMeshVertexes->GetMax(); }
    void SetMax(const D3DXVECTOR3 & vecMax)             { pMeshVertexes->SetMax(vecMax); }

    // 버텍스버퍼 구조체
    ST_MESHVERTEXBUFFER & GetVertexBuffers(void)        { return pMeshVertexes->GetVertexBuffers(); }

    static void Copy_BaseInfo( TKMeshGeometry & Dest, TKMeshGeometry & Src );
    static bool CreateOBBGeomList( GEOMLIST & GeomList, D3DXVECTOR3 & vecCenter, D3DXVECTOR3 vecAxis[3], float AxisLen[3], D3DXVECTOR3 vecOBB[8]);

#ifdef TK_D3D9
	LPDIRECT3DVERTEXBUFFER9 & GetVB(void)       { return pMeshVertexes->GetVB(); }
    LPDIRECT3DVERTEXDECLARATION9 & GetVDEC(void){ return pMeshVertexes->GetVDEC(); }
    LPDIRECT3DINDEXBUFFER9 & GetIB(void)        { return pMeshFaces->GetIB(); }
#endif
};

////////////////////////////////////////////////////////////////////////////////
// TKMeshRes
////////////////////////////////////////////////////////////////////////////////
class TKMeshRes : public TKEngineResource
{
    typedef TKEngineResource BASECLASS;

    friend class TKCustomFileConverter;
    friend class TKMAXFileConverter;
    friend class TKModelFileConverter;
    friend class TKSGObjectFileConverter;
protected:
    MATERIALLIST    MaterialList;                   // 메터리얼 리스트
    VERTEXESLIST    VertexesList;                   // 버텍스집합 리스트
    FACESLIST       FacesList;                      // 인덱스집합 리스트
    GEOMLIST        BoneList;                       // 본 데이터
    GEOMLIST        GeomList;                       // 메쉬 데이터
    LODLIST         LodList;                        // Lod 메쉬 리스트

    static bool CompareGEOMLIST_ALPHA(TKMeshGeometry *pA, TKMeshGeometry *pB);

    TKMeshShaderEffect *pShaderEffect;              // 쉐이더 이펙트 객체 포인터
    
    void MaterialBind(GEOMLIST & ParamList);        // 메쉬와 메터리얼을 바인드    

public:
    TKMeshRes(TKEngine *pEngine, int ID);
    virtual ~TKMeshRes(void);

    virtual void Init(void);
    virtual void Release(void);

    virtual void ClearDelete(void);
    static void Copy_BaseInfo( TKMeshRes & DestMeshRes, TKMeshRes & SrcMeshRes );

    MATERIALLIST & GetMaterialList(void)                { return MaterialList; }
    VERTEXESLIST & GetVertexesList(void)                { return VertexesList; }
    FACESLIST  & GetFacesList(void)                     { return FacesList; }
    GEOMLIST & GetBoneList(void)                        { return BoneList; }
    GEOMLIST & GetGeomList(void)                        { return GeomList; }
    LODLIST & GetLodList(void)                          { return LodList; }

    TKMeshShaderEffect *GetShaderEffect(void)           { return pShaderEffect; }
    void SetShaderEffect(TKMeshShaderEffect *pShaderEffect);

    // LOD 관련 함수
    bool NewLODDistance(int Distance);                              // LOD단계를 추가한.
    bool DeleteLODDistance(int Distance);                           // LOD단계를 삭제한다.
    bool ClearLODGeometry(int Distance);                            // 해당 LOD단계에 등록된 지오메트리 리스트를 초기화한다.
    bool AddLODGeometry(int Distance, TKMeshGeometry *pGeometry);   // 해당 LOD단계에 지오메트리를 추가한다.
    bool ClearAllLODGeometroy(void);                                // 최종단계를 제외한 모든 단계의 지오메트리 리스트를 삭제한다.
    bool AddAllGeometryToInfinityLevel(void);                       // 최종단계의 LOD레벨에 모든 지오메트리를 추가한다.

    // 메터리얼 관련 함수
    TKMeshMaterial * NewMaterial(string MaterialName);            // 새로운 메터리얼 생성
    void DeleteMaterial(TKMeshMaterial * pMaterial);                // 메터리얼 삭제
    void DeleteMaterial(string MaterialName);                     // 메터리얼 삭제
    void DeleteNoReferenceMaterial(void);                           // 참조되지 않는 메터리얼 삭제

    // 버텍스집합 관련함수
    TKMeshVertexes * NewVertexes(string NodeName, EMeshVertexesType MeshVertexesType);    // 새로운 버텍스군 생성.
    void DeleteVertexes(TKMeshVertexes * pMeshVertexes);                // 버텍스군 삭제.
    void DeleteNoReferenceBoneVertexes(void);                           // 참조되지 않는 본 버텍스 삭제.
    TKMeshVertexes * GetVertexesByNodeName(const string NodeName);    // 노드 이름으로 버텍스군 검색한다.

    // 인덱스집합 관련함수
    TKMeshFaces * NewFaces(string NodeName, TKMeshVertexes * pVertexes); // 새로운 페이스군 생성.
    void DeleteFaces(TKMeshFaces * pMeshFaces);                     // 페이스군 삭제.
    void DeleteNoRefFaces(void);                                    // 사용하지 않는 페이스군 삭제.
    TKMeshFaces * GetFacesByNodeName(const string NodeName);      // 노드 이름으로 페이스군 검색한다.

    // 지오메트리 관련 함수
    TKMeshGeometry * NewGeometry(string NodeName, TKMeshVertexes * pVertexes, TKMeshFaces * pFaces);    // 새 지오메트리 생성
    void DeleteGeometry(TKMeshGeometry * pGeometry);                // 지오메트리 삭제
    void DeleteNoReferenceBone(void);                               // 참조되지 않는 Bone 지오메트리 객체를 검색하여 삭제.
    void SortGeomListByAlpha(void);                                 // 지오메트리 알파블랜드/이름순 정렬
    void MoveGeomList(const int Index, TKMeshGeometry *pMeshGeom);  // 지오메트리 이동
    TKMeshGeometry * GetGeomByNodeName(const string NodeName);    // 노드 이름으로 지오메트리 검색한다.

    // AABB 기준 최소값/최대값.
    D3DXVECTOR3 GetMin(void);
    D3DXVECTOR3 GetMax(void);
};

////////////////////////////////////////////////////////////////////////////////
// TKAniMeshRes
////////////////////////////////////////////////////////////////////////////////
class TKAniMeshRes : public TKMeshRes
{
    typedef TKMeshRes BASECLASS;

    friend class TKCustomFileConverter;
    friend class TKMAXFileConverter;
    friend class TKModelFileConverter;
    friend class TKSGObjectFileConverter;
protected:
    MOTIONLIST          AniMotionList;              // 모션데이터 리스트

    int                 m_FirstFrame;               // 현재 애니메이션의 첫 프레임 번호
    int                 m_LastFrame;                // 현재 애니메이션의 마지막 프레임 번호
    int                 m_FrameSpeed;               // 현재 애니메이션의 초당 프레임 스피드

    int                 m_MatrixArrayColCount;      // 애니메이션 매트릭스 본 갯수(라고 할수있다)
    int                 m_MatrixArrayRowCount;      // 애니메이션 매트릭스 프레임 카운트
    int                 m_MatrixArrayTotalCount;    // 애니메이션 매트릭스 토탈 카운트(MatrixArrayRowCount * MatrixArrayColCount)
    D3DXMATRIX         *m_pMatrixArray;             // 애니메이션 매트릭스 배열

    void Build60FPSAnimation(GEOMLIST & ParamList); // 애니메이션 프레임을 60프레임으로 확장(미구현)
    void SetAnimationMatrixIndex(void);             // 메쉬마다 애니메이션 매트릭스를 조사하여 매트릭스 카운트를 세팅
    void SetAnimationMatrixData(void);              // 애니메이션 매트릭스 배열 데이터를 생성
    void MeshAnimationBind(void);                   // 메쉬와 애니메이션을 바인드(Mesh의 애니메이션 인덱스를 채워줌)

public:
    TKAniMeshRes(TKEngine *pEngine, int ID);
    virtual ~TKAniMeshRes(void);

    void Init(void);
    void Release(void);

    static void Copy_BaseInfo( TKAniMeshRes & DestMeshRes, TKAniMeshRes & SrcMeshRes );

    int GetFirstFrame(void)                             { return this->m_FirstFrame; }
    void SetFirstFrame(int FirstFrame)                  { this->m_FirstFrame = FirstFrame; }
    int GetLastFrame(void)                              { return this->m_LastFrame; }
    void SetLastFrame(int LastFrame)                    { this->m_LastFrame = LastFrame; }
    int GetFrameSpeed(void)                             { return this->m_FrameSpeed; }
    void SetFrameSpeed(int FrameSpeed)                  { this->m_FrameSpeed = FrameSpeed; }

    MOTIONLIST & GetMotionList(void)                    { return AniMotionList; }

    // 애니메이션 관련 함수
    pair<int, D3DXMATRIX *> GetAnimationMatrix(int CurrFrame);      // 현재 프레임의 애니메이션 매트릭스배열 포인터 가져오기.
    const ST_ANIMOTION GetAniMotionByName(string MotionName);       // 저장된 애니메이션 모션 데이터 가져오기
    int GetMatArrColCnt()   { return m_MatrixArrayColCount; }       // 애니메이션 매트릭스 카운트(라고 할수있다)
    int GetMatArrRowCnt()   { return m_MatrixArrayRowCount; }       // 애니메이션 매트릭스 프레임 카운트
    int GetMatArrTotalCnt() { return m_MatrixArrayTotalCount; }     // 애니메이션 매트릭스 토탈 카운트

    // FileIO 함수
    bool LoadLodFile(string FileName);                              // LOD 데이터 파일 로딩 함수
    bool SaveLodFile(string FileName);                              // LOD 데이터 파일 저장 함수
    bool LoadMotionFile(string FileName);                           // 애니메이션 모션 로딩 함수
    bool SaveMotionFile(string FileName);                           // 애니메이션 모션 저장 함수
    bool LoadFromFile(string FileName);                             // 모델 데이터 파일 로딩 함수
    bool SaveToFile(string FileName);                               // 모델 데이터 파일 저장 함수
};