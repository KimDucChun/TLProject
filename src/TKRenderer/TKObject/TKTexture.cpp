#include "TKTexture.h"
#include "TKEngine.h"
#include "TKD3D.h"

/////////////////////////////////////////////////////////////////////

TKTexture::TKTexture(TKEngine *pEngine, TKTextureManager *pTextureManager)
:TKObject()
{
    this->pEngine = pEngine;
    this->pTextureManager = pTextureManager;

#ifdef TK_D3D9
	pID3DTexture = NULL;
#endif
    UseCount = 0;
    Index = -1;
    ResourceID = -1;
}

TKTexture::~TKTexture(void)
{
#ifdef TK_D3D9
	SAFERELEASE(pID3DTexture);
#endif
}

bool TKTexture::LoadFromFile(const int Index, const string & TextureFileName, const D3DCOLOR MaskColorCode, bool SystemTexture)
{
#ifdef TK_D3D9
	string TempFileName;
    this->Index = Index;
    this->TextureFileName = TextureFileName;
    this->MaskColorCode = MaskColorCode;
    
    if ( SystemTexture )
    {
        TempFileName = pTextureManager->GetSystemTextureDir() + TextureFileName;
    }
    else
    {
        TempFileName = pTextureManager->GetUserTextureDir() + TextureFileName;
    }    
    
    ZeroMemory(&ImageInfo, sizeof(ImageInfo));

    if (FAILED(D3DXCreateTextureFromFileEx( R_D3DDEVICE,
                                            TempFileName.data(),
                                            D3DX_DEFAULT,
                                            D3DX_DEFAULT,
                                            D3DX_DEFAULT,
                                            //D3DX_FROM_FILE,
                                            0,
                                            D3DFMT_UNKNOWN, //D3DFMT_FROM_FILE, //
                                            D3DPOOL_MANAGED,
                                            //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
                                            //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
                                            D3DX_DEFAULT,   //D3DX_FILTER_LINEAR,
                                            D3DX_DEFAULT,   //D3DX_FILTER_LINEAR,
                                            MaskColorCode,     // 투명컬러키
                                            NULL,
                                            NULL,
                                            &pID3DTexture )))
    {
        pID3DTexture = NULL;
        MessageBox(0,  TempFileName.data(), "File Load Error", MB_ICONERROR);
        _ASSERT( false );
        return false;
    }
    pID3DTexture->GetLevelDesc(0, &ImageInfo);
#endif
    return true;
}

bool TKTexture::LoadFromResource(const int Index, int ResourceID, const D3DCOLOR MaskColorCode)
{
#ifdef TK_D3D9
	this->Index = Index;
    this->ResourceID = ResourceID;
    if (FAILED(D3DXCreateTextureFromResourceEx(
                    R_D3DDEVICE,
                    (HINSTANCE)pEngine->GethInstance(),
                    MAKEINTRESOURCE(ResourceID),
                    D3DX_DEFAULT,
                    D3DX_DEFAULT,
                    D3DX_DEFAULT,
                    0,
                    D3DFMT_UNKNOWN,
                    D3DPOOL_MANAGED,
                    D3DX_FILTER_LINEAR,//D3DX_DEFAULT,
                    D3DX_FILTER_LINEAR,//D3DX_DEFAULT,
                    MaskColorCode,     // 투명컬러키
                    NULL,
                    NULL,
                    &pID3DTexture
                )))
    {
        MessageBox(0,  "", "Image Load Error", MB_ICONERROR);
        return false;
    }
    pID3DTexture->GetLevelDesc(0, &ImageInfo);
#endif
    return true;
}

bool TKTexture::LoadFromMemory(const int Index, char *MemPoint, unsigned int Length, const D3DCOLOR MaskColorCode)
{
#ifdef TK_D3D9
	this->Index = Index;
    if (FAILED(D3DXCreateTextureFromFileInMemoryEx(
            R_D3DDEVICE,
            MemPoint,
            Length,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            D3DFMT_UNKNOWN,
            D3DPOOL_MANAGED,
            D3DX_FILTER_LINEAR,//D3DX_DEFAULT,
            D3DX_FILTER_LINEAR,//D3DX_DEFAULT,
            MaskColorCode,     // 투명컬러키
            NULL,
            NULL,
            &pID3DTexture)))
    {
        MessageBox(0,  "", "Image Load Error", MB_ICONERROR);
        return false;
    }
    pID3DTexture->GetLevelDesc(0, &ImageInfo);
#endif
    return true;
}

string & TKTexture::GetTextureFileName(void)
{
    return TextureFileName;
}

int TKTexture::GetResourceID(void)
{
    return ResourceID;
}

int TKTexture::GetImageWidth(void)
{
#ifdef TK_D3D9
	return ImageInfo.Width;
#else
	return 0;
#endif
}

int TKTexture::GetImageHeight(void)
{
#ifdef TK_D3D9
	return ImageInfo.Height;
#else
	return 0;
#endif
}

int TKTexture::GetIndex(void)
{
    return Index;
}

void TKTexture::IncUseCount(void)
{
    ++UseCount;
}

void TKTexture::DecUseCount(void)
{
    --UseCount;
}

int TKTexture::GetUseCount(void)
{
    return UseCount;
}

/////////////////////////////////////////////////////////////////////

TKTextureManager::TKTextureManager(TKEngine *pEngine, bool OwnerContainer)
{
    this->pEngine = pEngine;
    LastTextureID = 0;
    TextureList_ID.clear();
    TextureList_Name.clear();
    TextureList_ResourceID.clear();    
    TextureListSeq.clear();

    this->OwnerContainer = OwnerContainer;
}

TKTextureManager::~TKTextureManager(void)
{
    TEXTURELIST_BYID::iterator tit;
    if (OwnerContainer)
    {
        for (tit = TextureList_ID.begin() ; tit != TextureList_ID.end() ; ++tit )
        {
            delete tit->second;
        }
    }
    TextureList_ID.clear();
    TextureList_Name.clear();
    TextureList_ResourceID.clear();
    TextureListSeq.clear();
}

bool TKTextureManager::AddTexture(TKTexture *pTexture)
{
    // 내부함수이므로 크리티컬 섹션 걸면 안됨. 데드락 걸림
    TEXTURELIST_BYID::iterator iti;
    TEXTURELIST_BYNAME::iterator itn;    
    TEXTURELIST_BYRESOURCEID::iterator itr;

    pair<TEXTURELIST_BYID::iterator, bool> Rtn_ID;
    pair<TEXTURELIST_BYNAME::iterator, bool> Rtn_Name;
    pair<TEXTURELIST_BYRESOURCEID::iterator, bool> Rtn_ResID;

    // 텍스쳐ID 리스트에 Insert
    iti = TextureList_ID.find( pTexture->GetIndex() );
    if (iti == TextureList_ID.end())
    {
        Rtn_ID = TextureList_ID.insert( make_pair(pTexture->GetIndex() , pTexture));
        if (!Rtn_ID.second) MessageBox(0, "AddTexture ID fail", "TKTextureManager::AddTexture", MB_ICONERROR);
    }

    // 텍스쳐 파일명 리스트에 Insert
    if ( !pTexture->GetTextureFileName().empty() )
    {
        itn = TextureList_Name.find( pTexture->GetTextureFileName() );
        if (itn == TextureList_Name.end() )
        {
            Rtn_Name = TextureList_Name.insert( make_pair(pTexture->GetTextureFileName(), pTexture));
            if (!Rtn_Name.second) MessageBox(0, "AddTexture Name fail", "TKTextureManager::AddTexture", MB_ICONERROR);
        }
    }

    // 텍스쳐 리소스번호 리스트에 Insert
    if (pTexture->GetResourceID() >= 0 )
    {
        itr = TextureList_ResourceID.find( pTexture->GetResourceID() );
        if  (itr == TextureList_ResourceID.end() )
        {
            Rtn_ResID = TextureList_ResourceID.insert( make_pair(pTexture->GetResourceID(), pTexture) );
            if (!Rtn_ResID.second) MessageBox(0, "AddTexture ResID fail", "TKTextureManager::AddTexture", MB_ICONERROR);
        }
    }

    // 텍스쳐 배열에 Insert
    TextureListSeq.push_back( pTexture );

    return true;
}

bool TKTextureManager::RemoveTexture(TKTexture *pTexture)
{
    // 내부함수이므로 크리티컬 섹션 걸면 안됨. 데드락 걸림
    vector<TKTexture *>::iterator vit;

    if (pTexture != NULL)
    {
        TextureList_ID.erase( pTexture->Index );
        if ( !pTexture->TextureFileName.empty() )
        {
            TextureList_Name.erase( pTexture->TextureFileName );
        }
        if (pTexture->GetResourceID() >= 0)
        {
            TextureList_ResourceID.erase( pTexture->GetResourceID() );
        }
        for ( vit = TextureListSeq.begin() ; vit != TextureListSeq.end() ; ++vit )
        {
            if( (*vit)->GetIndex() == pTexture->GetIndex() )
            {
                TextureListSeq.erase( vit );
                break;
            }
        }
        return true;
    }
    return false;
}

bool TKTextureManager::RemoveTextureByIndex(const int Index)
{
    // 내부함수이므로 크리티컬 섹션 걸면 안됨. 데드락 걸림
    TEXTURELIST_BYID::iterator it;
    it = TextureList_ID.find(Index);
    if (it != TextureList_ID.end() )
    {
        RemoveTexture( it->second  );
    }
    return true;
}

bool TKTextureManager::RemoveTextureByName(string & TextureFileName)
{
    // 내부함수이므로 크리티컬 섹션 걸면 안됨. 데드락 걸림
    TEXTURELIST_BYNAME::iterator it;
    it = TextureList_Name.find(TextureFileName);
    if (it != TextureList_Name.end() )
    {
        RemoveTexture( it->second  );
    }
    return true;
}

bool TKTextureManager::RemoveTextureByResourceID(const int ResourceID)
{
    // 내부함수이므로 크리티컬 섹션 걸면 안됨. 데드락 걸림
    TEXTURELIST_BYRESOURCEID::iterator it;
    it = TextureList_ResourceID.find(ResourceID);
    if (it != TextureList_ID.end() )
    {
        RemoveTexture( it->second  );
    }
    return true;
}

int TKTextureManager::GenID(void)
{
    return LastTextureID++;
}

TKTexture * TKTextureManager::LoadTexture(string & TextureFileName, D3DCOLOR MaskColor, bool SystemTexture)
{
    TKTexture *pTexture = NULL;
    TEXTURELIST_BYNAME::iterator it;
    int Index;

    CS.Enter();
    if (!TextureFileName.empty())
    {
        it = TextureList_Name.find( TextureFileName );
        if (it == TextureList_Name.end() )
        {
            Index = GenID();
            pTexture = new TKTexture(pEngine, this);
            pTexture->LoadFromFile(Index, TextureFileName, MaskColor, SystemTexture);

            AddTexture( pTexture );
        }
        else
        {
             pTexture = it->second;
        }

        pTexture->IncUseCount();
    }
    CS.Leave();

    return pTexture;
}

TKTexture * TKTextureManager::LoadTextureFromResource(int ResourceID, D3DCOLOR MaskColor)
{
    TKTexture *pTexture = NULL;
    TEXTURELIST_BYID::iterator it;
    int Index;

    CS.Enter();
    it = TextureList_ID.find(ResourceID);
    if (it == TextureList_ID.end() )
    {
        Index = GenID();
        pTexture = new TKTexture(pEngine, this);
        pTexture->LoadFromResource(Index, ResourceID, MaskColor);
        
        AddTexture( pTexture );
    }
    else
    {
        pTexture = it->second;
    }
    pTexture->IncUseCount();
    CS.Leave();

    return pTexture;
}

TKTexture * TKTextureManager::LoadTextureFromMemory(char *MemPoint, unsigned int Length, D3DCOLOR MaskColor)
{
    TKTexture *pTexture = NULL;
    TEXTURELIST_BYID::iterator it;
    int Index;

    CS.Enter();
    it = TextureList_ID.find(Index);
    if (it == TextureList_ID.end() )
    {
        Index = GenID();
        pTexture = new TKTexture(pEngine, this);
        pTexture->LoadFromMemory(Index, MemPoint, Length, MaskColor);
        
        AddTexture( pTexture );
    }
    else
    {
        pTexture = it->second;
    }
    pTexture->IncUseCount();
    CS.Leave();

    return pTexture;
}

bool TKTextureManager::ReleaseTexture(TKTexture *pTexture)
{
    if (pTexture == NULL)
    {
        return false;
    }
    return this->ReleaseTextureByIndex( pTexture->Index );
}

bool TKTextureManager::ReleaseTextureByIndex(const int Index)
{
    TEXTURELIST_BYID::iterator it;
    TEXTURELIST_BYNAME::iterator nit;
    TEXTURELIST_BYRESOURCEID::iterator rit;
    vector<TKTexture *>::iterator vit;
    string strFileName;
    bool result = false;

    TKTexture *pTexture = NULL;

    CS.Enter();
    it = TextureList_ID.find(Index);
    if (it != TextureList_ID.end())
    {
        pTexture = it->second;
        pTexture->DecUseCount();
        if (pTexture->GetUseCount() <= 0)
        {
            RemoveTexture(pTexture);
            delete pTexture;
            result = true;
        }
    }
    CS.Leave();

    return result;
}

bool TKTextureManager::ReleaseTextureByName(string & TextureFileName)
{
    TEXTURELIST_BYNAME::iterator it;
    vector<TKTexture *>::iterator vit;
    TKTexture *pTexture = NULL;
    bool result = false;

    CS.Enter();
    it = TextureList_Name.find(TextureFileName);
    if (it != TextureList_Name.end())
    {
        pTexture = it->second;
        pTexture->DecUseCount();
        if (pTexture->GetUseCount() <= 0)
        {
            RemoveTexture(pTexture);
            delete pTexture;
            result = true;
        }
    }
    CS.Leave();

    return result;
}

bool TKTextureManager::ReleaseTextureByResourceID(const int ResourceID)
{
    TEXTURELIST_BYRESOURCEID::iterator it;
    vector<TKTexture *>::iterator vit;
    TKTexture *pTexture = NULL;
    bool result = false;

    CS.Enter();
    it = TextureList_ResourceID.find(ResourceID);
    if (it != TextureList_ID.end())
    {
        pTexture = it->second;
        pTexture->DecUseCount();
        if (pTexture->GetUseCount() <= 0)
        {
            RemoveTexture(pTexture);
            delete pTexture;
            result = true;
        }
    }
    CS.Leave();

    return result;
}

TKTexture * TKTextureManager::GetTexture(const int Index)
{
    TKTexture *pTexture = NULL;
    TEXTURELIST_BYID::iterator tit;
    CS.Enter();
    tit = TextureList_ID.find(Index);
    if (tit != TextureList_ID.end() )
    {
        pTexture = tit->second;
    }
    CS.Leave();
    return  pTexture;
}

TKTexture * TKTextureManager::GetTextureByName(string & TextureFileName)
{
    TKTexture *pTexture = NULL;
    TEXTURELIST_BYNAME::iterator tit;
    CS.Enter();
    tit = TextureList_Name.find(TextureFileName);
    if (tit != TextureList_Name.end() )
    {
        pTexture = tit->second;
    }
    CS.Leave();
    return  pTexture;
}

TKTexture * TKTextureManager::GetTextureByResourceID(const int ResourceID)
{
    TKTexture *pTexture = NULL;
    TEXTURELIST_BYRESOURCEID::iterator tit;
    CS.Enter();
    tit = TextureList_ResourceID.find(ResourceID);
    if (tit != TextureList_ResourceID.end() )
    {
        pTexture = tit->second;
    }
    CS.Leave();
    return  pTexture;
}

TKTexture * TKTextureManager::GetTextureBySeq(const int Index)
{
    TKTexture *pTexture = NULL;
    CS.Enter();
    if (Index < (int)TextureListSeq.size())
    {
        pTexture = TextureListSeq[Index];        
    }
    CS.Enter();
    return pTexture;
}

void TKTextureManager::SetSystemTextureDir(string & SystemTextureDir)
{
    this->SystemTextureDir = SystemTextureDir;
}

string & TKTextureManager::GetSystemTextureDir(void)
{
    return SystemTextureDir;
}

void TKTextureManager::SetUserTextureDir(string & UserTextureDir)
{
    this->UserTextureDir = UserTextureDir;
}

string & TKTextureManager::GetUserTextureDir(void)
{
    return UserTextureDir;
}

int TKTextureManager::Count(void)
{
    return (int)TextureList_ID.size();
}

/////////////////////////////////////////////////////////////////////

TKTextureList::TKTextureList(TKEngine *pEngine)
{
    this->pEngine = pEngine;
    TextureList_ID.clear();
    TextureListSeq.clear();
}

TKTextureList::~TKTextureList(void)
{
    TextureList_ID.clear();
    TextureListSeq.clear();
}

bool TKTextureList::AddTexture(TKTexture *pTexture)
{
    TEXTURELIST_BYID::iterator iti;
    TEXTURELIST_BYNAME::iterator itn;    
    TEXTURELIST_BYRESOURCEID::iterator itr;
    string strFileName;
    pair<TEXTURELIST_BYID::iterator, bool> Rtn_ID;

    CS.Enter();
    // 텍스쳐ID 리스트에 Insert
    iti = TextureList_ID.find( pTexture->GetIndex() );
    if (iti == TextureList_ID.end())
    {
        Rtn_ID = TextureList_ID.insert( make_pair(pTexture->GetIndex() , pTexture));
        if (!Rtn_ID.second) MessageBox(0, "AddTexture ID Fail", "TKTextureList::AddTexture", MB_ICONERROR);
    }

    // 텍스쳐 배열에 Insert
    TextureListSeq.push_back( pTexture );
    CS.Leave();

    return true;
}

bool TKTextureList::RemoveTexture(TKTexture *pTexture)
{
    vector<TKTexture *>::iterator vit;
    bool result = false;

    CS.Enter();
    if (pTexture != NULL)
    {
        TextureList_ID.erase( pTexture->Index );
        for ( vit = TextureListSeq.begin() ; vit != TextureListSeq.end() ; ++vit )
        {
            if( (*vit)->GetIndex() == pTexture->GetIndex() )
            {
                TextureListSeq.erase( vit );
                break;
            }
        }
        result = true;
    }
    CS.Leave();
    return result;
}

bool TKTextureList::RemoveTextureByIndex(const int Index)
{
    TEXTURELIST_BYID::iterator it;
    vector<TKTexture *>::iterator vit;
    bool result = false;

    CS.Enter();
    it = TextureList_ID.find(Index);
    if (it != TextureList_ID.end() )
    {
        TextureList_ID.erase( Index );
        for ( vit = TextureListSeq.begin() ; vit != TextureListSeq.end() ; ++vit )
        {
            if( (*vit)->GetIndex() == Index )
            {
                TextureListSeq.erase( vit );
                break;
            }
        }
        result = true;
    }
    CS.Leave();
    return result;
}

TKTexture * TKTextureList::GetTexture(const int Index)
{
    TKTexture *pTexture = NULL;
    TEXTURELIST_BYID::iterator tit;
    CS.Enter();
    tit = TextureList_ID.find(Index);
    if (tit != TextureList_ID.end() )
    {
        pTexture = tit->second;
    }
    CS.Leave();
    return  pTexture;
}

TKTexture * TKTextureList::GetTextureBySeq(const int Index)
{
    TKTexture *pTexture = NULL;
    CS.Enter();
    if (Index < (int)TextureListSeq.size())
    {
        pTexture = TextureListSeq[Index];        
    }
    CS.Leave();
    return pTexture;
}

int TKTextureList::Count(void)
{
    return (int)TextureList_ID.size();
}

void TKTextureList::Clear(void)
{
    CS.Enter();
    TextureList_ID.clear();
    TextureListSeq.clear();
    CS.Leave();
}

/////////////////////////////////////////////////////////////////////

