#pragma once

#include "../TKObject.h"
#include "./TKThread.h"

class TKTextureManager;
class TKTextureList;
class TKTexture;

typedef map <int, TKTexture *> TEXTURELIST_BYID;
typedef map <string, TKTexture *> TEXTURELIST_BYNAME;
typedef map <int, TKTexture *> TEXTURELIST_BYRESOURCEID;

/////////////////////////////////////////////////////////////////////

class TKTexture : public TKObject
{
private:
    friend class TKEngine;
    friend class TKTextureManager;
    friend class TKTextureList;

    int UseCount;

    TKEngine            *pEngine;
    TKTextureManager    *pTextureManager;
#ifdef TK_D3D9
	IDirect3DTexture9   *pID3DTexture;
    D3DSURFACE_DESC     ImageInfo;
#endif
    
    D3DCOLOR MaskColorCode;
    string TextureFileName;
    int  ResourceID;
    int  Index;

    bool LoadFromFile(const int Index, const string & TextureFileName, const D3DCOLOR MaskColorCode, bool SystemTexture);
    bool LoadFromResource(const int Index, const int ResourceID, const D3DCOLOR MaskColorCode);
    bool LoadFromMemory(const int Index, char *MemPoint, unsigned int Length, const D3DCOLOR MaskColorCode);

    void IncUseCount(void);
    void DecUseCount(void);
    int GetUseCount(void);

    TKTexture(TKEngine *pEngine, TKTextureManager *pTextureManager);
    virtual ~TKTexture();
public:
    TKEngine* GetEngine(void) { return pEngine; }

    string & GetTextureFileName(void);
    int GetResourceID(void);
    int GetImageWidth(void);
    int GetImageHeight(void);
    int GetIndex(void);
#ifdef TK_D3D9
	const D3DSURFACE_DESC & GetSurface_Desc(void)	{	return ImageInfo;	}
	IDirect3DTexture9 *GetTexture(void)				{	return pID3DTexture;	}
#endif
};

/////////////////////////////////////////////////////////////////////

class TKTextureManager : public TKObject
{
protected:
    friend class TKEngine;

    TKCriticalSection   CS;

    int         LastTextureID;
    TKEngine    *pEngine;

    // 텍스쳐 리스트(ID정렬)
    TEXTURELIST_BYID TextureList_ID;

    // 클래스 delete 시에 모든 텍스쳐의 해제는 TextureList_ID 를 참고하여 해제할 것이므로
    // BYNAME, RESOURCEID 리스트의 텍스쳐는 삭제해서는 안된다.
    // 텍스쳐 리스트(NAME정렬)
    TEXTURELIST_BYNAME TextureList_Name;
    // 텍스쳐 리스트(리소스ID정렬)
    TEXTURELIST_BYRESOURCEID TextureList_ResourceID;
    // 텍스쳐 리스트 등록순서대로.
    vector<TKTexture *> TextureListSeq;

    string UserTextureDir;
    string SystemTextureDir;

    bool OwnerContainer;   // 이 값이 True인 경우 매니저 소멸시 텍스쳐를 delete 하지 않는다.

    // 텍스쳐를 리스트에 추가
    bool AddTexture(TKTexture *pTexture);
    // 텍스쳐를 리스트에서 제거
    bool RemoveTexture(TKTexture *pTexture);
    bool RemoveTextureByIndex(const int Index);
    bool RemoveTextureByName(string & TextureFileName);
    bool RemoveTextureByResourceID(const int ResourceID);

public:
    TKTextureManager(TKEngine *pEngine, bool OwnerContainer );
    virtual ~TKTextureManager(void);
    
    int GenID(void);
    TKEngine* GetEngine(void) { return pEngine; }
    
    // 텍스쳐를 기타 장치에서 메모리로 로딩
    TKTexture * LoadTexture(string & TextureFileName, D3DCOLOR MaskColor = MASKCOLOR, bool SystemTexture = false);
    TKTexture * LoadTextureFromResource(int ResourceID, D3DCOLOR MaskColor = MASKCOLOR);
    TKTexture * LoadTextureFromMemory(char *MemPoint, unsigned int Length, D3DCOLOR MaskColor = MASKCOLOR);

    // 텍스쳐를 메모리에서 해제
    bool ReleaseTexture(TKTexture *pTexture);
    bool ReleaseTextureByIndex(const int Index);
    bool ReleaseTextureByName(string & TextureFileName);
    bool ReleaseTextureByResourceID(const int ResourceID);

    // 텍스쳐에 접근
    TKTexture * GetTexture(const int Index);
    TKTexture * GetTextureByName(string & TextureFileName);
    TKTexture * GetTextureByResourceID(const int ResourceID);
    TKTexture * GetTextureBySeq(const int Index);

    // 엔진시스템 텍스쳐 디렉토리 정의
    void SetSystemTextureDir(string & SystemTextureDir);
    string & GetSystemTextureDir(void);
  
    // 사용자텍스쳐 기본 디렉토리 정의
    void SetUserTextureDir(string & UserTextureDir);
    string & GetUserTextureDir(void);
    
    // 텍스쳐 갯수
    int Count(void);
};

/////////////////////////////////////////////////////////////////////

class TKTextureList : public TKObject
{
protected:
    friend class TKEngine;

    TKCriticalSection   CS;

    TKEngine    *pEngine;

    // 텍스쳐 리스트(ID정렬)
    TEXTURELIST_BYID TextureList_ID;
    // 텍스쳐 리스트 등록순서대로.
    vector<TKTexture *> TextureListSeq;

public:
    TKTextureList(TKEngine *pEngine );
    virtual ~TKTextureList(void);

    // 텍스쳐를 리스트에 추가
    bool AddTexture(TKTexture *pTexture);
    
    // 텍스쳐를 리스트에서 제거
    bool RemoveTexture(TKTexture *pTexture);
    bool RemoveTextureByIndex(const int Index);
    
    // 텍스쳐에 접근
    TKTexture * GetTexture(const int Index);
    TKTexture * GetTextureBySeq(const int Index);

    // 텍스쳐 갯수
    int Count(void);

    // 클리어
    void Clear(void);
};