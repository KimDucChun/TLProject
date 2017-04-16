#include "./TKFileIOClasses.h"

///////////////////////////////////////////////////////////////////////////////
//TKMAXFileIO
///////////////////////////////////////////////////////////////////////////////

TKMAXFileIO::TKMAXFileIO(void)
:TKObject()
{
    SetObjectType( eotCustomParser );
}

TKMAXFileIO::~TKMAXFileIO(void)
{
    Scene.Clear();
    Material_List.Clear();
    GeomObject_List.Clear();
}

bool TKMAXFileIO::Clear(void)
{
    Scene.Clear();
    Material_List.Clear();
    GeomObject_List.Clear();
    return true;
}

bool TKMAXFileIO::SaveToFile(const char *FileName)
{
    TKFile SaveFile;

    SaveFile.FileOpen(FileName, efomWRITE_BINARY);
    
    Scene.SaveToFile( &SaveFile );
    Material_List.SaveToFile( &SaveFile );
    GeomObject_List.BindMaterialList(Material_List);
    GeomObject_List.SaveToFile( &SaveFile );

    return true;
}

bool TKMAXFileIO::SaveToText(const char *FileName)
{
    TKFile TextFile;

    TextFile.FileOpen(FileName, efomWRITE);
    
    Scene.SaveToText( &TextFile );
    Material_List.SaveToText( &TextFile );
    GeomObject_List.BindMaterialList(Material_List);
    GeomObject_List.SaveToText( &TextFile );

    return true;
}

bool TKMAXFileIO::LoadFromFile(const char *FileName)
{
    TKFile LoadFile;
    bool FileOpend;

    FileOpend = LoadFile.FileOpen(FileName, efomREAD_BINARY);
    if (FileOpend == false)
    {
        MessageBox(0, FileName, "파일오픈에러", MB_ICONERROR);
        _ASSERT( 0 );
    }    

    LoadFile.Begin();
    this->Clear();

    Scene.LoadFromFile(&LoadFile);
    Material_List.LoadFromFile(&LoadFile);
    GeomObject_List.LoadFromFile(&LoadFile);    

    return true;
}

bool TKMAXFileIO::operator == (TKMAXFileIO & CustomParser)
{
    if (!(this->Scene == CustomParser.Scene))
    {
        return false;
    }
    if (!(this->Material_List == CustomParser.Material_List))
    {
        return false;
    }
    if (!(this->GeomObject_List == CustomParser.GeomObject_List))
    {
        return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//TKMAXFileIO
///////////////////////////////////////////////////////////////////////////////
TKModelFileIO::TKModelFileIO(void)
{
    this->Clear();
}

TKModelFileIO::~TKModelFileIO(void)
{
    this->Clear();
}

bool TKModelFileIO::Clear(void)
{
    MeshResData.Clear();
    AniMotionList.Clear();
    MeshLodList.Clear();    
    return true;
}

bool TKModelFileIO::SaveToFile(const char *FileName)
{
    TKFile SaveFile;
    SaveFile.FileOpen(FileName, efomWRITE_BINARY);
    MeshResData.SaveToFile( &SaveFile );
    AniMotionList.SaveToFile( &SaveFile );
    MeshLodList.SaveToFile( &SaveFile );
    return true;
}

bool TKModelFileIO::SaveToText(const char *FileName)
{
    return true;
}

bool TKModelFileIO::LoadFromFile(const char *FileName)
{
    TKFile LoadFile;
    bool FileOpend;

    FileOpend = LoadFile.FileOpen(FileName, efomREAD_BINARY);
    if (FileOpend == false)
    {
        MessageBox(0, FileName, "파일오픈에러", MB_ICONERROR);
        _ASSERT( 0 );
    }    

    LoadFile.Begin();
    this->Clear();
    
    MeshResData.LoadFromFile( &LoadFile );
    AniMotionList.LoadFromFile( &LoadFile );
    MeshLodList.LoadFromFile( &LoadFile );
   
    return true;
}

bool TKModelFileIO::operator == (TKModelFileIO & CustomParser)
{
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//TKMotionFileIO
///////////////////////////////////////////////////////////////////////////////
TKMotionFileIO::TKMotionFileIO(void)
{
}

TKMotionFileIO::~TKMotionFileIO(void)
{
    AniMotionList.Clear();
}

bool TKMotionFileIO::Clear(void)
{
    AniMotionList.Clear();
    return true;
}

bool TKMotionFileIO::SaveToFile(const char *FileName)
{
    TKFile SaveFile;
    SaveFile.FileOpen(FileName, efomWRITE_BINARY);
    AniMotionList.SaveToFile( &SaveFile );
    return true;
}

bool TKMotionFileIO::SaveToText(const char *FileName)
{
    TKFile TextFile;
    TextFile.FileOpen(FileName, efomWRITE);
    AniMotionList.SaveToText( &TextFile );
    return true;
}

bool TKMotionFileIO::LoadFromFile(const char *FileName)
{
    TKFile LoadFile;
    bool FileOpend;

    FileOpend = LoadFile.FileOpen(FileName, efomREAD_BINARY);
    if (FileOpend == false)
    {
        MessageBox(0, FileName, "파일오픈에러", MB_ICONERROR);
    }
    _ASSERT( FileOpend );

    LoadFile.Begin();
    this->Clear();

    AniMotionList.LoadFromFile(&LoadFile);    
    
    return true;
}

bool TKMotionFileIO::operator == (TKMotionFileIO & CustomParser)
{
    if (!(this->AniMotionList == CustomParser.AniMotionList))
    {
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//TKLODFileIO
///////////////////////////////////////////////////////////////////////////////
TKLODFileIO::TKLODFileIO(void)
{
}

TKLODFileIO::~TKLODFileIO(void)
{
    MeshLodList.Clear();
}

bool TKLODFileIO::Clear(void)
{
    MeshLodList.Clear();
    return true;
}

bool TKLODFileIO::SaveToFile(const char *FileName)
{
    TKFile SaveFile;
    SaveFile.FileOpen(FileName, efomWRITE_BINARY);
    MeshLodList.SaveToFile( &SaveFile );
    return true;
}

bool TKLODFileIO::SaveToText(const char *FileName)
{
    TKFile TextFile;
    TextFile.FileOpen(FileName, efomWRITE);
    MeshLodList.SaveToText( &TextFile );
    return true;
}

bool TKLODFileIO::LoadFromFile(const char *FileName)
{
    TKFile LoadFile;
    bool FileOpend;

    FileOpend = LoadFile.FileOpen(FileName, efomREAD_BINARY);
    if (FileOpend == false)
    {
        MessageBox(0, FileName, "파일오픈에러", MB_ICONERROR);
    }
    _ASSERT( FileOpend );

    LoadFile.Begin();
    this->Clear();

    MeshLodList.LoadFromFile(&LoadFile);    
    
    return true;
}

bool TKLODFileIO::operator == (TKLODFileIO & CustomParser)
{
    if (!(this->MeshLodList == CustomParser.MeshLodList))
    {
        return false;
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
//TKKCTFileIO
/////////////////////////////////////////////////////////////////////////////
TKKCTFileIO::TKKCTFileIO(void)
{
    this->Clear();
}

TKKCTFileIO::~TKKCTFileIO(void)
{
    this->Clear();
}

void TKKCTFileIO::Clear(void)
{
    memset(VersionInfo, 0, sizeof(VersionInfo));
    Collision.Clear();
}

void TKKCTFileIO::SaveToFile(const char *FileName)
{
    TKFile SaveFile;
    SaveFile.FileOpen(FileName, efomWRITE_BINARY);
    Collision.SaveToFile( &SaveFile );
}

void TKKCTFileIO::SaveToText(const char *FileName)
{
    TKFile TextFile;
    TextFile.FileOpen(FileName, efomWRITE);
    Collision.SaveToText( &TextFile );
}

void TKKCTFileIO::LoadFromFile(const char *FileName)
{
    TKFile LoadFile;
    if (LoadFile.FileOpen(FileName, efomREAD_BINARY) == false)
    {
        MessageBox(0, FileName, "파일오픈에러", MB_ICONERROR);
        _ASSERT(0);    
    }
    this->Clear();
    LoadFile.Begin();
    Collision.LoadFromFile(&LoadFile);
}

/////////////////////////////////////////////////////////////////////////////
//TKSGObjectFileIO
/////////////////////////////////////////////////////////////////////////////
TKSGObjectFileIO::TKSGObjectFileIO(void)
{
    this->Clear();
}

TKSGObjectFileIO::~TKSGObjectFileIO(void)
{
    this->Clear();
}

void TKSGObjectFileIO::Clear(void)
{
    MeshResData.Clear();
    AniMotionList.Clear();
    MeshLodList.Clear();
    Collision.Clear();
}

void TKSGObjectFileIO::SaveToFile(const char *FileName)
{
    TKFile SaveFile;
    SaveFile.FileOpen(FileName, efomWRITE_BINARY);

    MeshResData.SaveToFile( &SaveFile );
    AniMotionList.SaveToFile( &SaveFile );
    MeshLodList.SaveToFile( &SaveFile );
    Collision.SaveToFile( &SaveFile );
}

void TKSGObjectFileIO::SaveToText(const char *FileName)
{
}

void TKSGObjectFileIO::LoadFromFile(const char *FileName)
{
    TKFile LoadFile;
    if (LoadFile.FileOpen(FileName, efomREAD_BINARY) == false)
    {
        MessageBox(0, FileName, "파일오픈에러", MB_ICONERROR);
        _ASSERT(0);    
    }
    this->Clear();
    LoadFile.Begin();

    MeshResData.LoadFromFile(&LoadFile);
    AniMotionList.LoadFromFile(&LoadFile);
    MeshLodList.LoadFromFile(&LoadFile);
    Collision.LoadFromFile(&LoadFile);
}