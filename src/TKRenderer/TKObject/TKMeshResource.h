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
// �޽� ������ ����ü
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
    string          ParentNodeName;     // ����ġ�� ������ Bone ��� ����
public:
    float           Weight;             // ����ġ
    int             RefNodeIndex;       // ������ BoneIndex
    TKMeshVertexes* pRefNode;           // ������ Bone���

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

    TKMeshMaterial * NewMaterial(string MaterialName);            // ���ο� ���͸��� ����
    void DeleteMaterial(TKMeshMaterial * pMaterial);                // ���͸��� ����
    void DeleteMaterial(string MaterialName);                     // ���͸��� ����

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
    string      BoneNodeName;                       // �������õ� ���Ͽ��� �о��. �ִϸ��̼� �θ� ����̸�.
    int         AnimationMatrixIndex;               // �� ��ü�� �ִϸ��̼� ��Ʈ���� �ε���(BoneNodeName �� ���� �����ȴ�.)
    bool        IsSelected;                         // (������) ���� ���õ� ���ؽ� ����

    // �������� ���ؽ� ����(������ ������, �θ����� ���Ӱ�, ����ġ�� ������ �ִ�. Release �ÿ� �ʱ�ȭ�ϸ� �ȵȴ�.)
    vector<ST_GEOM_VERTEX>      OriginVertexList;   // �������� ���ؽ� ������, ����ġ ������ ������ �ִ� ����ü

    // ���� ����Ʈ���� Release �ÿ� �ʱ�ȭ �ϸ� �ȵȴ�.
    D3DXVECTOR3S                PosList;            // ���ؽ� Pos ����Ʈ
    vector<D3DXVECTOR4>         MatrixWeightList;   // ���ؽ��� ��Ʈ���� �ִϸ��̼� ����ġ
    vector<ST_MESHMATRIXINDEX>  MatrixIndexList;    // ���ؽ��� ��Ʈ���� �ִϸ��̼� �ε���
    vector<D3DXVECTOR2>         TexPosList;         // �ؽ��� ��ǥ ����Ʈ
    vector<D3DCOLOR>            ColorList;          // ���ؽ� Į�� ����Ʈ
    D3DXVECTOR3S                NormalList;         // ���ؽ� �븻 ����
    vector<ST_ANIMATRIX>        AniMatrixList;      // �ִϸ��̼� ��Ʈ���� ����Ʈ

    // �� �κ� ���� �ٲٸ� �ȵ�(���Ϸε��� �� ����/������� memcpy��)
    D3DXVECTOR3                 vecCenter;          // �ٿ���ڽ��� ����
    D3DXVECTOR3                 vecAxis[3];         // �ٿ���ڽ��� ������
    float                       AxisLen[3];         // ������ ����
    D3DXVECTOR3                 vecOBB[8];          // OBB�� �����ϴ� �� 8��
    D3DXVECTOR3                 vecMin;             // AABB ���� �ּҰ�
    D3DXVECTOR3                 vecMax;             // AABB ���� �ִ밪

    // �̺κ��� Init �ÿ� ���ǵȴ�. Release �ÿ� �ʱ�ȭ �Ѵ�.
    ST_MESHVERTEXBUFFER         VertexBuffers;      // ���ؽ����� <- ���� ����!
    EMeshVertexesType           MeshVertexesType;   // ������Ʈ�� Ÿ��

    bool InitVertexFormat(void);
    bool InitVertexBuffers(void);
    
    TKMeshVertexes(TKEngine *pEngine, int ID);
    virtual ~TKMeshVertexes(void);
public:

    void Init(void);
    void Release(void);

    const TKMeshVertexes & operator = (const TKMeshVertexes & MeshVertexes);

    // �θ� ��ü �̸�
    const string GetBoneNodeName(void)                    { return BoneNodeName; }
    void SetBoneNodeName(string BoneNodeName)             { this->BoneNodeName = BoneNodeName; }

    // Ư�� ƽ�� �ش�Ǵ� �ִϸ��̼� ����� �����´�.
    ST_ANIMATRIX * GetCurrentAniMat(const float CurrentTick)
    {
        if ( !AniMatrixList.empty() )
            return &AniMatrixList[(int)CurrentTick];
        else
            return NULL;
    }

    // ���簴ü�� �ִϸ��̼Ǹ�Ʈ�����迭 �ε���.
    int GetAnimationMatrixIndex(void)                   { return this->AnimationMatrixIndex; }
    void SetAnimationMatrixIndex(int Index)             { this->AnimationMatrixIndex = Index; }

    // (������) ���� ���õ� ������Ʈ��
    bool GetSelected(void)                              { return IsSelected; }
    void SetSelected(bool IsSelected)                   { this->IsSelected = IsSelected; }

    // ���ؽ��� Ÿ��
    EMeshVertexesType GetMeshVertexesType(void)                 { return MeshVertexesType; }
    void SetMeshVertexesType(EMeshVertexesType MeshVertexesType){ this->MeshVertexesType = MeshVertexesType; }

    // �������� ���ؽ� ������, ����ġ ����
    vector<ST_GEOM_VERTEX> & GetOriginVertexList(void)  { return OriginVertexList; }
    // ���� �޽��� ���� ��ġ
    D3DXVECTOR3S & GetPosList(void)                     { return PosList; }
    // ���ؽ��� ��Ʈ���� �ִϸ��̼� ����ġ
    vector<D3DXVECTOR4> & GetMatrixWeightList(void)     { return MatrixWeightList; }
    // ���ؽ��� ��Ʈ���� �ִϸ��̼� �ε���
    vector<ST_MESHMATRIXINDEX> & GetMatrixIndexList(void) { return MatrixIndexList; }
    // ���� �޽��� �ؼ� ����Ʈ
    vector<D3DXVECTOR2> & GetTexPosList(void)           { return TexPosList; }
    // ���ؽ� Į�� ����Ʈ
    vector<D3DCOLOR> & GetColorList(void)               { return ColorList; }
    // ���� �޽��� ���ؽ��븻
    D3DXVECTOR3S & GetNormalList(void)                  { return NormalList; }
    // �ִϸ��̼� ��Ʈ���� ����Ʈ
    vector<ST_ANIMATRIX> & GetAniMatrixList(void)       { return AniMatrixList; }

    // OBB �ٿ���ڽ��� ����
    const D3DXVECTOR3 & GetOBBCenter(void)              { return vecCenter; }
    void SetOBBCenter(const D3DXVECTOR3 & vecCenter)    { this->vecCenter = vecCenter; }
    // �ٿ���ڽ��� ������
    const D3DXVECTOR3 * GetOBBAxis(void)                { return vecAxis; }
    void SetOBBAxis(const D3DXVECTOR3 vecAxis[3])       { memcpy(this->vecAxis, vecAxis, sizeof(vecAxis)); }
    // ������ ����
    const float * GetOBBAxisLen(void)                   { return AxisLen; }
    void SetOBBAxisLen(const float AxisLen[3])          { memcpy(this->AxisLen, AxisLen, sizeof(AxisLen)); }
    // OBB�� �����ϴ� �� 8��
    const D3DXVECTOR3 * GetOBB(void)                    { return vecOBB; }
    void SetOBB(const D3DXVECTOR3 vecOBB[8])            { memcpy(this->vecOBB, vecOBB, sizeof(vecOBB)); }
    // AABB ���� �ּҰ�
    const D3DXVECTOR3 & GetMin(void)                    { return vecMin; }
    void SetMin(const D3DXVECTOR3 & vecMin)             { this->vecMin = vecMin; }
    // AABB ���� �ִ밪
    const D3DXVECTOR3 & GetMax(void)                    { return vecMax; }
    void SetMax(const D3DXVECTOR3 & vecMax)             { this->vecMax = vecMax; }

    // ���ؽ����� ����ü
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

    // ���� ����Ʈ�� Release �ÿ� �ʱ�ȭ �ϸ� �ȵȴ�.
    vector<ST_IB_TRIANGLE>      FaceList;           // ���̽� ����

    bool InitIndexBuffers(void);
    bool InitNormalSmoothing(void); //�븻 ������ ���� ����ȭ�ϰ�, FaceNormal������ ������.

    TKMeshFaces(TKEngine *pEngine, int ID);
    virtual ~TKMeshFaces(void);
public:

    void Init(void);
    void Release(void);

    void SetMeshVertexes(TKMeshVertexes *pMeshVertexes) { this->pMeshVertexes = pMeshVertexes; }
    TKMeshVertexes * GetMeshVertexes(void)              { return this->pMeshVertexes; }

    const TKMeshFaces & operator = (const TKMeshFaces & MeshFaces);

    // ���� �޽��� ���̽� ����Ʈ
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
    TKMeshVertexes  *pMeshVertexes;                 // �޽� ���ؽ�
    TKMeshFaces     *pMeshFaces;                    // �޽� �ε���

    // TKMAXFileConverter/TKModelFileConverter Ŭ�������� �����Ѵ�. Release�ÿ� �ʱ�ȭ ���� �ʴ´�.
    string                      MaterialName;       // �ڽ� ��ü�� ���͸��� �̸�
    string                      SubMaterialName;    // �ڽ� ��ü�� ������͸��� �̸�
    TKMeshMaterial             *pMaterial;          // �ڽ� ��ü�� ���͸��� ��ü ������ <- ���� ����!
    TKMeshMaterial             *pSubMaterial;       // �ڽ� ��ü�� ������͸��� ��ü ������ <- ���� ����!

    // Init �ϱ� ���� ����ڰ� �޼��带 �̿��Ͽ� ���� ���� �־�� �ϴ� ����.
    TKMeshShaderEffect         *pShaderEffect;      // �������� ����� ����Ʈ���̴� ��ü������ <- ������ ���� ����

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

    // �θ� ��ü �̸�
    const string GetBoneNodeName(void)                { return pMeshVertexes->GetBoneNodeName(); }
    void SetBoneNodeName(string BoneNodeName)         { pMeshVertexes->SetBoneNodeName( BoneNodeName ); }

    // ���̴� ��ü
    TKMeshShaderEffect * GetShaderEffect(void)              { return pShaderEffect; }
    void SetShaderEffect(TKMeshShaderEffect *pShaderEffect) { this->pShaderEffect = pShaderEffect; }

    // Ư�� ƽ�� �ش�Ǵ� �ִϸ��̼� ����� �����´�.
    ST_ANIMATRIX * GetCurrentAniMat(const float CurrentTick){ return pMeshVertexes->GetCurrentAniMat(CurrentTick); }

    // ���簴ü�� �ִϸ��̼Ǹ�Ʈ�����迭 �ε���.
    int GetAnimationMatrixIndex(void)                   { return pMeshVertexes->GetAnimationMatrixIndex(); }
    void SetAnimationMatrixIndex(int Index)             { pMeshVertexes->SetAnimationMatrixIndex(Index); }

    // (������) ���� ���õ� ������Ʈ��
    bool GetSelected(void)                              { return pMeshVertexes->GetSelected(); }
    void SetSelected(bool IsSelected)                   { pMeshVertexes->SetSelected(IsSelected); }

    // ���ؽ��� Ÿ��
    EMeshVertexesType GetMeshVertexesType(void)         { return pMeshVertexes->GetMeshVertexesType(); }
    void SetMeshVertexesType(EMeshVertexesType MeshVertexesType){ pMeshVertexes->SetMeshVertexesType(MeshVertexesType); }

    // ���͸��� �̸�
    string GetMaterialName(void)                        { return MaterialName; }
    void SetMaterialName(string Name)                   { this->MaterialName = Name; }

    string GetSubMaterialName(void)                     { return SubMaterialName; }
    void SetSubMaterialName(string Name)                { this->SubMaterialName = Name; }

    // ���͸��� ��ü ����
    TKMeshMaterial * GetMaterial(void)                  { return pMaterial; }
    void SetMaterial(TKMeshMaterial* pMaterial)         { this->pMaterial = pMaterial; }
    // ������͸��� ��ü ����
    TKMeshMaterial * GetSubMaterial(void)               { return pSubMaterial; }
    void SetSubMaterial(TKMeshMaterial* pSubMaterial)   { this->pSubMaterial = pSubMaterial; }

    // �������� ���ؽ� ������, ����ġ ����
    vector<ST_GEOM_VERTEX> & GetOriginVertexList(void)  { return pMeshVertexes->GetOriginVertexList(); }
    // ���� �޽��� ���� ��ġ
    D3DXVECTOR3S & GetPosList(void)                     { return pMeshVertexes->GetPosList(); }
    // ���ؽ��� ��Ʈ���� �ִϸ��̼� ����ġ
    vector<D3DXVECTOR4> & GetMatrixWeightList(void)     { return pMeshVertexes->GetMatrixWeightList(); }
    // ���ؽ��� ��Ʈ���� �ִϸ��̼� �ε���
    vector<ST_MESHMATRIXINDEX> & GetMatrixIndexList(void) { return pMeshVertexes->GetMatrixIndexList(); }
    // ���� �޽��� �ؼ� ����Ʈ
    vector<D3DXVECTOR2> & GetTexPosList(void)           { return pMeshVertexes->GetTexPosList(); }
    // ���ؽ� Į�� ����Ʈ
    vector<D3DCOLOR> & GetColorList(void)               { return pMeshVertexes->GetColorList(); }
    // ���� �޽��� ���ؽ��븻
    D3DXVECTOR3S & GetNormalList(void)                  { return pMeshVertexes->GetNormalList(); }
    // ���� �޽��� ���̽� ����Ʈ
    vector<ST_IB_TRIANGLE> & GetFaceList(void)          { return pMeshFaces->GetFaceList(); }
    // �ִϸ��̼� ��Ʈ���� ����Ʈ
    vector<ST_ANIMATRIX> & GetAniMatrixList(void)       { return pMeshVertexes->GetAniMatrixList(); }

    // �ٿ���ڽ��� ����
    const D3DXVECTOR3 & GetOBBCenter(void)              { return pMeshVertexes->GetOBBCenter(); }
    void SetOBBCenter(const D3DXVECTOR3 & vecCenter)    { pMeshVertexes->SetOBBCenter(vecCenter); }
    // �ٿ���ڽ��� ������
    const D3DXVECTOR3 * GetOBBAxis(void)                { return pMeshVertexes->GetOBBAxis(); }
    void SetOBBAxis(const D3DXVECTOR3 vecAxis[3])       { pMeshVertexes->SetOBBAxis(vecAxis); }
    // ������ ����
    const float * GetOBBAxisLen(void)                   { return pMeshVertexes->GetOBBAxisLen(); }
    void SetOBBAxisLen(const float AxisLen[3])          { pMeshVertexes->SetOBBAxisLen(AxisLen); }
    // OBB�� �����ϴ� �� 8��
    const D3DXVECTOR3 * GetOBB(void)                    { return pMeshVertexes->GetOBB(); }
    void SetOBB(const D3DXVECTOR3 vecOBB[8])            { pMeshVertexes->SetOBB(vecOBB); }
    // AABB ���� �ּҰ�
    const D3DXVECTOR3 & GetMin(void)                    { return pMeshVertexes->GetMin(); }
    void SetMin(const D3DXVECTOR3 & vecMin)             { pMeshVertexes->SetMin(vecMin); }
    // AABB ���� �ִ밪
    const D3DXVECTOR3 & GetMax(void)                    { return pMeshVertexes->GetMax(); }
    void SetMax(const D3DXVECTOR3 & vecMax)             { pMeshVertexes->SetMax(vecMax); }

    // ���ؽ����� ����ü
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
    MATERIALLIST    MaterialList;                   // ���͸��� ����Ʈ
    VERTEXESLIST    VertexesList;                   // ���ؽ����� ����Ʈ
    FACESLIST       FacesList;                      // �ε������� ����Ʈ
    GEOMLIST        BoneList;                       // �� ������
    GEOMLIST        GeomList;                       // �޽� ������
    LODLIST         LodList;                        // Lod �޽� ����Ʈ

    static bool CompareGEOMLIST_ALPHA(TKMeshGeometry *pA, TKMeshGeometry *pB);

    TKMeshShaderEffect *pShaderEffect;              // ���̴� ����Ʈ ��ü ������
    
    void MaterialBind(GEOMLIST & ParamList);        // �޽��� ���͸����� ���ε�    

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

    // LOD ���� �Լ�
    bool NewLODDistance(int Distance);                              // LOD�ܰ踦 �߰���.
    bool DeleteLODDistance(int Distance);                           // LOD�ܰ踦 �����Ѵ�.
    bool ClearLODGeometry(int Distance);                            // �ش� LOD�ܰ迡 ��ϵ� ������Ʈ�� ����Ʈ�� �ʱ�ȭ�Ѵ�.
    bool AddLODGeometry(int Distance, TKMeshGeometry *pGeometry);   // �ش� LOD�ܰ迡 ������Ʈ���� �߰��Ѵ�.
    bool ClearAllLODGeometroy(void);                                // �����ܰ踦 ������ ��� �ܰ��� ������Ʈ�� ����Ʈ�� �����Ѵ�.
    bool AddAllGeometryToInfinityLevel(void);                       // �����ܰ��� LOD������ ��� ������Ʈ���� �߰��Ѵ�.

    // ���͸��� ���� �Լ�
    TKMeshMaterial * NewMaterial(string MaterialName);            // ���ο� ���͸��� ����
    void DeleteMaterial(TKMeshMaterial * pMaterial);                // ���͸��� ����
    void DeleteMaterial(string MaterialName);                     // ���͸��� ����
    void DeleteNoReferenceMaterial(void);                           // �������� �ʴ� ���͸��� ����

    // ���ؽ����� �����Լ�
    TKMeshVertexes * NewVertexes(string NodeName, EMeshVertexesType MeshVertexesType);    // ���ο� ���ؽ��� ����.
    void DeleteVertexes(TKMeshVertexes * pMeshVertexes);                // ���ؽ��� ����.
    void DeleteNoReferenceBoneVertexes(void);                           // �������� �ʴ� �� ���ؽ� ����.
    TKMeshVertexes * GetVertexesByNodeName(const string NodeName);    // ��� �̸����� ���ؽ��� �˻��Ѵ�.

    // �ε������� �����Լ�
    TKMeshFaces * NewFaces(string NodeName, TKMeshVertexes * pVertexes); // ���ο� ���̽��� ����.
    void DeleteFaces(TKMeshFaces * pMeshFaces);                     // ���̽��� ����.
    void DeleteNoRefFaces(void);                                    // ������� �ʴ� ���̽��� ����.
    TKMeshFaces * GetFacesByNodeName(const string NodeName);      // ��� �̸����� ���̽��� �˻��Ѵ�.

    // ������Ʈ�� ���� �Լ�
    TKMeshGeometry * NewGeometry(string NodeName, TKMeshVertexes * pVertexes, TKMeshFaces * pFaces);    // �� ������Ʈ�� ����
    void DeleteGeometry(TKMeshGeometry * pGeometry);                // ������Ʈ�� ����
    void DeleteNoReferenceBone(void);                               // �������� �ʴ� Bone ������Ʈ�� ��ü�� �˻��Ͽ� ����.
    void SortGeomListByAlpha(void);                                 // ������Ʈ�� ���ĺ���/�̸��� ����
    void MoveGeomList(const int Index, TKMeshGeometry *pMeshGeom);  // ������Ʈ�� �̵�
    TKMeshGeometry * GetGeomByNodeName(const string NodeName);    // ��� �̸����� ������Ʈ�� �˻��Ѵ�.

    // AABB ���� �ּҰ�/�ִ밪.
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
    MOTIONLIST          AniMotionList;              // ��ǵ����� ����Ʈ

    int                 m_FirstFrame;               // ���� �ִϸ��̼��� ù ������ ��ȣ
    int                 m_LastFrame;                // ���� �ִϸ��̼��� ������ ������ ��ȣ
    int                 m_FrameSpeed;               // ���� �ִϸ��̼��� �ʴ� ������ ���ǵ�

    int                 m_MatrixArrayColCount;      // �ִϸ��̼� ��Ʈ���� �� ����(��� �Ҽ��ִ�)
    int                 m_MatrixArrayRowCount;      // �ִϸ��̼� ��Ʈ���� ������ ī��Ʈ
    int                 m_MatrixArrayTotalCount;    // �ִϸ��̼� ��Ʈ���� ��Ż ī��Ʈ(MatrixArrayRowCount * MatrixArrayColCount)
    D3DXMATRIX         *m_pMatrixArray;             // �ִϸ��̼� ��Ʈ���� �迭

    void Build60FPSAnimation(GEOMLIST & ParamList); // �ִϸ��̼� �������� 60���������� Ȯ��(�̱���)
    void SetAnimationMatrixIndex(void);             // �޽����� �ִϸ��̼� ��Ʈ������ �����Ͽ� ��Ʈ���� ī��Ʈ�� ����
    void SetAnimationMatrixData(void);              // �ִϸ��̼� ��Ʈ���� �迭 �����͸� ����
    void MeshAnimationBind(void);                   // �޽��� �ִϸ��̼��� ���ε�(Mesh�� �ִϸ��̼� �ε����� ä����)

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

    // �ִϸ��̼� ���� �Լ�
    pair<int, D3DXMATRIX *> GetAnimationMatrix(int CurrFrame);      // ���� �������� �ִϸ��̼� ��Ʈ�����迭 ������ ��������.
    const ST_ANIMOTION GetAniMotionByName(string MotionName);       // ����� �ִϸ��̼� ��� ������ ��������
    int GetMatArrColCnt()   { return m_MatrixArrayColCount; }       // �ִϸ��̼� ��Ʈ���� ī��Ʈ(��� �Ҽ��ִ�)
    int GetMatArrRowCnt()   { return m_MatrixArrayRowCount; }       // �ִϸ��̼� ��Ʈ���� ������ ī��Ʈ
    int GetMatArrTotalCnt() { return m_MatrixArrayTotalCount; }     // �ִϸ��̼� ��Ʈ���� ��Ż ī��Ʈ

    // FileIO �Լ�
    bool LoadLodFile(string FileName);                              // LOD ������ ���� �ε� �Լ�
    bool SaveLodFile(string FileName);                              // LOD ������ ���� ���� �Լ�
    bool LoadMotionFile(string FileName);                           // �ִϸ��̼� ��� �ε� �Լ�
    bool SaveMotionFile(string FileName);                           // �ִϸ��̼� ��� ���� �Լ�
    bool LoadFromFile(string FileName);                             // �� ������ ���� �ε� �Լ�
    bool SaveToFile(string FileName);                               // �� ������ ���� ���� �Լ�
};