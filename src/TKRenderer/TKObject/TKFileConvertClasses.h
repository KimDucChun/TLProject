#pragma once

#include "TKFileIOClasses.h"
#include "TKMeshResource.h"
#include "TKCollisionResource.h"

using namespace KContainer;

class TKBaseObjectList;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TKCustomFileConverter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TKCustomFileConverter : TKObject
{
protected:
    TKEngine                    *pEngine;
    TKAniMeshRes                *pAniMeshRes;
    TKCollisionObjectRes        *pCollisionRes;
    ECollisionObjectType        CollisionObjectType;
    ORIGINCOLLISIONLIST         OriginCollisionList;

    // Max Model File Converter 
    bool ExtractMaterialListFromMAX    (FILESTRUCT::MAX_MATERIALINFOS *pSrcList, TKMeshMaterial *pParentMaterial);

    bool ExtractBoneListFromMAX        (FILESTRUCT::MAX_GEOMOBJECTS *pSrcList);
    bool ExtractGeomListFromMAX        (FILESTRUCT::MAX_GEOMOBJECTS *pSrcList);
    bool ExtractGeometryFromMAX        (FILESTRUCT::MAX_GEOMOBJECT *pSrcGeom, EMeshVertexesType MeshVertexesType);

    bool ExtractBoneMeshOriginVertexListFromMAX(FILESTRUCT::MAX_VERTEXS *pSrcList, vector<ST_GEOM_VERTEX> *pDestList, string & ParentNodeName);

    bool ExtractOriginVertexListFromMAX(FILESTRUCT::MAX_VERTEXS *pSrcList, vector<ST_GEOM_VERTEX> *pDestList);
    bool ExtractVertexListFromMAX      (FILESTRUCT::MAX_VERTEXS *pSrcList, D3DXVECTOR3S *pDestList);
    bool ExtractTexelListFromMAX       (D3DXVECTOR3S *pSrcList, vector<D3DXVECTOR2> *pDestList);
    bool ExtractVertexColorListFromMAX (D3DXVECTOR3S *pSrcList, vector<D3DCOLOR> *pDestList);
    bool ExtractNormalListFromMAX      (D3DXVECTOR3S *pSrcList, D3DXVECTOR3S *pDestList);

    bool ExtractFaceListFromMAX (FILESTRUCT::MAX_MESH_FACES *pSrcList, vector<ST_IB_TRIANGLE> *pDestList);

    bool ExtractAniMatrixListFromMAX(FILESTRUCT::ANIMATRIX_LIST *pSrcList, vector<ST_ANIMATRIX> *pDestList);
    bool ExtractAniMotionFromMAX(FILESTRUCT::ST_ANIMOTIONINFOS *pSrcList, MOTIONLIST *pDestList);

    // Max Collision File Converter
    bool ExtractCollisionListFromMAX(FILESTRUCT::MAX_GEOMOBJECTS    *pSrcList   );
    bool ExtractCollisionFromMAX       (FILESTRUCT::MAX_GEOMOBJECT     *pSrcGeom   );

    // Model File Converter
    template <typename OWNER> void ExtractMaterialFromFile(OWNER *pDestOwner, FILESTRUCT::ST_MATERIALS *pSrcList);
    template <typename OWNER> void ExtractMaterialToFile(FILESTRUCT::ST_MATERIALS *pDestList, OWNER *pSrcOwner);
    //void ExtractMaterialFromFile(MATERIALLIST *pDestList, ST_MATERIALS *pSrcList);
    //void ExtractMaterialToFile(FILESTRUCT::ST_MATERIALS *pDestList, MATERIALLIST *pSrcList);
    
    void ExtractGeometryFromFile(FILESTRUCT::ST_GEOMETRIES *pSrcList );
    void ExtractGeometryToFile(FILESTRUCT::ST_GEOMETRIES *pDestList, GEOMLIST *pSrcList );
    
    void ExtractLODDataFromFile(FILESTRUCT::ST_MESHLODINFOS *pSrcList);
    void ExtractLODDataToFile(FILESTRUCT::ST_MESHLODINFOS *pDestList, TKMeshRes *pSrcRes);
    
    void ExtractAniMotionFromFile(FILESTRUCT::ST_ANIMOTIONINFOS *pSrcList);
    void ExtractAniMotionToFile(FILESTRUCT::ST_ANIMOTIONINFOS *pDestList, MOTIONLIST *pSrcList);
    
    // Collision File Converter
    bool ExtractCollisionFromFile(FILESTRUCT::ST_COLLISION *pSrcData, TKCollisionResource   *pDestData);
    bool ExtractCollisionToFile(TKCollisionResource *pSrcData  , FILESTRUCT::ST_COLLISION *pDestData);

    // Bind
    bool BindMeshAndMaterial    (void);
    // 로딩된 LOD의 적합성 검사
    bool CheckValidateIOData(FILESTRUCT::ST_MESHLODINFOS *pSrcList);
    
public:
    TKCustomFileConverter(void);
    virtual ~TKCustomFileConverter(void);
    
    TKEngine* GetEngine(void) { return pEngine; }
};



/////////////////////////////////////////////////////////////////////
// TKMAXFileConverter
class TKMAXFileConverter : public TKCustomFileConverter
{
private:
    TKMAXFileIO     MAXFileIO;
public:
    TKMAXFileConverter(void);
    virtual ~TKMAXFileConverter(void);

    bool ConvertFromKMF(string & FileName, TKAniMeshRes *pAniMeshRes);
};

/////////////////////////////////////////////////////////////////////
// TKModelFileConverter
class TKModelFileConverter : public TKCustomFileConverter
{
private:
    TKModelFileIO   ModelFileIO;
public:
    TKModelFileConverter(void);
    virtual ~TKModelFileConverter(void);

    bool ConvertFromKMT(string & FileName, TKAniMeshRes *pAniMeshRes);
    bool ConvertToKMT(string & FileName, TKAniMeshRes *pAniMeshRes);
};

/////////////////////////////////////////////////////////////////////
// TKLODFileConverter
class TKLODFileConverter : public TKCustomFileConverter
{
private:
    TKLODFileIO     LODFileIO;
public:
    TKLODFileConverter(void);
    virtual ~TKLODFileConverter(void);

    bool ConvertFromKLF(string & FileName, TKAniMeshRes *pAniMeshRes);
    bool ConvertToKLF(string & FileName, TKAniMeshRes *pAniMeshRes);
};

/////////////////////////////////////////////////////////////////////
// TKMotionFileConverter
class TKMotionFileConverter : public TKCustomFileConverter
{
    TKMotionFileIO  MotionFileIO;

public:
    TKMotionFileConverter(void);
    virtual ~TKMotionFileConverter(void);

    bool ConvertFromKNF(string & FileName, TKAniMeshRes *pAniMeshRes);
    bool ConvertToKNF(string & FileName, TKAniMeshRes *pAniMeshRes);
};

/////////////////////////////////////////////////////////////////////////////////
//TKCollisionFileConverter
/////////////////////////////////////////////////////////////////////////////////
class TKCollisionFileConverter : public TKCustomFileConverter
{
private:
    TKMAXFileIO                 MAXFileIO;
    TKKCTFileIO                 KCTFileIO;

public:
    TKCollisionFileConverter(void);
    virtual ~TKCollisionFileConverter(void);

    bool ConvertFromKCF(string & FileName,
                        TKCollisionObjectRes *pCollisionRes,
                        ECollisionObjectType CollisionObjectType);

    bool ConvertFromKCT(string & FileName,
                        TKCollisionObjectRes *pCollisionRes);
                        
    bool ConvertToKCT(  string & FileName,
                        TKCollisionObjectRes *pCollisionRes);                        
};

/////////////////////////////////////////////////////////////////////////////////
//TKSGObjectFileConverter
/////////////////////////////////////////////////////////////////////////////////
class TKSGObjectFileConverter : public TKCustomFileConverter
{
private:
    TKSGObjectFileIO    SGObjectFileIO;
public:
    TKSGObjectFileConverter(void);
    virtual ~TKSGObjectFileConverter(void);
    
    pair<TKMeshRes *, TKCollisionObjectRes *>ConvertFromKSG
        ( string & FileName );

    bool ConvertToKSG(  string & FileName, 
                        TKAniMeshRes *pAniMeshRes,
                        TKCollisionObjectRes *pCollisionRes);
};


/////////////////////////////////////////////////////////////////////////////////

#include "./TKFileCOnvertClasses.hpp"
