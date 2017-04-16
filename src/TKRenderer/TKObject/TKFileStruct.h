#pragma once

#include <atlstr.h>
#include "../TKObject.h"
#include "../TKContainer.h"
#include "./TKFile.h"

#pragma pack(push, 1)

namespace FILESTRUCT
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    class MAX_SCENE
    {
    public:
        char        strFileName[SBUFSIZE];
        int         FirstFrame;
        int         LastFrame;
        int         FrameSpeed;
        int         TickPerFrame;
        D3DXCOLOR   vBackGround_static;
        D3DXCOLOR   vAmbient_static;

        MAX_SCENE(void);
        MAX_SCENE(MAX_SCENE & Scene);

        void Clear(void);

        MAX_SCENE & operator = (MAX_SCENE & Scene);
        bool operator == (MAX_SCENE & Scene);
	    bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);
    };

    class MAPPING_INFO
    {
    public:
        char        MapName[SBUFSIZE];
        char        MapClass[SBUFSIZE];
        int         MapSubNo;
        float       MapAmount;
        char        MapFileName[SBUFSIZE];
        int         MapType;
        char        strMapType[SBUFSIZE];
        float       UVW_U_OffSet;
        float       UVW_V_OffSet;
        float       UVW_U_Tiling;
        float       UVW_V_Tiling;
        float       UVW_Angle;
        float       UVW_Blur;
        float       UVW_BlurOffSet;
        float       UVW_NouseAmt;
        float       UVW_NoiseSize;
        int         UVW_NoiseLevel;
        float       UVW_NoisePhase;
        int         BitmapFilter;
        char        strBitmapFilter[SBUFSIZE];

        MAPPING_INFO(void);
        MAPPING_INFO(MAPPING_INFO & Mapping_Info);
        ~MAPPING_INFO(void);

        MAPPING_INFO & operator = (MAPPING_INFO & Mapping_Info);
        bool operator == (MAPPING_INFO & Mapping_Info);
	    bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);
    };

    enum EMAXMaterialMappingCount
    {
        emmcAmbient,
        emmcDiffuse,
        emmcSpecular,
        emmcShine,
        emmcShineStrength,
        emmcSelfIllum,
        emmcOpacity,
        emmcFilterColor,
        emmcBump,
        emmcReflect,
        emmcRefract,
        emmcGeneric
    };

    class MAX_MATERIALINFO;

    // 얘 원래 맵이었음...인덱스 순으로 소팅해줘야 함.
    typedef TKArrayList<MAX_MATERIALINFO> MAX_MATERIALINFOS;

    class MAX_MATERIALINFO
    {
    public:
        int             Index;
        char            MaterialName[SBUFSIZE];
        char            MaterialClass[SBUFSIZE];
        D3DXCOLOR       MAmbient;
        D3DXCOLOR       MDiffuse;
        D3DXCOLOR       MSpecular;
        float           Shine;
        float           ShineStrength;
        float           Transparency;
        float           WireSize;
        int             Shading;
        char            strShading[SBUFSIZE];
        float           XP_FallOff;
        float           Selfillum;
        bool            TwoSided;
        bool            AlphaBlending;
        char            FallOff[SBUFSIZE];
        int             XPType;
        char            strXPType[SBUFSIZE];

        MAPPING_INFO    MappingInfo[MATERIAL_TEXTURE_COUNT];

        MAX_MATERIALINFOS   Items;

        MAX_MATERIALINFO(void);
        MAX_MATERIALINFO(MAX_MATERIALINFO & Material_Info);
        ~MAX_MATERIALINFO(void);

        void AddItem(MAX_MATERIALINFO & Material_Info);
        void DeleteItem(int Index);
        void Clear(void);

        MAX_MATERIALINFO & operator = (MAX_MATERIALINFO & Material_Info);
        bool operator == (MAX_MATERIALINFO & Material_Info);

        bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);
    };

    int MAX_MATERIALINFO_COMPARE(MAX_MATERIALINFO *, MAX_MATERIALINFO *);

    class MAX_MATERIAL_LIST
    {
    public:
        MAX_MATERIAL_LIST(void);
        MAX_MATERIAL_LIST(MAX_MATERIAL_LIST & Max_Material_List);
        ~MAX_MATERIAL_LIST(void);

        MAX_MATERIALINFOS   Items;

        void AddItem(MAX_MATERIALINFO & Material_Info);
        void DeleteItem(int Index);
        void Clear(void);

        bool operator == (MAX_MATERIAL_LIST & Max_Material_List);
	    bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    class MAX_NODE_TM
    {
    public:
        char        NodeName[SBUFSIZE];
        INT_VECTOR3 InheritPos;
        INT_VECTOR3 InheritRot;
        INT_VECTOR3 InheritScl;
        D3DXVECTOR3 Tm_Row0;
        D3DXVECTOR3 Tm_Row1;
        D3DXVECTOR3 Tm_Row2;
        D3DXVECTOR3 Tm_Row3;
        D3DXVECTOR3 Tm_Pos;
        D3DXVECTOR3 Tm_RotAxis;
        float       Tm_RotAngle;
        D3DXVECTOR3 Tm_Scale;
        D3DXVECTOR3 Tm_ScaleAxis;
        float       Tm_ScaleAxisAng;

        MAX_NODE_TM(void);
        MAX_NODE_TM(MAX_NODE_TM & Node_Tm);
        ~MAX_NODE_TM(void);

        MAX_NODE_TM & operator = (MAX_NODE_TM & Node_Tm);
        bool operator == (MAX_NODE_TM & Node_Tm);

	    bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);

        void Clear(void);
    };

    class MAX_MATRIX_WEIGHT
    {
    public:
        char    ParentNodeName[SBUFSIZE];
        float   Weight;
            
        MAX_MATRIX_WEIGHT(void);
        MAX_MATRIX_WEIGHT(const MAX_MATRIX_WEIGHT & Matrix_Weight);

        const MAX_MATRIX_WEIGHT & operator = (const MAX_MATRIX_WEIGHT & Matrix_Weight);
        bool operator == (MAX_MATRIX_WEIGHT & Matrix_Weight);

        void Clear(void);
    };

    typedef vector<MAX_MATRIX_WEIGHT> MAX_MATRIX_WEIGHTS;

    bool MAX_MATRIX_WEIGHT_COMPARE(MAX_MATRIX_WEIGHT & A, MAX_MATRIX_WEIGHT & B);

    class MAX_VERTEX
    {
        void MatrixWeightReAllocate(void);
    public:
        float x, y, z;
        MAX_MATRIX_WEIGHTS MatrixWeights;

        MAX_VERTEX(void);
        MAX_VERTEX(const MAX_VERTEX & Max_Vertex);

        const MAX_VERTEX & operator = (const MAX_VERTEX & Max_Vertex);
        bool operator == (MAX_VERTEX & Max_Vertex);

	    bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);

        void AddWeight(MAX_MATRIX_WEIGHT & MatrixWeight);

        void Clear(void);
    };
    typedef vector<MAX_VERTEX> MAX_VERTEXS;

    class MAX_MESH_FACE
    {
    public:
        int         Index;
        int         a;
        int         b;
        int         c;
        int         MtLid;

        MAX_MESH_FACE(void);
        MAX_MESH_FACE(const MAX_MESH_FACE & Mesh_Face);

        const MAX_MESH_FACE & operator = (const MAX_MESH_FACE & Mesh_Face);
	    bool operator == (MAX_MESH_FACE & Mesh_Face);
        bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);
    };
    typedef vector<MAX_MESH_FACE> MAX_MESH_FACES;

    class MAX_MESH
    {
        void    CreateOBB(void);
        void    NormalSmoothing(void);

    public:
        MAX_VERTEXS         PosWeightList;      // 버텍스/버텍스가중치
        D3DXVECTOR3S        TexPosList;         // 텍스쳐POS
        D3DXVECTOR3S        ColorList;          // 버텍스칼라
        D3DXVECTOR3S        NormalList;         // 버텍스노말
        MAX_MESH_FACES      FaceList;           // 페이스리스트
        
        D3DXVECTOR3         vecCenter;
        D3DXVECTOR3         vecAxis[3];
        float               AxisLen[3];
        D3DXVECTOR3         vecOBB[8];
        D3DXVECTOR3         vecMin;
        D3DXVECTOR3         vecMax;

        MAX_MESH(void);
        MAX_MESH(MAX_MESH & Mesh);
        ~MAX_MESH(void);

        MAX_MESH & operator = (MAX_MESH & Mesh);
        bool operator == (MAX_MESH & Mesh);
        bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);

        void Add_PosWeight  (MAX_VERTEX      & PosWeight    );
        void Add_TexPos     (D3DXVECTOR3     & TexPos       );
        void Add_Color      (D3DXVECTOR3     & Color        );
        void Add_Normal     (D3DXVECTOR3     & Normal       );

        void Add_Face       (MAX_MESH_FACE   & Face         );

        void Clear(void);
    };
    typedef TKArrayList<MAX_MESH> MAX_MESHS;

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    class MAX_CONTROL_POS
    {
    public:
        int         TimeValue;
        float       x;
        float       y;
        float       z;

        MAX_CONTROL_POS(void);
        MAX_CONTROL_POS(MAX_CONTROL_POS & Control_Pos);

        MAX_CONTROL_POS & operator = (MAX_CONTROL_POS & Control_Pos);
	    bool operator == (MAX_CONTROL_POS & Control_Pos);
        bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);
    };
    typedef vector<MAX_CONTROL_POS> CONTROL_POS_LIST;

    class MAX_CONTROL_ROT
    {
    public:
        int         TimeValue;
        float       x;
        float       y;
        float       z;
        float       w;

        MAX_CONTROL_ROT(void);
        MAX_CONTROL_ROT(MAX_CONTROL_ROT & Control_Rot);

        MAX_CONTROL_ROT & operator = (MAX_CONTROL_ROT & Control_Rot);
	    bool operator == (MAX_CONTROL_ROT & Control_Rot);
        bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);
    };
    typedef vector<MAX_CONTROL_ROT> CONTROL_ROT_LIST;

    class MAX_CONTROL_SCALE
    {
    public:
        int         TimeValue;
        float       x;
        float       y;
        float       z;
        float       AxisX;
        float       AxisY;
        float       AxisZ;
        float       Radian;

        MAX_CONTROL_SCALE(void);
        MAX_CONTROL_SCALE(MAX_CONTROL_SCALE & Control_Scale);

        MAX_CONTROL_SCALE & operator = (MAX_CONTROL_SCALE & Control_Scale);
	    bool operator == (MAX_CONTROL_SCALE & Control_Scale);
        bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);
    };
    typedef vector<MAX_CONTROL_SCALE> CONTROL_SCALE_LIST;

    class MAX_ANIMATRIX
    {
    public:
        int         TimeValue;
        D3DXMATRIX  matAni;

        MAX_ANIMATRIX(void);
        MAX_ANIMATRIX(const MAX_ANIMATRIX & AniMatrix);

        const MAX_ANIMATRIX & operator = (const MAX_ANIMATRIX & AniMatrix);
	    bool operator == (MAX_ANIMATRIX & AniMatrix);
        bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);
    };
    typedef vector<MAX_ANIMATRIX> ANIMATRIX_LIST;

    class MAX_TM_ANIMATION
    {
    public:
        ANIMATRIX_LIST  AniMatrix_List;

        MAX_TM_ANIMATION(void);
        MAX_TM_ANIMATION(MAX_TM_ANIMATION & Tm_Animation);
        ~MAX_TM_ANIMATION(void);

        MAX_TM_ANIMATION & operator = (MAX_TM_ANIMATION & Tm_Animation);
        bool operator == (MAX_TM_ANIMATION & Tm_Animation);
        bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);

        void Add_AniMatrix     (MAX_ANIMATRIX     &AniMatrix    );

        void Clear(void);
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    enum EGeomObjectType
    {
        egotBone,
        egotMesh,
        egotCollisionFace,
        egotCollisionOBB
    };

    class MAX_GEOMOBJECT
    {
    public:
        int                 Index;

        char                NodeName[SBUFSIZE];
        char                ParentNodeName[SBUFSIZE];

        MAX_NODE_TM         Node_Tm;
        MAX_MESH            Mesh;
        int                 MotionBlur;
        int                 CastShadow;
        int                 RecvShadow;
        MAX_TM_ANIMATION    Tm_Animation;
        D3DXCOLOR           WireframeColor;
       
        D3DXVECTOR3         BoundingBox_Min;
        D3DXVECTOR3         BoundingBox_Max;

        int                 Material_Reference;
        int                 SubMaterial_Reference;
        char                MaterialName[SBUFSIZE];
        char                SubMaterialName[SBUFSIZE];

        EGeomObjectType     GeomObjectType;

        MAX_GEOMOBJECT(void);
        MAX_GEOMOBJECT(MAX_GEOMOBJECT & GeomObject);
        ~MAX_GEOMOBJECT(void);

        MAX_GEOMOBJECT & operator = (MAX_GEOMOBJECT & GeomObject);
        bool operator == (MAX_GEOMOBJECT & GeomObject);
        
        bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);

        void Clear(void);

        void BindMaterialList(MAX_MATERIAL_LIST & MaterialList);
    };
    typedef TKArrayList<MAX_GEOMOBJECT> MAX_GEOMOBJECTS;

    class MAX_GEOMOBJECT_LIST
    {
    public:
        MAX_GEOMOBJECT_LIST(void);
        ~MAX_GEOMOBJECT_LIST(void);

        MAX_GEOMOBJECTS Items;

        void AddItem(MAX_GEOMOBJECT & GeomObject);
        void DeleteItem(int Index);
        void Clear(void);

        bool operator == (MAX_GEOMOBJECT_LIST & Max_GeomObject_List);
	    bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);

        void BindMaterialList(MAX_MATERIAL_LIST & MaterialList);
    };
    typedef TKArrayList<MAX_GEOMOBJECT_LIST> MAX_GEOMOBJECT_LISTS;

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    class ST_MESHLODINFO
    {
    public:
        int     Level;
        char    NodeName[TINYSBUFSIZE];

        ST_MESHLODINFO(void);
        ST_MESHLODINFO(const ST_MESHLODINFO & St_MeshLodInfo);

        const ST_MESHLODINFO & operator = (const ST_MESHLODINFO & St_MeshLodInfo);
	    bool operator == (ST_MESHLODINFO & St_MeshLodInfo);
    };

    typedef vector<ST_MESHLODINFO> ST_MESHLODINFOS;

    class ST_MESHLODLIST
    {
    public:
        ST_MESHLODLIST(void);
        ~ST_MESHLODLIST(void);

        ST_MESHLODINFOS    Items;

        void AddItem(ST_MESHLODINFO & St_MeshLodInfo);
        void DeleteItem(int Index);
        void Clear(void);

        bool operator == (ST_MESHLODLIST & St_MeshLodList);
	    bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);    
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    class ST_ANIMOTIONINFO
    {
    public:
        int     Index;
        int     StartFrame;
        int     EndFrame;
        char    AnimationName[TINYSBUFSIZE];

        ST_ANIMOTIONINFO(void);
        ST_ANIMOTIONINFO(const ST_ANIMOTIONINFO & St_AniMationInfo);

        const ST_ANIMOTIONINFO & operator = (const ST_ANIMOTIONINFO & St_AniMationInfo);
	    bool operator == (ST_ANIMOTIONINFO & St_AniMationInfo);
        bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);
    };
    typedef vector<ST_ANIMOTIONINFO> ST_ANIMOTIONINFOS;

    class ST_ANIMOTIONLIST
    {
    public:
        ST_ANIMOTIONLIST(void);
        ~ST_ANIMOTIONLIST(void);

        ST_ANIMOTIONINFOS Items;

        void AddItem(ST_ANIMOTIONINFO & St_AniMationInfo);
        void DeleteItem(int Index);
        void Clear(void);

        bool operator == (ST_ANIMOTIONLIST & St_AniMotionList);
	    bool SaveToFile(TKFile *pFile);
        bool SaveToText(TKFile *pTextFile);
        bool LoadFromFile(TKFile *pFile);
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // 충돌데이터 파일 구조체
    struct ST_COLLISION
    {
    public:
        ETKObjectType   ObjectType;

        char NodeName[TINYSBUFSIZE];
        char BoneNodeName[TINYSBUFSIZE];
        
        D3DXVECTOR3S     PosList;
        D3DXVECTOR3S     NormalList;
        vector<ST_IB_TRIANGLE>  FaceList;
        D3DXVECTOR3S     FaceNormalList;

        D3DXVECTOR3 Center;
        D3DXVECTOR3 Min;
        D3DXVECTOR3 Max;
        float       Radius;

        float       AxisLen[3];         // OBB에만 있음
        D3DXVECTOR3 vecAxisDir[3];      // OBB에만 있음
        D3DXVECTOR3 vecOBB[8];          // OBB에만 있음
        
        bool    IsDynamicObject;        // TKCollisionObjectRes에만 있음
        bool    IsMeshAnimation;        // TKCollisionObjectRes에만 있음

        vector<ST_COLLISION*> Items;

    public:
        ST_COLLISION(void);
        ~ST_COLLISION(void);

        void Clear(void);

	    void SaveToFile(TKFile *pFile);
        void SaveToText(TKFile *pTextFile);
        void LoadFromFile(TKFile *pFile);
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    // 메쉬 파일 구조체
    struct ST_MATERIAL;
    typedef vector<ST_MATERIAL*> ST_MATERIALS;
    struct ST_MATERIAL
    {
    public:
        // 멤버 순서 변경하지 말것.
        char            NodeName[TINYSBUFSIZE];
        char            TextureName[MATERIAL_TEXTURE_COUNT][TINYSBUFSIZE];
        bool            AlphaBlend;
		D3DBLEND        AlphaBlend_Src;
        D3DBLEND        AlphaBlend_Dest;
        D3DBLENDOP      AlphaBlend_OP;
		bool            AlphaTest;
		D3DCMPFUNC      AlphaTestFunc;
        DWORD           AlphaTestRef;
        bool            TwoSided;
        bool            ZWriteEnable;
        
        ST_MATERIALS    SubMaterialList;
    public:    
        ST_MATERIAL(void);
        ~ST_MATERIAL(void);
        
        void Clear(void);

	    void SaveToFile(TKFile *pFile);
        void SaveToText(TKFile *pTextFile);
        void LoadFromFile(TKFile *pFile);
    };

    struct ST_GEOMETRY
    {
    public:
        struct ST_VERTEXWEIGHT
        {
            char                        ParentNode[TINYSBUFSIZE];
            float                       Weight;
        };
        struct ST_VERTEX
        {
            D3DXVECTOR3                 Pos;
            ST_VERTEXWEIGHT             Weights[4];
        };
        
    public:
        // 멤버 순서 변경하지 말것.
        char                            NodeName[TINYSBUFSIZE];     // 객체이름
        char                            BoneNodeName[TINYSBUFSIZE]; // 애니메이션 참조 본 객체명
        int                             AnimationMatrixIndex;       // 애니메이션 인덱스
        D3DXVECTOR3                     vecCenter;                  // 바운딩박스의 중점
        D3DXVECTOR3                     vecAxis[3];                 // 바운딩박스의 고유축
        float                           AxisLen[3];                 // 고유축 길이
        D3DXVECTOR3                     vecOBB[8];                  // OBB를 정의하는 점 8개
        D3DXVECTOR3                     vecMin;                     // AABB 기준 최소값
        D3DXVECTOR3                     vecMax;                     // AABB 기준 최대값
        EMeshVertexesType               MeshVertexesType;           // 지오메트리 타입
        char                            MaterialName[TINYSBUFSIZE];     // 자신 객체의 메터리얼 이름
        char                            SubMaterialName[TINYSBUFSIZE];  // 자신 객체의 서브메터리얼 이름
        char                            ShaderName[TINYSBUFSIZE];       // 쉐이더 파일 이름

        vector<ST_VERTEX>               OriginPosList;
        D3DXVECTOR3S                    PosList;            // 버텍스 Pos 리스트
        vector<D3DXVECTOR4>             MatrixWeightList;   // 버텍스당 매트릭스 애니메이션 가중치
        vector<ST_MESHMATRIXINDEX>      MatrixIndexList;    // 버텍스당 매트릭스 애니메이션 인덱스
        vector<D3DXVECTOR2>             TexPosList;         // 텍스쳐 좌표 리스트
        vector<D3DCOLOR>                ColorList;          // 버텍스 칼라 리스트
        D3DXVECTOR3S                    NormalList;         // 버텍스 노말 정보
        vector<ST_IB_TRIANGLE>          FaceList;           // 페이스 정보
        vector<ST_ANIMATRIX>            AniMatrixList;      // 애니메이션 매트릭스 리스트   
            
    public:
        ST_GEOMETRY(void);
        ~ST_GEOMETRY(void);
        
        void Clear(void);

	    void SaveToFile(TKFile *pFile);
        void SaveToText(TKFile *pTextFile);
        void LoadFromFile(TKFile *pFile);   
    };
    typedef vector<ST_GEOMETRY*> ST_GEOMETRIES;

    struct ST_MESHRES
    {
    public:
        char    NodeName[TINYSBUFSIZE];

        int     FirstFrame;
        int     LastFrame;
        int     FrameSpeed;

        ST_MATERIALS    MaterialList;
        ST_GEOMETRIES   GeomList;
        ST_GEOMETRIES   BoneList;
        
    public:
        ST_MESHRES(void);
        ~ST_MESHRES(void);
        
        void Clear(void);

	    void SaveToFile(TKFile *pFile);
        void SaveToText(TKFile *pTextFile);
        void LoadFromFile(TKFile *pFile);       
    };
}

#pragma pack(pop)