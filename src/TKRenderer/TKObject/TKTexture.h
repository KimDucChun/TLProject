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

    // �ؽ��� ����Ʈ(ID����)
    TEXTURELIST_BYID TextureList_ID;

    // Ŭ���� delete �ÿ� ��� �ؽ����� ������ TextureList_ID �� �����Ͽ� ������ ���̹Ƿ�
    // BYNAME, RESOURCEID ����Ʈ�� �ؽ��Ĵ� �����ؼ��� �ȵȴ�.
    // �ؽ��� ����Ʈ(NAME����)
    TEXTURELIST_BYNAME TextureList_Name;
    // �ؽ��� ����Ʈ(���ҽ�ID����)
    TEXTURELIST_BYRESOURCEID TextureList_ResourceID;
    // �ؽ��� ����Ʈ ��ϼ������.
    vector<TKTexture *> TextureListSeq;

    string UserTextureDir;
    string SystemTextureDir;

    bool OwnerContainer;   // �� ���� True�� ��� �Ŵ��� �Ҹ�� �ؽ��ĸ� delete ���� �ʴ´�.

    // �ؽ��ĸ� ����Ʈ�� �߰�
    bool AddTexture(TKTexture *pTexture);
    // �ؽ��ĸ� ����Ʈ���� ����
    bool RemoveTexture(TKTexture *pTexture);
    bool RemoveTextureByIndex(const int Index);
    bool RemoveTextureByName(string & TextureFileName);
    bool RemoveTextureByResourceID(const int ResourceID);

public:
    TKTextureManager(TKEngine *pEngine, bool OwnerContainer );
    virtual ~TKTextureManager(void);
    
    int GenID(void);
    TKEngine* GetEngine(void) { return pEngine; }
    
    // �ؽ��ĸ� ��Ÿ ��ġ���� �޸𸮷� �ε�
    TKTexture * LoadTexture(string & TextureFileName, D3DCOLOR MaskColor = MASKCOLOR, bool SystemTexture = false);
    TKTexture * LoadTextureFromResource(int ResourceID, D3DCOLOR MaskColor = MASKCOLOR);
    TKTexture * LoadTextureFromMemory(char *MemPoint, unsigned int Length, D3DCOLOR MaskColor = MASKCOLOR);

    // �ؽ��ĸ� �޸𸮿��� ����
    bool ReleaseTexture(TKTexture *pTexture);
    bool ReleaseTextureByIndex(const int Index);
    bool ReleaseTextureByName(string & TextureFileName);
    bool ReleaseTextureByResourceID(const int ResourceID);

    // �ؽ��Ŀ� ����
    TKTexture * GetTexture(const int Index);
    TKTexture * GetTextureByName(string & TextureFileName);
    TKTexture * GetTextureByResourceID(const int ResourceID);
    TKTexture * GetTextureBySeq(const int Index);

    // �����ý��� �ؽ��� ���丮 ����
    void SetSystemTextureDir(string & SystemTextureDir);
    string & GetSystemTextureDir(void);
  
    // ������ؽ��� �⺻ ���丮 ����
    void SetUserTextureDir(string & UserTextureDir);
    string & GetUserTextureDir(void);
    
    // �ؽ��� ����
    int Count(void);
};

/////////////////////////////////////////////////////////////////////

class TKTextureList : public TKObject
{
protected:
    friend class TKEngine;

    TKCriticalSection   CS;

    TKEngine    *pEngine;

    // �ؽ��� ����Ʈ(ID����)
    TEXTURELIST_BYID TextureList_ID;
    // �ؽ��� ����Ʈ ��ϼ������.
    vector<TKTexture *> TextureListSeq;

public:
    TKTextureList(TKEngine *pEngine );
    virtual ~TKTextureList(void);

    // �ؽ��ĸ� ����Ʈ�� �߰�
    bool AddTexture(TKTexture *pTexture);
    
    // �ؽ��ĸ� ����Ʈ���� ����
    bool RemoveTexture(TKTexture *pTexture);
    bool RemoveTextureByIndex(const int Index);
    
    // �ؽ��Ŀ� ����
    TKTexture * GetTexture(const int Index);
    TKTexture * GetTextureBySeq(const int Index);

    // �ؽ��� ����
    int Count(void);

    // Ŭ����
    void Clear(void);
};