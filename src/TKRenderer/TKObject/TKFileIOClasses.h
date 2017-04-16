#pragma once

#include <atlstr.h>
#include "./TKFileStruct.h"

#pragma pack(push, 1)

///////////////////////////////////////////////////////////////////////////////
//TKMAXFileIO
///////////////////////////////////////////////////////////////////////////////
class TKMAXFileIO : public TKObject
{
public:
    FILESTRUCT::MAX_SCENE               Scene;
    FILESTRUCT::MAX_MATERIAL_LIST       Material_List;
    FILESTRUCT::MAX_GEOMOBJECT_LIST     GeomObject_List;
    
public:
    TKMAXFileIO(void);
    virtual ~TKMAXFileIO(void);

    bool Clear(void);

    bool SaveToFile(const char *FileName);
    bool SaveToText(const char *FileName);
    bool LoadFromFile(const char *FileName);

    bool operator == (TKMAXFileIO & CustomParser);
};

///////////////////////////////////////////////////////////////////////////////
//TKMAXFileIO
///////////////////////////////////////////////////////////////////////////////
class TKModelFileIO : public TKObject
{
public:
    FILESTRUCT::ST_MESHRES  MeshResData;
    FILESTRUCT::ST_ANIMOTIONLIST AniMotionList;
    FILESTRUCT::ST_MESHLODLIST MeshLodList;
    
public:
    TKModelFileIO(void);
    virtual ~TKModelFileIO(void);

    bool Clear(void);

    bool SaveToFile(const char *FileName);
    bool SaveToText(const char *FileName);
    bool LoadFromFile(const char *FileName);

    bool operator == (TKModelFileIO & CustomParser);
};


///////////////////////////////////////////////////////////////////////////////
//TKMotionFileIO
///////////////////////////////////////////////////////////////////////////////
class TKMotionFileIO : public TKObject
{
public:
    FILESTRUCT::ST_ANIMOTIONLIST AniMotionList;
    
public:
    TKMotionFileIO(void);
    virtual ~TKMotionFileIO(void);

    bool Clear(void);

    bool SaveToFile(const char *FileName);
    bool SaveToText(const char *FileName);
    bool LoadFromFile(const char *FileName);

    bool operator == (TKMotionFileIO & CustomParser);
};

///////////////////////////////////////////////////////////////////////////////
//TKLODFileIO
///////////////////////////////////////////////////////////////////////////////
class TKLODFileIO : public TKObject
{
public:
    FILESTRUCT::ST_MESHLODLIST MeshLodList;
    
public:
    TKLODFileIO(void);
    virtual ~TKLODFileIO(void);

    bool Clear(void);

    bool SaveToFile(const char *FileName);
    bool SaveToText(const char *FileName);
    bool LoadFromFile(const char *FileName);

    bool operator == (TKLODFileIO & CustomParser);
};

/////////////////////////////////////////////////////////////////////////////
//TKKCTFileIO
/////////////////////////////////////////////////////////////////////////////
class TKKCTFileIO : public TKObject
{
public:
    char                        VersionInfo[SBUFSIZE];    
    FILESTRUCT::ST_COLLISION    Collision;
    
public:
    TKKCTFileIO(void);
    virtual ~TKKCTFileIO(void);

    void Clear(void);

    void SaveToFile(const char *FileName);
    void SaveToText(const char *FileName);
    void LoadFromFile(const char *FileName);
};


/////////////////////////////////////////////////////////////////////////////
//TKSGObjectFileIO
/////////////////////////////////////////////////////////////////////////////
class TKSGObjectFileIO : public TKObject
{
public:
    FILESTRUCT::ST_MESHRES          MeshResData;
    FILESTRUCT::ST_ANIMOTIONLIST    AniMotionList;
    FILESTRUCT::ST_MESHLODLIST      MeshLodList;
    FILESTRUCT::ST_COLLISION        Collision;
    
public:
    TKSGObjectFileIO(void);
    virtual ~TKSGObjectFileIO(void);

    void Clear(void);

    void SaveToFile(const char *FileName);
    void SaveToText(const char *FileName);
    void LoadFromFile(const char *FileName);
};

/////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)