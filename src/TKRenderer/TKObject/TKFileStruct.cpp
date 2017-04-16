#include "./TKFileStruct.h"

using namespace FILESTRUCT;

///////////////////////////////////////////////////////////////////////////////
//MAX_SCENE
///////////////////////////////////////////////////////////////////////////////

MAX_SCENE::MAX_SCENE(void)
{
    this->Clear();
}

MAX_SCENE::MAX_SCENE(MAX_SCENE & Scene)
{
    *this = Scene;
}

void MAX_SCENE::Clear(void)
{
    ZeroMemory(this, sizeof(MAX_SCENE));
}

MAX_SCENE & MAX_SCENE::operator = (MAX_SCENE & Scene)
{
    strcpy(this->strFileName, Scene.strFileName);
    this->FirstFrame         = Scene.FirstFrame         ;
    this->LastFrame          = Scene.LastFrame          ;
    this->FrameSpeed         = Scene.FrameSpeed         ;
    this->TickPerFrame       = Scene.TickPerFrame       ;
    this->vBackGround_static = Scene.vBackGround_static ;
    this->vAmbient_static    = Scene.vAmbient_static    ;

    return Scene;
}

bool MAX_SCENE::operator == (MAX_SCENE & Scene)
{
    return 
        ((strcmp(this->strFileName, Scene.strFileName) == 0) &&
        (this->FirstFrame         == Scene.FirstFrame        ) &&
        (this->LastFrame          == Scene.LastFrame         ) &&
        (this->FrameSpeed         == Scene.FrameSpeed        ) &&
        (this->TickPerFrame       == Scene.TickPerFrame      ) &&
        (this->vBackGround_static == Scene.vBackGround_static) &&
        (this->vAmbient_static    == Scene.vAmbient_static   ) );
}

bool MAX_SCENE::SaveToFile(TKFile *pFile)
{
    pFile->Write(this, sizeof(MAX_SCENE), 1);
    return true;
}

bool MAX_SCENE::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;
    WriteBuffer.Format("MAXSCENE\n%s, %d, %d, %d, %d, %f, %f, %f, %f, %f, %f, %f, %f\n", 
                        strFileName, FirstFrame, LastFrame, FrameSpeed, TickPerFrame, 
                        vBackGround_static.a, vBackGround_static.r, vBackGround_static.g, vBackGround_static.b,
                        vAmbient_static.a, vAmbient_static.r, vAmbient_static.g, vAmbient_static.b );
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    return true;
}

bool MAX_SCENE::LoadFromFile(TKFile *pFile)
{
    this->Clear();
    pFile->Read(this, sizeof(MAX_SCENE), 1);
    return true;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//MAPPING_INFO
///////////////////////////////////////////////////////////////////////////////

MAPPING_INFO::MAPPING_INFO(void)
:MapSubNo(0)
,MapAmount(0)
,UVW_U_OffSet(0)
,UVW_V_OffSet(0)
,UVW_U_Tiling(0)
,UVW_V_Tiling(0)
,UVW_Angle(0)
,UVW_Blur(0)
,UVW_BlurOffSet(0)
,UVW_NouseAmt(0)
,UVW_NoiseSize(0)
,UVW_NoiseLevel(0)
,UVW_NoisePhase(0)
{
    MapName[0] = 0;
    MapClass[0] = 0;
    MapFileName[0] = 0;
    MapType = 0;
    strMapType[0] = 0;
    BitmapFilter = 0;
    strBitmapFilter[0] = 0;
}

MAPPING_INFO::MAPPING_INFO(MAPPING_INFO & Mapping_Info)
{
    *this = Mapping_Info;
}

MAPPING_INFO::~MAPPING_INFO(void)
{
}

MAPPING_INFO & MAPPING_INFO::operator = (MAPPING_INFO &Mapping_Info)
{
    strcpy(this->MapName    , Mapping_Info.MapName);
    strcpy(this->MapClass   , Mapping_Info.MapClass);
    this->MapSubNo       = Mapping_Info.MapSubNo;
    this->MapAmount      = Mapping_Info.MapAmount;
    strcpy(this->MapFileName, Mapping_Info.MapFileName);
    this->MapType        = Mapping_Info.MapType;
    strcpy(this->strMapType, Mapping_Info.strMapType);
    this->UVW_U_OffSet   = Mapping_Info.UVW_U_OffSet;
    this->UVW_V_OffSet   = Mapping_Info.UVW_V_OffSet;
    this->UVW_U_Tiling   = Mapping_Info.UVW_U_Tiling;
    this->UVW_V_Tiling   = Mapping_Info.UVW_V_Tiling;
    this->UVW_Angle      = Mapping_Info.UVW_Angle;
    this->UVW_Blur       = Mapping_Info.UVW_Blur;
    this->UVW_BlurOffSet = Mapping_Info.UVW_BlurOffSet;
    this->UVW_NouseAmt   = Mapping_Info.UVW_NouseAmt;
    this->UVW_NoiseSize  = Mapping_Info.UVW_NoiseSize;
    this->UVW_NoiseLevel = Mapping_Info.UVW_NoiseLevel;
    this->UVW_NoisePhase = Mapping_Info.UVW_NoisePhase;
    this->BitmapFilter   = Mapping_Info.BitmapFilter;
    strcpy(this->strBitmapFilter, Mapping_Info.strBitmapFilter);

    return Mapping_Info;
}

bool MAPPING_INFO::operator == (MAPPING_INFO & Mapping_Info)
{
    bool result;
    result =  
        (strcmp(this->MapName, Mapping_Info.MapName) == 0                 ) &&
        (strcmp(this->MapClass, Mapping_Info.MapClass) == 0               ) &&
        (this->MapSubNo               == Mapping_Info.MapSubNo            ) &&
        (this->MapAmount              == Mapping_Info.MapAmount           ) &&
        (strcmp(this->MapFileName, Mapping_Info.MapFileName) == 0         ) &&
        (this->MapType                == Mapping_Info.MapType             ) &&
        (strcmp(this->strMapType, Mapping_Info.strMapType) == 0           ) &&
        (this->UVW_U_OffSet           == Mapping_Info.UVW_U_OffSet        ) &&
        (this->UVW_V_OffSet           == Mapping_Info.UVW_V_OffSet        ) &&
        (this->UVW_U_Tiling           == Mapping_Info.UVW_U_Tiling        ) &&
        (this->UVW_V_Tiling           == Mapping_Info.UVW_V_Tiling        ) &&
        (this->UVW_Angle              == Mapping_Info.UVW_Angle           ) &&
        (this->UVW_Blur               == Mapping_Info.UVW_Blur            ) &&
        (this->UVW_BlurOffSet         == Mapping_Info.UVW_BlurOffSet      ) &&
        (this->UVW_NouseAmt           == Mapping_Info.UVW_NouseAmt        ) &&
        (this->UVW_NoiseSize          == Mapping_Info.UVW_NoiseSize       ) &&
        (this->UVW_NoiseLevel         == Mapping_Info.UVW_NoiseLevel      ) &&
        (this->UVW_NoisePhase         == Mapping_Info.UVW_NoisePhase      ) &&
        (this->BitmapFilter           == Mapping_Info.BitmapFilter        ) &&
        (strcmp(this->strBitmapFilter, Mapping_Info.strBitmapFilter) == 0 );

    return result;
}

bool MAPPING_INFO::SaveToFile(TKFile *pFile)
{
    pFile->Write(this, sizeof(MAPPING_INFO), 1);
    return true;
}

bool MAPPING_INFO::SaveToText(TKFile *pTextFile)
{
    CString FormatBuffer;
    CString WriteBuffer;

    FormatBuffer = "MAPPINGINFO\n";
    FormatBuffer += "MapName: %s, ";
    FormatBuffer += "MapClass: %s, ";
    FormatBuffer += "MapSubNo: %d, ";
    FormatBuffer += "MapAmount: %f, ";
    FormatBuffer += "MapFileName: %s, ";
    FormatBuffer += "MapType: %d, ";
    FormatBuffer += "strMapType: %s, ";
    FormatBuffer += "UVW_U_OffSet: %f, ";
    FormatBuffer += "UVW_V_OffSet: %f, ";
    FormatBuffer += "UVW_U_Tiling: %f, ";
    FormatBuffer += "UVW_V_Tiling: %f, ";
    FormatBuffer += "UVW_Angle: %f, ";
    FormatBuffer += "UVW_Blur: %f, ";
    FormatBuffer += "UVW_BlurOffSet: %f, ";
    FormatBuffer += "UVW_NouseAmt: %f, ";
    FormatBuffer += "UVW_NoiseSize: %f, ";
    FormatBuffer += "UVW_NoiseLevel: %d, ";
    FormatBuffer += "UVW_NoisePhase: %f, ";
    FormatBuffer += "BitmapFilter: %d, ";
    FormatBuffer += "strBitmapFilter: %s\n";

    WriteBuffer.Format( FormatBuffer.GetBuffer(), 
                        MapName, MapClass, MapSubNo, MapAmount, 
                        MapFileName, MapType, strMapType, UVW_U_OffSet, UVW_V_OffSet, 
                        UVW_U_Tiling, UVW_V_Tiling, UVW_Angle, UVW_Blur, UVW_BlurOffSet, 
                        UVW_NouseAmt, UVW_NoiseSize, UVW_NoiseLevel, UVW_NoisePhase, 
                        BitmapFilter, strBitmapFilter);

    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    return true;
}

bool MAPPING_INFO::LoadFromFile(TKFile *pFile)
{
    pFile->Read(this, sizeof(MAPPING_INFO), 1);
    return true;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//MAX_MATERIALINFO
///////////////////////////////////////////////////////////////////////////////
MAX_MATERIALINFO::MAX_MATERIALINFO(void)
{
    this->Clear();
}

MAX_MATERIALINFO::MAX_MATERIALINFO(MAX_MATERIALINFO & Material_Info)
{
    *this = Material_Info;
}

MAX_MATERIALINFO::~MAX_MATERIALINFO(void)
{
    this->Clear();
}

void MAX_MATERIALINFO::AddItem(MAX_MATERIALINFO & Material_Info)
{
    Items.AddNew(Material_Info);
    //Items.Sort( MAX_MATERIALINFO_COMPARE );
}

void MAX_MATERIALINFO::DeleteItem(int Index)
{
    Items.EraseDelete(Index);
}

void MAX_MATERIALINFO::Clear(void)
{
    Index = -1;
    MaterialName[0] = 0;
    MaterialClass[0] = 0;
    MAmbient = D3DXCOLOR(0,0,0,0);
    MDiffuse = D3DXCOLOR(0,0,0,0);
    MSpecular = D3DXCOLOR(0,0,0,0);
    Shine = 0;
    ShineStrength = 0;
    Transparency = 0;
    WireSize = 0;
    Shading = 0;
    strShading[0] = 0;
    XP_FallOff = 0;
    Selfillum = 0;
    TwoSided = false;
    AlphaBlending = false;
    FallOff[0] = 0;    
    XPType = 0;
    strXPType[0] = 0;

    Items.ClearDelete();
}

MAX_MATERIALINFO & MAX_MATERIALINFO::operator = (MAX_MATERIALINFO & Material_Info)
{
    this->Clear();

    this->Index                 = Material_Info.Index;
    strcpy(this->MaterialName   , Material_Info.MaterialName);
    strcpy(this->MaterialClass  , Material_Info.MaterialClass);
    this->MAmbient              = Material_Info.MAmbient;
    this->MDiffuse              = Material_Info.MDiffuse;
    this->MSpecular             = Material_Info.MSpecular;
    this->Shine                 = Material_Info.Shine;
    this->ShineStrength         = Material_Info.ShineStrength;
    this->Transparency          = Material_Info.Transparency;
    this->WireSize              = Material_Info.WireSize;
    this->Shading               = Material_Info.Shading;
    strcpy(this->strShading, Material_Info.strShading);
    this->XP_FallOff            = Material_Info.XP_FallOff;
    this->Selfillum             = Material_Info.Selfillum;
    this->TwoSided              = Material_Info.TwoSided;
    this->AlphaBlending         = Material_Info.AlphaBlending;
    strcpy(this->FallOff, Material_Info.FallOff);
    this->XPType                = Material_Info.XPType;
    strcpy(this->strXPType, Material_Info.strXPType);

    for ( int i = 0 ; i < MATERIAL_TEXTURE_COUNT ; ++i )
    {
        this->MappingInfo[i] = Material_Info.MappingInfo[i];
    }

    for ( int i = 0 ; i < Material_Info.Items.Count() ; ++i )
    {
        MAX_MATERIALINFO *pMat = Material_Info.Items[i];
        this->Items.AddNew( (*pMat) );
    }

    return Material_Info;
}

bool MAX_MATERIALINFO::operator == (MAX_MATERIALINFO & Material_Info)
{
    bool Result;
    
    Result =
        (this->Index                == Material_Info.Index         ) &&
        (strcmp(this->MaterialName, Material_Info.MaterialName) == 0) &&
        (strcmp(this->MaterialClass, Material_Info.MaterialClass) == 0) &&
        (this->MAmbient             == Material_Info.MAmbient      ) &&
        (this->MDiffuse             == Material_Info.MDiffuse      ) &&
        (this->MSpecular            == Material_Info.MSpecular     ) &&
        (this->Shine                == Material_Info.Shine         ) &&
        (this->ShineStrength        == Material_Info.ShineStrength ) &&
        (this->Transparency         == Material_Info.Transparency  ) &&
        (this->WireSize             == Material_Info.WireSize      ) &&
        (this->Shading              == Material_Info.Shading       ) &&
        (strcmp(this->strShading, Material_Info.strShading)     == 0) &&
        (this->XP_FallOff           == Material_Info.XP_FallOff    ) &&
        (this->Selfillum            == Material_Info.Selfillum     ) &&
        (this->TwoSided             == Material_Info.TwoSided      ) &&
        (this->AlphaBlending        == Material_Info.AlphaBlending ) &&
        (strcmp(this->FallOff, Material_Info.FallOff)           == 0) &&
        (this->XPType               == Material_Info.XPType        ) &&
        (strcmp(this->strXPType, Material_Info.strXPType)       == 0);

    if (Result == false)
    {
        return false;
    }

    for (int i = 0 ; i < MATERIAL_TEXTURE_COUNT ; ++i )
    {
        if (( this->MappingInfo[i] == Material_Info.MappingInfo[i] ) == false )
        {
            return false;
        }
    }

    if (this->Items.Count() != Material_Info.Items.Count())
    {
        return false;
    }

    int iSize = (int)this->Items.Count();

    for ( int i = 0 ; i < iSize ; ++i )
    {
        if (!(*this->Items[i] == *Material_Info.Items[i]))
        {
            return false;
        }
    }

    return true;
}

bool MAX_MATERIALINFO::SaveToFile(TKFile *pFile)
{
    pFile->Write(&Index         ,sizeof(int      )     , 1 );
    pFile->Write(MaterialName   ,sizeof(char     ) * 32, 1 );
    pFile->Write(MaterialClass  ,sizeof(char     ) * 32, 1 );
    pFile->Write(&MAmbient      ,sizeof(D3DXCOLOR)     , 1 );
    pFile->Write(&MDiffuse      ,sizeof(D3DXCOLOR)     , 1 );
    pFile->Write(&MSpecular     ,sizeof(D3DXCOLOR)     , 1 );
    pFile->Write(&Shine         ,sizeof(float    )     , 1 );
    pFile->Write(&ShineStrength ,sizeof(float    )     , 1 );
    pFile->Write(&Transparency  ,sizeof(float    )     , 1 );
    pFile->Write(&WireSize      ,sizeof(float    )     , 1 );
    pFile->Write(&Shading       ,sizeof(int      )     , 1 );
    pFile->Write(strShading     ,sizeof(char     ) * 32, 1 );
    pFile->Write(&XP_FallOff    ,sizeof(float    )     , 1 );
    pFile->Write(&Selfillum     ,sizeof(float    )     , 1 );
    pFile->Write(&TwoSided      ,sizeof(bool     )     , 1 );
    pFile->Write(&AlphaBlending ,sizeof(bool     )     , 1 );
    pFile->Write(FallOff        ,sizeof(char     ) * 8 , 1 );
    pFile->Write(&XPType        ,sizeof(int      )     , 1 );
    pFile->Write(strXPType      ,sizeof(char     ) * 32, 1 );

    for (int i = 0 ; i < MATERIAL_TEXTURE_COUNT ; ++i )
    {
        MappingInfo[i].SaveToFile(pFile);
    }
    
    int iSize = (int)Items.Count();
    pFile->Write(&iSize, sizeof(int), 1);

    for ( int i = 0 ; i < Items.Count() ; ++i )
    {
        Items[i]->SaveToFile(pFile);
    }

    return true;
}

bool MAX_MATERIALINFO::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;
    CString FormatBuffer;

    FormatBuffer = "MAX_MATERIALINFO\n";
    FormatBuffer += "Index: %d,";
    FormatBuffer += "MaterialName: %s,";
    FormatBuffer += "MaterialClass: %s,";
    FormatBuffer += "MAmbient.a: %f,";
    FormatBuffer += "MAmbient.r: %f,";
    FormatBuffer += "MAmbient.g: %f,";
    FormatBuffer += "MAmbient.b: %f,";
    FormatBuffer += "MDiffuse.a: %f,";
    FormatBuffer += "MDiffuse.r: %f,";
    FormatBuffer += "MDiffuse.g: %f,";
    FormatBuffer += "MDiffuse.b: %f,";
    FormatBuffer += "Specular.a: %f,";
    FormatBuffer += "MSpecular.r: %f,";
    FormatBuffer += "MSpecular.g: %f,";
    FormatBuffer += "MSpecular.b: %f,";
    FormatBuffer += "Shine: %f,";
    FormatBuffer += "ShineStrength: %f,";
    FormatBuffer += "Transparency: %f,";
    FormatBuffer += "WireSize: %f,";
    FormatBuffer += "Shading: %d,";
    FormatBuffer += "strShading: %s,";
    FormatBuffer += "XP_FallOff: %f,";
    FormatBuffer += "Selfillum: %f,";
    FormatBuffer += "TwoSided: %d,";
    FormatBuffer += "AlphaBlending: %d,";
    FormatBuffer += "FallOff: %s,";
    FormatBuffer += "XPType: %d,";
    FormatBuffer += "strXPType: %s\n";

    WriteBuffer.Format(FormatBuffer.GetBuffer(), 
                        Index, MaterialName, MaterialClass, 
                        MAmbient.a, MAmbient.r, MAmbient.g, MAmbient.b, 
                        MDiffuse.a, MDiffuse.r, MDiffuse.g, MDiffuse.b, 
                        MSpecular.a, MSpecular.r, MSpecular.g, MSpecular.b,
                        Shine, ShineStrength, Transparency, WireSize, Shading,
                        strShading, XP_FallOff, Selfillum, TwoSided, AlphaBlending,
                        FallOff, XPType, strXPType);

    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    for (int i = 0 ; i < MATERIAL_TEXTURE_COUNT ; ++i )
    {
        WriteBuffer.Format( "TextureIndex : %d\n", i );
        pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
        MappingInfo[i].SaveToText(pTextFile);
    }
    
    int iSize = (int)Items.Count();
    WriteBuffer.Format( "SubMaterialCount : %d\n", iSize );
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    
    for ( int i = 0 ; i < Items.Count() ; ++i )
    {
        Items[i]->SaveToText(pTextFile);
    }

    return true;
}

bool MAX_MATERIALINFO::LoadFromFile(TKFile *pFile)
{
    this->Clear();

    pFile->Read(&Index              ,sizeof(int      )     , 1 );
    pFile->Read(MaterialName        ,sizeof(char     ) * 32, 1 );
    pFile->Read(MaterialClass       ,sizeof(char     ) * 32, 1 );
    pFile->Read(&MAmbient           ,sizeof(D3DXCOLOR)     , 1 );
    pFile->Read(&MDiffuse           ,sizeof(D3DXCOLOR)     , 1 );
    pFile->Read(&MSpecular          ,sizeof(D3DXCOLOR)     , 1 );
    pFile->Read(&Shine              ,sizeof(float    )     , 1 );
    pFile->Read(&ShineStrength      ,sizeof(float    )     , 1 );
    pFile->Read(&Transparency       ,sizeof(float    )     , 1 );
    pFile->Read(&WireSize           ,sizeof(float    )     , 1 );
    pFile->Read(&Shading            ,sizeof(int      )     , 1 );
    pFile->Read(strShading          ,sizeof(char     ) * 32, 1 );
    pFile->Read(&XP_FallOff         ,sizeof(float    )     , 1 );
    pFile->Read(&Selfillum          ,sizeof(float    )     , 1 );
    pFile->Read(&TwoSided           ,sizeof(bool     )     , 1 );
    pFile->Read(&AlphaBlending      ,sizeof(bool     )     , 1 );
    pFile->Read(FallOff             ,sizeof(char     ) * 8 , 1 );
    pFile->Read(&XPType             ,sizeof(int      )     , 1 );
    pFile->Read(strXPType           ,sizeof(char     ) * 32, 1 );

    for (int i = 0 ; i < MATERIAL_TEXTURE_COUNT ; ++i )
    {
        MappingInfo[i].LoadFromFile(pFile);
    }

    int iSize;
    pFile->Read(&iSize, sizeof(int), 1);

    MAX_MATERIALINFO *pMaterial_Info;
    for (int i = 0 ; i < iSize ; ++i )
    {
        pMaterial_Info = new MAX_MATERIALINFO;
        pMaterial_Info->LoadFromFile(pFile);
        Items.Add( pMaterial_Info );
    }

    return true;
}

int MAX_MATERIALINFO_COMPARE(MAX_MATERIALINFO *pA, MAX_MATERIALINFO *pB)
{
    if (pA->Index > pB->Index) return 1;
    else if (pA->Index < pB->Index) return -1;
    else return 0;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//MAX_MATERIAL_LIST
///////////////////////////////////////////////////////////////////////////////
MAX_MATERIAL_LIST::MAX_MATERIAL_LIST(void)
{
    this->Clear();
}

MAX_MATERIAL_LIST::~MAX_MATERIAL_LIST(void)
{
    this->Clear();
}

void MAX_MATERIAL_LIST::AddItem(MAX_MATERIALINFO & Material_Info)
{
    Items.AddNew(Material_Info);
    //Items.Sort( MAX_MATERIALINFO_COMPARE );
}

void MAX_MATERIAL_LIST::DeleteItem(int Index)
{
    Items.EraseDelete(Index);
}

void MAX_MATERIAL_LIST::Clear(void)
{
    Items.ClearDelete();
}

bool MAX_MATERIAL_LIST::operator == (MAX_MATERIAL_LIST & Max_Material_List)
{
    if (this->Items.Count() != Max_Material_List.Items.Count())
    {
        return false;
    }

    int iSize = (int)this->Items.Count();

    for ( int i = 0 ; i < iSize ; ++i )
    {
        if (!( *this->Items[i] == *Max_Material_List.Items[i] ))
        {
            return false;
        }
    }
    return true;
}

bool MAX_MATERIAL_LIST::SaveToFile(TKFile *pFile)
{
    int iSize = (int)Items.Count();
    pFile->Write(&iSize, sizeof(int), 1);

    for (int i = 0 ; i < iSize ; ++i )
    {
        Items[i]->SaveToFile(pFile);
    }
    return true;
}

bool MAX_MATERIAL_LIST::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;
    CString FormatBuffer;

    WriteBuffer.Format("MAX_MATERIAL_LIST\n");
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    int iSize = (int)Items.Count();
    WriteBuffer.Format("ItemCount:%d\n", iSize);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    for (int i = 0 ; i < iSize ; ++i )
    {
        Items[i]->SaveToText(pTextFile);
    }
    return true;
}


bool MAX_MATERIAL_LIST::LoadFromFile(TKFile *pFile)
{
    this->Clear();

    int iSize;
    pFile->Read(&iSize, sizeof(int), 1);

    MAX_MATERIALINFO *pMaterial_Info;
    for (int i = 0 ; i < iSize ; ++i)
    {
        pMaterial_Info = new MAX_MATERIALINFO;
        pMaterial_Info->LoadFromFile(pFile);
        Items.Add(pMaterial_Info);
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//MAX_NODE_TM
///////////////////////////////////////////////////////////////////////////////
MAX_NODE_TM::MAX_NODE_TM(void)
{
    this->Clear();
}

MAX_NODE_TM::MAX_NODE_TM(MAX_NODE_TM & Node_Tm)
{
    *this = Node_Tm;
}

MAX_NODE_TM::~MAX_NODE_TM(void)
{
    this->Clear();
}

MAX_NODE_TM & MAX_NODE_TM::operator = (MAX_NODE_TM & Node_Tm)
{
    this->Clear();
    strcpy(this->NodeName, Node_Tm.NodeName);
    this->InheritPos      = Node_Tm.InheritPos;
    this->InheritRot      = Node_Tm.InheritRot;
    this->InheritScl      = Node_Tm.InheritScl;
    this->Tm_Row0         = Node_Tm.Tm_Row0;
    this->Tm_Row1         = Node_Tm.Tm_Row1;
    this->Tm_Row2         = Node_Tm.Tm_Row2;
    this->Tm_Row3         = Node_Tm.Tm_Row3;
    this->Tm_Pos          = Node_Tm.Tm_Pos;
    this->Tm_RotAxis      = Node_Tm.Tm_RotAxis;
    this->Tm_RotAngle     = Node_Tm.Tm_RotAngle;
    this->Tm_Scale        = Node_Tm.Tm_Scale;
    this->Tm_ScaleAxis    = Node_Tm.Tm_ScaleAxis;
    this->Tm_ScaleAxisAng = Node_Tm.Tm_ScaleAxisAng;

    return Node_Tm;
}

bool MAX_NODE_TM::operator == (MAX_NODE_TM & Node_Tm)
{
    return 
        (strcmp(NodeName, NodeName) == 0    ) &&
        (InheritPos      == InheritPos      ) &&
        (InheritRot      == InheritRot      ) &&
        (InheritScl      == InheritScl      ) &&
        (Tm_Row0         == Tm_Row0         ) &&
        (Tm_Row1         == Tm_Row1         ) &&
        (Tm_Row2         == Tm_Row2         ) &&
        (Tm_Row3         == Tm_Row3         ) &&
        (Tm_Pos          == Tm_Pos          ) &&
        (Tm_RotAxis      == Tm_RotAxis      ) &&
        (Tm_RotAngle     == Tm_RotAngle     ) &&
        (Tm_Scale        == Tm_Scale        ) &&
        (Tm_ScaleAxis    == Tm_ScaleAxis    ) &&
        (Tm_ScaleAxisAng == Tm_ScaleAxisAng ) ;
}

bool MAX_NODE_TM::SaveToFile(TKFile *pFile)
{
    pFile->Write(this, sizeof(MAX_NODE_TM), 1);
    return true;
}

bool MAX_NODE_TM::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;
    CString FormatBuffer;

    FormatBuffer  = "MAX_NODE_TM\n";
    FormatBuffer += "NodeName:%s, ";
    FormatBuffer += "InheritPos.x:%f, ";
    FormatBuffer += "InheritPos.y:%f, ";
    FormatBuffer += "InheritPos.z:%f, ";
    FormatBuffer += "InheritRot.x:%f, ";
    FormatBuffer += "InheritRot.y:%f, ";
    FormatBuffer += "InheritRot.z:%f, ";
    FormatBuffer += "InheritScl.x:%f, ";
    FormatBuffer += "InheritScl.y:%f, ";
    FormatBuffer += "InheritScl.z:%f, ";
    FormatBuffer += "Tm_Row0.x:%f, ";
    FormatBuffer += "Tm_Row0.y:%f, ";
    FormatBuffer += "Tm_Row0.z:%f, ";
    FormatBuffer += "Tm_Row1.x:%f, ";
    FormatBuffer += "Tm_Row1.y:%f, ";
    FormatBuffer += "Tm_Row1.z:%f, ";
    FormatBuffer += "Tm_Row2.x:%f, ";
    FormatBuffer += "Tm_Row2.y:%f, ";
    FormatBuffer += "Tm_Row2.z:%f, ";
    FormatBuffer += "Tm_Row3.x:%f, ";
    FormatBuffer += "Tm_Row3.y:%f, ";
    FormatBuffer += "Tm_Row3.z:%f, ";
    FormatBuffer += "Tm_Pos.x:%f, ";
    FormatBuffer += "Tm_Pos.y:%f, ";
    FormatBuffer += "Tm_Pos.z:%f, ";
    FormatBuffer += "Tm_RotAxis.x:%f, ";
    FormatBuffer += "Tm_RotAxis.y:%f, ";
    FormatBuffer += "Tm_RotAxis.z:%f, ";
    FormatBuffer += "Tm_RotAngle:%f, ";
    FormatBuffer += "Tm_Scale.x:%f, ";
    FormatBuffer += "Tm_Scale.y:%f, ";
    FormatBuffer += "Tm_Scale.z:%f, ";
    FormatBuffer += "Tm_ScaleAxis.x:%f, ";
    FormatBuffer += "Tm_ScaleAxis.y:%f, ";
    FormatBuffer += "Tm_ScaleAxis.z:%f, ";
    FormatBuffer += "Tm_ScaleAxisAng:%f\n";

    WriteBuffer.Format(FormatBuffer.GetBuffer(), 
                        NodeName, 
                        InheritPos.x, InheritPos.y, InheritPos.z, 
                        InheritRot.x, InheritRot.y, InheritRot.z,
                        InheritScl.x, InheritScl.y, InheritScl.z,
                        Tm_Row0.x, Tm_Row0.y, Tm_Row0.z,
                        Tm_Row1.x, Tm_Row1.y, Tm_Row1.z,
                        Tm_Row2.x, Tm_Row2.y, Tm_Row2.z,
                        Tm_Row3.x, Tm_Row3.y, Tm_Row3.z,
                        Tm_Pos.x, Tm_Pos.y, Tm_Pos.z,
                        Tm_RotAxis.x, Tm_RotAxis.y, Tm_RotAxis.z,
                        Tm_RotAngle,
                        Tm_Scale.x, Tm_Scale.y, Tm_Scale.z,
                        Tm_ScaleAxis.x, Tm_ScaleAxis.y, Tm_ScaleAxis.z,
                        Tm_ScaleAxisAng);

    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    return true;
}

bool MAX_NODE_TM::LoadFromFile(TKFile *pFile)
{
    this->Clear();
    pFile->Read(this, sizeof(MAX_NODE_TM), 1);
    return true;
}

void MAX_NODE_TM::Clear(void)
{
    ZeroMemory(this, sizeof(MAX_NODE_TM));
}

///////////////////////////////////////////////////////////////////////////////
//MAX_MATRIX_WEIGHT
///////////////////////////////////////////////////////////////////////////////

MAX_MATRIX_WEIGHT::MAX_MATRIX_WEIGHT(void)
{
    this->Clear();
}

MAX_MATRIX_WEIGHT::MAX_MATRIX_WEIGHT(const MAX_MATRIX_WEIGHT & Matrix_Weight)
{
    (*this) = Matrix_Weight;
}

const MAX_MATRIX_WEIGHT & MAX_MATRIX_WEIGHT::operator = (const MAX_MATRIX_WEIGHT & Matrix_Weight)
{
    strcpy(this->ParentNodeName, Matrix_Weight.ParentNodeName);
    this->Weight = Matrix_Weight.Weight;
    
    return Matrix_Weight;
}

bool MAX_MATRIX_WEIGHT::operator == (MAX_MATRIX_WEIGHT & Matrix_Weight)
{
    bool Result;

    Result = (strcmp(this->ParentNodeName, Matrix_Weight.ParentNodeName) == 0) &&
             (this->Weight == Matrix_Weight.Weight);

    return Result;
}

void MAX_MATRIX_WEIGHT::Clear(void)
{
    ZeroMemory(this, sizeof(MAX_MATRIX_WEIGHT));
}

bool FILESTRUCT::MAX_MATRIX_WEIGHT_COMPARE(FILESTRUCT::MAX_MATRIX_WEIGHT & A, FILESTRUCT::MAX_MATRIX_WEIGHT & B)
{
    return (A.Weight > B.Weight);
}

///////////////////////////////////////////////////////////////////////////////
//MAX_VERTEX
///////////////////////////////////////////////////////////////////////////////

MAX_VERTEX::MAX_VERTEX(void)
:x(0.0f),y(0.0f),z(0.0f)
{
    this->Clear();
}

MAX_VERTEX::MAX_VERTEX(const MAX_VERTEX & Max_Vertex)
{
    (*this) = Max_Vertex;
}

const MAX_VERTEX & MAX_VERTEX::operator = (const MAX_VERTEX & Max_Vertex)
{
    this->x = Max_Vertex.x;
    this->y = Max_Vertex.y;
    this->z = Max_Vertex.z;
    MatrixWeights.clear();
    for ( int i = 0 ; i < (int)Max_Vertex.MatrixWeights.size() ; ++i )
    {
        MatrixWeights.push_back ( Max_Vertex.MatrixWeights[i] );
    }
    return Max_Vertex;
}

bool MAX_VERTEX::operator == (MAX_VERTEX & Max_Vertex)
{
    if (!(  (this->x == Max_Vertex.x) && 
            (this->y == Max_Vertex.y) && 
            (this->z == Max_Vertex.z)))
    {
        return false;
    }
    if ( MatrixWeights.size() != Max_Vertex.MatrixWeights.size())
    {
        return false;
    }
    for (int i = 0 ; i < (int)Max_Vertex.MatrixWeights.size() ; ++i )
    {
        if ((this->MatrixWeights[i] == Max_Vertex.MatrixWeights[i]) == false)
        {
            return false;
        }
    }
    return true;
}

void MAX_VERTEX::MatrixWeightReAllocate(void)
{
    // 버텍스 가중치를 무거운 순서대로 정렬.
    sort( MatrixWeights.begin(), MatrixWeights.end(), MAX_MATRIX_WEIGHT_COMPARE );

    // 이하 버텍스 가중치를 4개로 조정한다.
    // 가중치가 4개보다 적은 경우.
    MAX_MATRIX_WEIGHT Max_Matrix_Weight;
    float TotalWeight;

    // 모자라는 가중치를 채워 넣는다.
    MatrixWeights.reserve(4);
    while ( MatrixWeights.size() < 4 )
    {
        MatrixWeights.push_back( Max_Matrix_Weight );
    }

    // 초과하는 가중치를 제거한다.
    TotalWeight = 0.0f;
    MatrixWeights.resize(4);
    for ( int i = 0 ; i < 4 ; ++i )
    {
        TotalWeight += MatrixWeights[i].Weight;
    }
    
    // 가중치의 합을 구한다.
    MatrixWeights[0].Weight += 1.0f - TotalWeight;
}

bool MAX_VERTEX::SaveToFile(TKFile *pFile)
{
    MatrixWeightReAllocate();

    pFile->Write(&x, sizeof(x), 1);
    pFile->Write(&y, sizeof(y), 1);
    pFile->Write(&z, sizeof(z), 1);

    int iSize = (int)MatrixWeights.size();
    pFile->Write(&iSize, sizeof(int), 1);

    if (iSize > 0)
    {
        pFile->Write( &MatrixWeights[0], sizeof(MAX_MATRIX_WEIGHT) * iSize, 1  );
    }

    return true;
}

bool MAX_VERTEX::SaveToText(TKFile *pTextFile)
{
    MatrixWeightReAllocate();

    CString WriteBuffer;
    CString FormatBuffer;

    FormatBuffer = "MAX_VERTEX\n";
    FormatBuffer += "x:%f, y:%f, z:%f\n";
    WriteBuffer.Format(FormatBuffer.GetBuffer(), x, y, z);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    int iSize = (int)MatrixWeights.size();
    FormatBuffer = "WeightCount : %d\n";
    WriteBuffer.Format(FormatBuffer.GetBuffer(), iSize);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);    

    for (int i = 0 ; i < iSize ; ++i )
    {
        FormatBuffer = "    MAX_MATRIX_WEIGHT - ParentNodeName:%s, Weight: %f\n";
        WriteBuffer.Format(FormatBuffer.GetBuffer(), MatrixWeights[i].ParentNodeName, MatrixWeights[i].Weight);
        pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    }
    return true;
}

bool MAX_VERTEX::LoadFromFile(TKFile *pFile)
{
    pFile->Read(&x, sizeof(x), 1);
    pFile->Read(&y, sizeof(y), 1);
    pFile->Read(&z, sizeof(z), 1);

    int iSize = 0;
    pFile->Read(&iSize, sizeof(int), 1);
    MatrixWeights.resize( iSize );
    if (iSize > 0)
        pFile->Read( &MatrixWeights[0], sizeof(MAX_MATRIX_WEIGHT) * iSize, 1  );

    return true;
}

void MAX_VERTEX::AddWeight(MAX_MATRIX_WEIGHT & MatrixWeight)
{
    MatrixWeights.push_back( MatrixWeight );
    //sort( MatrixWeights.begin(), MatrixWeights.end(), MAX_MATRIX_WEIGHT_COMPARE );
}

void MAX_VERTEX::Clear(void)
{
    x = y = z = 0.0f;
    MatrixWeights.clear();
}

///////////////////////////////////////////////////////////////////////////////
//MAX_MESH_FACE
///////////////////////////////////////////////////////////////////////////////

MAX_MESH_FACE::MAX_MESH_FACE(void)
:Index(0)
,a(0)
,b(0)
,c(0)
,MtLid(0)
{    
}

MAX_MESH_FACE::MAX_MESH_FACE(const MAX_MESH_FACE & Mesh_Face)
{
    memcpy(this, &Mesh_Face, sizeof(MAX_MESH_FACE));
}

const MAX_MESH_FACE & MAX_MESH_FACE::operator = (const MAX_MESH_FACE & Mesh_Face)
{
    memcpy(this, &Mesh_Face, sizeof(MAX_MESH_FACE));
    return Mesh_Face;
}

bool MAX_MESH_FACE::operator == (MAX_MESH_FACE & Mesh_Face)
{
	return (this->a == Mesh_Face.a) &&
			(this->b == Mesh_Face.b) &&
			(this->c == Mesh_Face.c) &&
			(this->Index == Mesh_Face.Index) &&
			(this->MtLid == Mesh_Face.MtLid);
}

bool MAX_MESH_FACE::SaveToFile(TKFile *pFile)
{
    pFile->Write(this, sizeof(MAX_MESH_FACE), 1);
    return true;
}

bool MAX_MESH_FACE::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;
    CString FormatBuffer;

    FormatBuffer = "MAX_MESH_FACE\n";
    FormatBuffer += "Index:%d, a:%d, b:%d, c:%d, MtLid:%d\n";
    WriteBuffer.Format(FormatBuffer.GetBuffer(), Index, a, b, c, MtLid);
    
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    return true;
}

bool MAX_MESH_FACE::LoadFromFile(TKFile *pFile)
{
    pFile->Read(this, sizeof(MAX_MESH_FACE), 1);
    return true;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//MAX_MESH
///////////////////////////////////////////////////////////////////////////////

MAX_MESH::MAX_MESH(void)
{
    this->Clear();
}

MAX_MESH::MAX_MESH(MAX_MESH & Mesh)
{
    (*this) = Mesh;
}

MAX_MESH::~MAX_MESH(void)
{
    this->Clear();
}

void MAX_MESH::CreateOBB(void)
{
    D3DXVECTOR3S PosList;

    PosList.resize( this->PosWeightList.size() );
    for ( int i = 0 ; i < (int)PosList.size() ; ++i )
    {
        PosList[i].x = this->PosWeightList[i].x;
        PosList[i].y = this->PosWeightList[i].y;
        PosList[i].z = this->PosWeightList[i].z;
    }
    
    BuildOBB( PosList, vecCenter, vecAxis, AxisLen, vecOBB );

    // vecMin, vecMax 값 구하기.
    vecMin = D3DXVECTOR3(  FLT_MAX,  FLT_MAX,  FLT_MAX);
    vecMax = D3DXVECTOR3( -FLT_MAX, -FLT_MAX, -FLT_MAX);
    for ( int i = 0 ; i < (int)PosList.size() ; ++i )
    {
        vecMin.x = min( PosList[i].x, vecMin.x );
        vecMin.y = min( PosList[i].y, vecMin.y );
        vecMin.z = min( PosList[i].z, vecMin.z );
        
        vecMax.x = max( PosList[i].x, vecMax.x );
        vecMax.y = max( PosList[i].y, vecMax.y );
        vecMax.z = max( PosList[i].z, vecMax.z );
    }
}

void MAX_MESH::NormalSmoothing(void)
{
    //Smoothing
    //엑스포팅된 노말값이 정확하지 않으므로, 여기서 재 계산해주도록 한다.
    D3DXVECTOR3S FaceNormalList;
    D3DXVECTOR3 vecDir[2];
    D3DXVECTOR3 vec0, vec1, vec2;

    // 페이스 노말을 구한다
    FaceNormalList.resize( FaceList.size() );
    for ( int i = 0 ; i < (int)FaceList.size() ; ++i )
    {
        vec0.x = PosWeightList[ FaceList[i].a ].x;
        vec0.y = PosWeightList[ FaceList[i].a ].y;
        vec0.z = PosWeightList[ FaceList[i].a ].z;

        vec1.x = PosWeightList[ FaceList[i].b ].x;
        vec1.y = PosWeightList[ FaceList[i].b ].y;
        vec1.z = PosWeightList[ FaceList[i].b ].z;

        vec2.x = PosWeightList[ FaceList[i].c ].x;
        vec2.y = PosWeightList[ FaceList[i].c ].y;
        vec2.z = PosWeightList[ FaceList[i].c ].z;

        vecDir[0] = vec1 - vec0;
        vecDir[1] = vec2 - vec0;
        D3DXVec3Cross( &FaceNormalList[i], &vecDir[0], &vecDir[1] );
        D3DXVec3Normalize( &FaceNormalList[i], &FaceNormalList[i] );
    }
    
    // 구해진 페이스 노말에서 버텍스 노말을 구한다.
    NormalList.resize( PosWeightList.size() );
    if ( NormalList.size () > 0 )
    {
        memset( &NormalList[0], 0, sizeof(D3DXVECTOR3) * PosWeightList.size() );
        for ( int i = 0 ; i < (int)FaceList.size() ; ++i )
        {
            NormalList[ FaceList[i].a ] += FaceNormalList[i];
            NormalList[ FaceList[i].b ] += FaceNormalList[i];
            NormalList[ FaceList[i].c ] += FaceNormalList[i];
        }
        for ( int i = 0 ; i < (int)NormalList.size() ; ++i )
        {
            D3DXVec3Normalize( &NormalList[i], &NormalList[i] );
        }
    }

    // 구해진 버텍스 노말을 바탕으로 하여 버텍스 값이 같을 경우 동일한 버텍스로 간주하고.
    // 노말값의 평균을 구한다.
    map<D3DXVECTOR3, vector<D3DXVECTOR3*>*> NormalizeList;
    map<D3DXVECTOR3, vector<D3DXVECTOR3*>*>::iterator it;
    vector<D3DXVECTOR3*> * pSameNormalPointers;
    for ( int i = 0 ; i < (int)PosWeightList.size()-1 ; ++i )
    {
        it = NormalizeList.end();
        for ( it = NormalizeList.begin() ; it != NormalizeList.end() ; ++it )
        {
            if ( it->first.x == PosWeightList[i].x && 
                 it->first.y == PosWeightList[i].y && 
                 it->first.z == PosWeightList[i].z ) 
                break;
        }
        if ( it == NormalizeList.end() )
        {
            pSameNormalPointers = new vector<D3DXVECTOR3*>;
            pSameNormalPointers->push_back( &NormalList[i] );
            NormalizeList.insert( make_pair( D3DXVECTOR3( PosWeightList[i].x, 
                                                          PosWeightList[i].y, 
                                                          PosWeightList[i].z ), 
                                             pSameNormalPointers) );
        }
        else
        {
            it->second->push_back( &NormalList[i] );
        }
    }

    D3DXVECTOR3 vecSummery;
    for ( it = NormalizeList.begin() ; it != NormalizeList.end() ; ++it )
    {
        memset(&vecSummery, 0, sizeof(D3DXVECTOR3));
        pSameNormalPointers = it->second;
        for ( int i = 0 ; i < (int)pSameNormalPointers->size() ; ++i )
        {
            vecSummery += (*(*pSameNormalPointers)[i]);
        }
        D3DXVec3Normalize(&vecSummery, &vecSummery);
        for ( int i = 0 ; i < (int)pSameNormalPointers->size() ; ++i )
        {
            (*(*pSameNormalPointers)[i]) = vecSummery;
        }
        delete pSameNormalPointers;
    }
}

MAX_MESH & MAX_MESH::operator = (MAX_MESH & Mesh)
{
    this->Clear();

    for ( int i = 0 ; i < (int)Mesh.PosWeightList.size() ; ++i )
    {
        this->PosWeightList.push_back( Mesh.PosWeightList[i] );
    }
    this->TexPosList = Mesh.TexPosList;
    this->ColorList = Mesh.ColorList;
    this->NormalList = Mesh.NormalList;
    this->FaceList = Mesh.FaceList;

    int iSize = sizeof(vecCenter) + 
                sizeof(vecAxis) + 
                sizeof(AxisLen) + 
                sizeof(vecOBB) + 
                sizeof(vecMin) +
                sizeof(vecMax);
    memcpy(&this->vecCenter, &Mesh.vecCenter, iSize);

    return *this;
}

bool MAX_MESH::operator == (MAX_MESH & Mesh)
{
    int i;

    if (this->PosWeightList.size() != Mesh.PosWeightList.size())
    {
        return false;
    }
    for ( i = 0 ; i < (int)this->PosWeightList.size() ; ++i )
    {
        if ((this->PosWeightList[i]) == (Mesh.PosWeightList[i]) == false)
        {
            return false;
        }
    }

    if (this->TexPosList.size() != Mesh.TexPosList.size())
    {
        return false;
    }
    for ( i = 0 ; i < (int)this->TexPosList.size() ; ++i )
    {
        if ((this->TexPosList[i]) == (Mesh.TexPosList[i]) == false)
        {
            return false;
        }
    }

    if (this->ColorList.size() != Mesh.ColorList.size())
    {
        return false;
    }
    for ( i = 0 ; i < (int)this->ColorList.size() ; ++i )
    {
        if ((this->ColorList[i]) == (Mesh.ColorList[i]) == false)
        {
            return false;
        }
    }

    if (this->NormalList.size() != Mesh.NormalList.size())
    {
        return false;
    }
    for ( i = 0 ; i < (int)this->NormalList.size() ; ++i )
    {
        if ((this->NormalList[i]) == (Mesh.NormalList[i]) == false)
        {
            return false;
        }
    }

    if (this->FaceList.size() != Mesh.FaceList.size())
    {
        return false;
    }
    for ( i = 0 ; i < (int)this->FaceList.size() ; ++i )
    {
        if ((this->FaceList[i]) == (Mesh.FaceList[i]) == false)
        {
            return false;
        }
    }
    
    if ( this->vecCenter != Mesh.vecCenter ) return false;
    
    for ( i = 0 ; i < 3 ; ++i )
    {
        if ( this->vecAxis[i] != Mesh.vecAxis[i] ) return false;
    }
    
    for ( i = 0 ; i < 3 ; ++i )
    {
        if ( this->AxisLen[i] != Mesh.AxisLen[i] ) return false;
    }

    for ( i = 0 ; i < 8 ; ++i )
    {
        if ( this->vecOBB[i] != Mesh.vecOBB[i] ) return false;
    }

    if ( this->vecMin != vecMin ) return false;
    if ( this->vecMax != vecMax ) return false;

    return true;
}

bool MAX_MESH::SaveToFile(TKFile *pFile)
{
    CreateOBB();
    NormalSmoothing();

    int iSize;

    iSize = (int)PosWeightList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    for ( int i = 0 ; i < iSize ; ++i )
    {
        PosWeightList[i].SaveToFile(pFile);
    }

    iSize = (int)TexPosList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    pFile->Write(&TexPosList[0], sizeof(D3DXVECTOR3) * iSize, 1);

    iSize = (int)ColorList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    pFile->Write(&ColorList[0], sizeof(D3DXVECTOR3) * iSize, 1);

    iSize = (int)NormalList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    pFile->Write(&NormalList[0], sizeof(D3DXVECTOR3) * iSize, 1);

    iSize = (int)FaceList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    pFile->Write(&FaceList[0], sizeof(MAX_MESH_FACE) * iSize, 1);

    iSize = sizeof(vecCenter) + 
                sizeof(vecAxis) + 
                sizeof(AxisLen) + 
                sizeof(vecOBB) + 
                sizeof(vecMin) +
                sizeof(vecMax);
    pFile->Write(&vecCenter, iSize, 1);

    return true;
}

bool MAX_MESH::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;
    CString FormatBuffer;

    WriteBuffer.Format("MAX_MESH\n");
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    int iSize;

    iSize = (int)PosWeightList.size();
    WriteBuffer.Format("PosWeightListCount:%d\n", iSize);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    for ( int i = 0 ; i < iSize ; ++i )
    {
        PosWeightList[i].SaveToText(pTextFile);
    }

    iSize = (int)TexPosList.size();
    WriteBuffer.Format("TexPosListCount:%d\n", iSize);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    for ( int i = 0 ; i < iSize ; ++i )
    {
        WriteBuffer.Format("TexPosList: %f, %f, %f\n", TexPosList[i].x, TexPosList[i].y, TexPosList[i].z);
        pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    }

    iSize = (int)ColorList.size();
    WriteBuffer.Format("ColorListCount:%d\n", iSize);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    for ( int i = 0 ; i < iSize ; ++i )
    {
        WriteBuffer.Format("ColorList: %f, %f, %f\n", ColorList[i].x, ColorList[i].y, ColorList[i].z);
        pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    }

    iSize = (int)NormalList.size();
    WriteBuffer.Format("NormalListCount:%d\n", iSize);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    for ( int i = 0 ; i < iSize ; ++i )
    {
        WriteBuffer.Format("NormalList: %f, %f, %f\n", NormalList[i].x, NormalList[i].y, NormalList[i].z);
        pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    }

    iSize = (int)FaceList.size();
    WriteBuffer.Format("FaceListCount:%d\n", iSize);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    for ( int i = 0 ; i < iSize ; ++i )
    {
        FaceList[i].SaveToText(pTextFile);
    }

    WriteBuffer.Format("vecCenter: %f, %f, %f\n", vecCenter.x, vecCenter.y, vecCenter.z );
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    
    for ( int i = 0 ; i < 3 ; ++i )
    {
        WriteBuffer.Format("vecAxis %d: %f, %f ,%f\n", i, vecAxis[i].x, vecAxis[i].y, vecAxis[i].z );
        pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    }

    WriteBuffer.Format("AxisLen: %f, %f ,%f\n", AxisLen[0], AxisLen[1], AxisLen[2] );
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    
    for ( int i = 0 ; i < 8 ; ++i )
    {
        WriteBuffer.Format("vecOBB %d: %f, %f ,%f\n", i, vecOBB[i].x, vecOBB[i].y, vecOBB[i].z );
        pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    }

    WriteBuffer.Format("vecMin: %f, %f ,%f\n", vecMin.x, vecMin.y, vecMin.z );
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    WriteBuffer.Format("vecMax: %f, %f ,%f\n", vecMax.x, vecMax.y, vecMax.z );
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    return true;
}

bool MAX_MESH::LoadFromFile(TKFile *pFile)
{
    int iSize;

    pFile->Read(&iSize, sizeof(int), 1);
    PosWeightList.resize(iSize);
    for ( int i = 0 ; i < iSize ; ++i )
    {
        PosWeightList[i].LoadFromFile( pFile );
    }

    pFile->Read(&iSize, sizeof(int), 1);
    TexPosList.resize(iSize);
    if (iSize > 0) 
        pFile->Read(&TexPosList[0], sizeof(D3DXVECTOR3) * iSize, 1);

    pFile->Read(&iSize, sizeof(int), 1);
    ColorList.resize(iSize);
    if (iSize > 0) 
        pFile->Read(&ColorList[0], sizeof(D3DXVECTOR3) * iSize, 1);

    pFile->Read(&iSize, sizeof(int), 1);
    NormalList.resize(iSize);
    if (iSize > 0) 
        pFile->Read(&NormalList[0], sizeof(D3DXVECTOR3) * iSize, 1);

    pFile->Read(&iSize, sizeof(int), 1);
    FaceList.resize(iSize);
    if (iSize > 0) 
        pFile->Read(&FaceList[0], sizeof(MAX_MESH_FACE) * iSize, 1);

    iSize = sizeof(vecCenter) + 
                sizeof(vecAxis) + 
                sizeof(AxisLen) + 
                sizeof(vecOBB) + 
                sizeof(vecMin) +
                sizeof(vecMax);
    pFile->Read(&vecCenter, iSize, 1);

    return true;
}

void MAX_MESH::Add_PosWeight( MAX_VERTEX & PosWeight )
{
    PosWeightList.push_back( PosWeight );
}

void MAX_MESH::Add_TexPos(D3DXVECTOR3 & TexPos )
{
    TexPosList.push_back( TexPos );
}

void MAX_MESH::Add_Color (D3DXVECTOR3 & Color )
{
    ColorList.push_back( Color );
}

void MAX_MESH::Add_Normal (D3DXVECTOR3 & Normal )
{
    NormalList.push_back( Normal );
}

void MAX_MESH::Add_Face (MAX_MESH_FACE & Face )
{
    FaceList.push_back( Face );
}


void MAX_MESH::Clear(void)
{
    PosWeightList.clear();
    TexPosList.clear();
    ColorList.clear();
    NormalList.clear();
    FaceList.clear();
    
    int iSize = sizeof(vecCenter) + 
                sizeof(vecAxis) + 
                sizeof(AxisLen) + 
                sizeof(vecOBB) + 
                sizeof(vecMin) +
                sizeof(vecMax);
    memset(&this->vecCenter, 0, iSize);
}

///////////////////////////////////////////////////////////////////////////////
//MAX_CONTROL_POS
///////////////////////////////////////////////////////////////////////////////
MAX_CONTROL_POS::MAX_CONTROL_POS(void)
:TimeValue(0)
,x(0)
,y(0)
,z(0)
{
}

MAX_CONTROL_POS::MAX_CONTROL_POS(MAX_CONTROL_POS & Control_Pos)
{
    (*this) = Control_Pos;
}

MAX_CONTROL_POS & MAX_CONTROL_POS::operator = (MAX_CONTROL_POS & Control_Pos)
{
    this->TimeValue = Control_Pos.TimeValue;
    this->x = Control_Pos.x;
    this->y = Control_Pos.y;
    this->z = Control_Pos.z;
    
    return Control_Pos;
}

bool MAX_CONTROL_POS::operator == (MAX_CONTROL_POS & Control_Pos)
{
	return (this->TimeValue == Control_Pos.TimeValue) &&
		(this->x == Control_Pos.x) &&
		(this->y == Control_Pos.y) &&
		(this->z == Control_Pos.z);
}


bool MAX_CONTROL_POS::SaveToFile(TKFile *pFile)
{
    pFile->Write(this, sizeof(MAX_CONTROL_POS), 1);
    return true;
}

bool MAX_CONTROL_POS::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;

    WriteBuffer.Format("MAX_CONTROL_POS\n");
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    WriteBuffer.Format("TimeValue:%d, x:%f, y:%f, z:%f\n", TimeValue, x, y, z);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    return true;
}

bool MAX_CONTROL_POS::LoadFromFile(TKFile *pFile)
{
    pFile->Read(this, sizeof(MAX_CONTROL_POS), 1);
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//MAX_CONTROL_ROT
///////////////////////////////////////////////////////////////////////////////
MAX_CONTROL_ROT::MAX_CONTROL_ROT(void)
:TimeValue(0)
,x(0)
,y(0)
,z(0)
,w(0)
{
}

MAX_CONTROL_ROT::MAX_CONTROL_ROT(MAX_CONTROL_ROT & Control_Rot)
{
    (*this) = Control_Rot;
}

MAX_CONTROL_ROT & MAX_CONTROL_ROT::operator = (MAX_CONTROL_ROT & Control_Rot)
{
    this->TimeValue = Control_Rot.TimeValue;
    this->x         = Control_Rot.x        ;
    this->y         = Control_Rot.y        ;
    this->z         = Control_Rot.z        ;
    this->w         = Control_Rot.w        ;

    return Control_Rot;
}

bool MAX_CONTROL_ROT::operator == (MAX_CONTROL_ROT & Control_Rot)
{
	return (this->TimeValue == Control_Rot.TimeValue) &&
		(this->w == Control_Rot.w) &&
		(this->x == Control_Rot.x) &&
		(this->y == Control_Rot.y) &&
		(this->z == Control_Rot.z);
}

bool MAX_CONTROL_ROT::SaveToFile(TKFile *pFile)
{
    pFile->Write(this, sizeof(MAX_CONTROL_ROT), 1);
    return true;
}

bool MAX_CONTROL_ROT::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;

    WriteBuffer = "MAX_CONTROL_ROT\n";
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    WriteBuffer.Format("TimeValue:%d, x:%f, y:%f, z:%f, w:%f\n", TimeValue, x, y, z, w);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);    

    return true;
}

bool MAX_CONTROL_ROT::LoadFromFile(TKFile *pFile)
{
    pFile->Read(this, sizeof(MAX_CONTROL_ROT), 1);
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//MAX_CONTROL_SCALE
///////////////////////////////////////////////////////////////////////////////

MAX_CONTROL_SCALE::MAX_CONTROL_SCALE(void)
{
}

MAX_CONTROL_SCALE::MAX_CONTROL_SCALE(MAX_CONTROL_SCALE & Control_Scale)
{
    (*this) = Control_Scale;
}

MAX_CONTROL_SCALE & MAX_CONTROL_SCALE::operator = (MAX_CONTROL_SCALE & Control_Scale)
{
    this->TimeValue  = Control_Scale.TimeValue;
    this->x          = Control_Scale.x        ;
    this->y          = Control_Scale.y        ;
    this->z          = Control_Scale.z        ;
    this->AxisX      = Control_Scale.AxisX    ;
    this->AxisY      = Control_Scale.AxisY    ;
    this->AxisZ      = Control_Scale.AxisZ    ;
    this->Radian     = Control_Scale.Radian   ;

    return Control_Scale;
}

bool MAX_CONTROL_SCALE::operator == (MAX_CONTROL_SCALE & Control_Scale)
{
    return  (this->AxisX == Control_Scale.AxisX) &&
            (this->AxisY == Control_Scale.AxisY) &&
            (this->AxisZ== Control_Scale.AxisZ) &&
            (this->Radian == Control_Scale.Radian) &&
            (this->TimeValue == Control_Scale.TimeValue) &&
            (this->x == Control_Scale.x) &&
            (this->y == Control_Scale.y) &&
            (this->z == Control_Scale.z);
}


bool MAX_CONTROL_SCALE::SaveToFile(TKFile *pFile)
{
    pFile->Write(this, sizeof(MAX_CONTROL_SCALE), 1);
    return true;
}

bool MAX_CONTROL_SCALE::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;

    WriteBuffer.Format("MAX_CONTROL_SCALE\n");
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    WriteBuffer.Format("TimeValue:%d, x:%f, y:%f, z:%f, AxisX:%f, AxisY:%f, AxisZ:%f, Radian:%f\n",
                        TimeValue, x, y, z, AxisX, AxisY, AxisZ, Radian );
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    return true;
}

bool MAX_CONTROL_SCALE::LoadFromFile(TKFile *pFile)
{
    pFile->Read(this, sizeof(MAX_CONTROL_SCALE), 1);
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//MAX_ANIMATRIX
///////////////////////////////////////////////////////////////////////////////

MAX_ANIMATRIX::MAX_ANIMATRIX(void)
{

}

MAX_ANIMATRIX::MAX_ANIMATRIX(const MAX_ANIMATRIX & AniMatrix)
{
    memcpy(this, &AniMatrix, sizeof(MAX_ANIMATRIX));
}

const MAX_ANIMATRIX & MAX_ANIMATRIX::operator = (const MAX_ANIMATRIX & AniMatrix)
{
    memcpy(this, &AniMatrix, sizeof(MAX_ANIMATRIX));
    return AniMatrix;
}

bool MAX_ANIMATRIX::operator == (MAX_ANIMATRIX & AniMatrix)
{
    return  (this->TimeValue    == AniMatrix.TimeValue) &&
            (this->matAni       == AniMatrix.matAni);
}

bool MAX_ANIMATRIX::SaveToFile(TKFile *pFile)
{
    pFile->Write(this, sizeof(MAX_ANIMATRIX), 1);
    return true;
}

bool MAX_ANIMATRIX::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;
    CString FormatBuffer;

    WriteBuffer.Format("MAX_ANIMATRIX\n");
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    FormatBuffer = "    TimeValue:%d\n";
    FormatBuffer += "   00:%f, 01:%f, 02:%f, 03:%f\n";
    FormatBuffer += "   10:%f, 11:%f, 12:%f, 13:%f\n";
    FormatBuffer += "   20:%f, 21:%f, 22:%f, 23:%f\n";
    FormatBuffer += "   30:%f, 31:%f, 32:%f, 33:%f\n";

    WriteBuffer.Format( FormatBuffer.GetBuffer(), 
                        TimeValue,
                        matAni._11, matAni._12, matAni._13, matAni._14,
                        matAni._21, matAni._22, matAni._23, matAni._24,
                        matAni._31, matAni._32, matAni._33, matAni._34,
                        matAni._41, matAni._42, matAni._43, matAni._44 );
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    return true;
}

bool MAX_ANIMATRIX::LoadFromFile(TKFile *pFile)
{
    pFile->Read(this, sizeof(MAX_ANIMATRIX), 1);
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//MAX_TM_ANIMATION
///////////////////////////////////////////////////////////////////////////////
MAX_TM_ANIMATION::MAX_TM_ANIMATION(void)
{
    /*
    Control_Scale_List.SetCapacity(6000);
    Control_Rot_List.SetCapacity(6000);
    Control_Rot_List.SetCapacity(6000);
    */
    this->Clear();
}

MAX_TM_ANIMATION::MAX_TM_ANIMATION(MAX_TM_ANIMATION & Tm_Animation)
{
    /*
    Control_Scale_List.SetCapacity(6000);
    Control_Rot_List.SetCapacity(6000);
    Control_Rot_List.SetCapacity(6000);
    */
    *this = Tm_Animation;
}

MAX_TM_ANIMATION::~MAX_TM_ANIMATION(void)
{
    this->Clear();
}

MAX_TM_ANIMATION & MAX_TM_ANIMATION::operator = (MAX_TM_ANIMATION & Tm_Animation)
{
    this->AniMatrix_List = Tm_Animation.AniMatrix_List;
    return Tm_Animation;
}

bool MAX_TM_ANIMATION::operator == (MAX_TM_ANIMATION & Tm_Animation)
{
    if (this->AniMatrix_List.size() != Tm_Animation.AniMatrix_List.size() )
    {
        return false;
    }
    for ( int i = 0 ; i < (int)this->AniMatrix_List.size() ; ++i )
    {
        if ((this->AniMatrix_List[i] == Tm_Animation.AniMatrix_List[i]) == false)
        {
            return false;
        }
    }

    return true;
}

bool MAX_TM_ANIMATION::SaveToFile(TKFile *pFile)
{
    int iSize;

    iSize = (int)AniMatrix_List.size();
    pFile->Write(&iSize, sizeof(int), 1);
    pFile->Write(&AniMatrix_List[0], sizeof(MAX_ANIMATRIX) * iSize, 1);

    return true;
}

bool MAX_TM_ANIMATION::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;

    WriteBuffer.Format("MAX_TM_ANIMATION\n");
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    int iSize;
    iSize = (int)AniMatrix_List.size();

    WriteBuffer.Format("AniMatrix_ListCount:%d\n", iSize);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    for ( int i = 0 ; i < iSize ; ++i )
    {
        AniMatrix_List[i].SaveToText(pTextFile);
    };

    return true;
}

bool MAX_TM_ANIMATION::LoadFromFile(TKFile *pFile)
{
    int iSize;

    pFile->Read(&iSize, sizeof(int), 1);
    AniMatrix_List.resize(iSize);
    if (iSize > 0) 
        pFile->Read(&AniMatrix_List[0], sizeof(MAX_ANIMATRIX) * iSize, 1);

    return true;
}

void MAX_TM_ANIMATION::Add_AniMatrix(MAX_ANIMATRIX &AniMatrix)
{
    AniMatrix_List.push_back(AniMatrix);
}

void MAX_TM_ANIMATION::Clear(void)
{
    AniMatrix_List.clear();
}

///////////////////////////////////////////////////////////////////////////////
//MAX_GEOMOBJECT
///////////////////////////////////////////////////////////////////////////////
MAX_GEOMOBJECT::MAX_GEOMOBJECT(void)
{
    this->Clear();
}

MAX_GEOMOBJECT::MAX_GEOMOBJECT(MAX_GEOMOBJECT & GeomObject)
{
    (*this) = GeomObject;
}

MAX_GEOMOBJECT::~MAX_GEOMOBJECT(void)
{
    this->Clear();
}

MAX_GEOMOBJECT & MAX_GEOMOBJECT::operator = (MAX_GEOMOBJECT & GeomObject)
{
    this->Clear();

    this->Index                 = GeomObject.Index;
    strcpy(this->NodeName       , GeomObject.NodeName);
    strcpy(this->ParentNodeName , GeomObject.ParentNodeName);
    this->Node_Tm               = GeomObject.Node_Tm;
    this->Mesh                  = GeomObject.Mesh;
    this->MotionBlur            = GeomObject.MotionBlur;
    this->CastShadow            = GeomObject.CastShadow;
    this->RecvShadow            = GeomObject.RecvShadow;
    this->Tm_Animation          = GeomObject.Tm_Animation;
    this->WireframeColor        = GeomObject.WireframeColor;
    this->BoundingBox_Min       = GeomObject.BoundingBox_Min;
    this->BoundingBox_Max       = GeomObject.BoundingBox_Max;
    this->Material_Reference    = GeomObject.Material_Reference;
    this->SubMaterial_Reference = GeomObject.SubMaterial_Reference;
    strcpy(this->MaterialName, GeomObject.MaterialName);
    strcpy(this->SubMaterialName, GeomObject.SubMaterialName);
    this->GeomObjectType        = GeomObject.GeomObjectType;

    return GeomObject;
}

bool MAX_GEOMOBJECT::operator == (MAX_GEOMOBJECT & GeomObject)
{
    bool result =
        (this->Index                == GeomObject.Index              ) &&
        (strcmp(this->NodeName   ,GeomObject.NodeName  ) == 0        ) &&
        (strcmp(this->ParentNodeName, GeomObject.ParentNodeName) == 0) &&
        (this->Node_Tm              == GeomObject.Node_Tm            ) &&
        (this->Mesh                 == GeomObject.Mesh               ) &&
        (this->MotionBlur           == GeomObject.MotionBlur         ) &&
        (this->CastShadow           == GeomObject.CastShadow         ) &&
        (this->RecvShadow           == GeomObject.RecvShadow         ) &&
        (this->Tm_Animation         == GeomObject.Tm_Animation       ) &&
        (this->WireframeColor       == GeomObject.WireframeColor     ) &&
        (this->BoundingBox_Min      == GeomObject.BoundingBox_Min    ) &&
        (this->BoundingBox_Max      == GeomObject.BoundingBox_Max    ) &&
        (this->Material_Reference   == GeomObject.Material_Reference ) &&
        (this->SubMaterial_Reference== GeomObject.SubMaterial_Reference) &&
        (this->Material_Reference   == GeomObject.Material_Reference ) &&
        (strcmp(this->MaterialName, GeomObject.MaterialName) == 0    ) &&
        (strcmp(this->SubMaterialName, GeomObject.SubMaterialName) == 0    ) &&
        (this->GeomObjectType       == GeomObject.GeomObjectType     );

    if (result == false)
    {
        return false;
    }

    return true;
}

bool MAX_GEOMOBJECT::SaveToFile(TKFile *pFile)
{
    pFile->Write(&Index, sizeof(int), 1);
    pFile->Write(NodeName, sizeof(char)*32, 1);
    pFile->Write(ParentNodeName, sizeof(ParentNodeName), 1);

    Node_Tm.SaveToFile(pFile);
    Mesh.SaveToFile(pFile);

    pFile->Write(&MotionBlur, sizeof(int), 1);
    pFile->Write(&CastShadow, sizeof(int), 1);
    pFile->Write(&RecvShadow, sizeof(int), 1);
    Tm_Animation.SaveToFile(pFile);
    pFile->Write(&WireframeColor, sizeof(D3DXCOLOR), 1);

    pFile->Write(&BoundingBox_Min, sizeof(D3DXVECTOR3), 1);
    pFile->Write(&BoundingBox_Max, sizeof(D3DXVECTOR3), 1);
    pFile->Write(&Material_Reference, sizeof(int), 1);
    pFile->Write(&SubMaterial_Reference, sizeof(int), 1);
    pFile->Write(MaterialName, sizeof(MaterialName), 1);
    pFile->Write(SubMaterialName, sizeof(SubMaterialName), 1);
    pFile->Write(&GeomObjectType, sizeof(EGeomObjectType), 1);

    return true;
}






bool MAX_GEOMOBJECT::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;
    CString FormatBuffer;

    WriteBuffer.Format("MAX_GEOMOBJECT\n");
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    WriteBuffer.Format("Index:%d, NodeName:%s, ParentNodeName:%s\n", Index, NodeName, ParentNodeName);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    Node_Tm.SaveToText(pTextFile);
    Mesh.SaveToText(pTextFile);

    WriteBuffer.Format("MotionBlur:%d, CastShadow:%d, RecvShadow:%d\n", MotionBlur, CastShadow, RecvShadow);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    Tm_Animation.SaveToText(pTextFile);

    WriteBuffer.Format("WireframeColor a:%f, r:%f, g:%f, b:%f \n", 
                        WireframeColor.a, WireframeColor.r, WireframeColor.g, WireframeColor.b );
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    WriteBuffer.Format("BoundingBox_Min x:%f, y:%f, z:%f \n", BoundingBox_Min.x, BoundingBox_Min.y, BoundingBox_Min.z );
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    WriteBuffer.Format("BoundingBox_Max x:%f, y:%f, z:%f \n", BoundingBox_Max.x, BoundingBox_Max.y, BoundingBox_Max.z );
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    WriteBuffer.Format("Material_Reference :%d, SubMaterial_Reference : %d, MaterialName :%s, SubMaterialName :%s \n", Material_Reference, SubMaterial_Reference, MaterialName, SubMaterialName );
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    WriteBuffer.Format("GeomObjectType :%d\n", GeomObjectType );
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    return true;
}

bool MAX_GEOMOBJECT::LoadFromFile(TKFile *pFile)
{
    this->Clear();

    pFile->Read(&Index, sizeof(int), 1);
    pFile->Read(NodeName, sizeof(char)*32, 1);
    pFile->Read(ParentNodeName, sizeof(ParentNodeName), 1);

    Node_Tm.LoadFromFile(pFile);
    Mesh.LoadFromFile(pFile);

    pFile->Read(&MotionBlur, sizeof(int), 1);
    pFile->Read(&CastShadow, sizeof(int), 1);
    pFile->Read(&RecvShadow, sizeof(int), 1);
    Tm_Animation.LoadFromFile(pFile);
    pFile->Read(&WireframeColor, sizeof(D3DXCOLOR), 1);

    pFile->Read(&BoundingBox_Min, sizeof(D3DXVECTOR3), 1);
    pFile->Read(&BoundingBox_Max, sizeof(D3DXVECTOR3), 1);
    pFile->Read(&Material_Reference, sizeof(int), 1);
    pFile->Read(&SubMaterial_Reference, sizeof(int), 1);
    pFile->Read(MaterialName, sizeof(MaterialName), 1);
    pFile->Read(SubMaterialName, sizeof(SubMaterialName), 1);
    pFile->Read(&GeomObjectType, sizeof(EGeomObjectType), 1);

    return true;
}

void MAX_GEOMOBJECT::Clear(void)
{
    Index = -1;
    ZeroMemory(NodeName, sizeof(NodeName));
    ZeroMemory(ParentNodeName, sizeof(ParentNodeName));

    Node_Tm.Clear();
    Mesh.Clear();
    MotionBlur = 0;
    CastShadow = 0;
    RecvShadow = 0;
    Tm_Animation.Clear();
    WireframeColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

    BoundingBox_Min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    BoundingBox_Max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    Material_Reference = -1;
    SubMaterial_Reference = -1;
    memset(MaterialName, 0, sizeof(MaterialName));
    memset(SubMaterialName, 0, sizeof(SubMaterialName));

    GeomObjectType = egotMesh;    
}

void MAX_GEOMOBJECT::BindMaterialList(MAX_MATERIAL_LIST & MaterialList)
{
    MAX_MATERIALINFO *pMaterial = NULL, *pSubMaterial = NULL;

    if ( this->Material_Reference >= 0 && this->Material_Reference < MaterialList.Items.Count() )
    {
        pMaterial = MaterialList.Items[ this->Material_Reference ];
        strcpy(this->MaterialName, pMaterial->MaterialName);
        //pMaterial = MaterialList.Items[ this->Material_Reference ];
        //if ( strcmp( pMaterial->MaterialName, this->MaterialName ) != 0 )
        //{
        //    this->Material_Reference = -1;
        //    memset(this->MaterialName, 0, sizeof(this->MaterialName));
        //}
    }
    else
    {
        this->Material_Reference = -1;
        memset(this->MaterialName, 0, sizeof(this->MaterialName));
    }

    if ( pMaterial )
    {
        if ( this->SubMaterial_Reference >= 0 && 
                this->SubMaterial_Reference < pMaterial->Items.Count() )
        {
            pSubMaterial = pMaterial->Items[ this->SubMaterial_Reference ];
            strcpy(this->SubMaterialName, pSubMaterial->MaterialName);
        }
        else
        {
            this->SubMaterial_Reference = -1;
            memset(this->SubMaterialName, 0, sizeof(this->SubMaterialName));
        }
    }
    else
    {
        this->SubMaterial_Reference = -1;
        memset(this->SubMaterialName, 0, sizeof(this->SubMaterialName));
    }
}

///////////////////////////////////////////////////////////////////////////////
//MAX_GEOMOBJECT_LIST
///////////////////////////////////////////////////////////////////////////////

MAX_GEOMOBJECT_LIST::MAX_GEOMOBJECT_LIST(void)
{
    this->Clear();
}

MAX_GEOMOBJECT_LIST::~MAX_GEOMOBJECT_LIST(void)
{
    this->Clear();
}

void MAX_GEOMOBJECT_LIST::AddItem(MAX_GEOMOBJECT & GeomObject)
{
    Items.AddNew( GeomObject );
}

void MAX_GEOMOBJECT_LIST::DeleteItem(int Index)
{
    Items.EraseDelete( Index );
}

void MAX_GEOMOBJECT_LIST::Clear(void)
{
    Items.ClearDelete();
}

bool MAX_GEOMOBJECT_LIST::operator == (MAX_GEOMOBJECT_LIST & Max_GeomObject_List)
{
    if (this->Items.Count() != Max_GeomObject_List.Items.Count())
    {
        return false;
    }
    for ( int i = 0 ; i < this->Items.Count() ; ++i)
    {
        if ((*this->Items[i] == *Max_GeomObject_List.Items[i]) == false)
        {
            return false;
        }
    }

    return true;
}

bool MAX_GEOMOBJECT_LIST::SaveToFile(TKFile *pFile)
{
    int iSize = (int)Items.Count();
    pFile->Write(&iSize, sizeof(int), 1);
    for ( int i = 0 ; i < Items.Count() ; ++i )
    {
        Items[i]->SaveToFile(pFile);
    }

    return true;
}

bool MAX_GEOMOBJECT_LIST::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;

    WriteBuffer = "MAX_GEOMOBJECT_LIST\n";
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    int iSize = (int)Items.Count();

    WriteBuffer.Format("ItemCount:%d\n", iSize);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    for ( int i = 0 ; i < Items.Count() ; ++i )
    {
        Items[i]->SaveToText(pTextFile);
    }

    return true;
}

bool MAX_GEOMOBJECT_LIST::LoadFromFile(TKFile *pFile)
{
    this->Clear();

    int iSize;
    pFile->Read(&iSize, sizeof(int), 1);

    MAX_GEOMOBJECT *pGeomObject;

    for ( int i = 0 ; i < iSize ; ++i )
    {
        pGeomObject = new MAX_GEOMOBJECT;
        pGeomObject->LoadFromFile(pFile);
        Items.Add(pGeomObject);
    }

    return true;
}

void MAX_GEOMOBJECT_LIST::BindMaterialList(MAX_MATERIAL_LIST & MaterialList)
{
    for ( int i = 0 ; i < Items.Count() ; ++i )
    {
        Items[i]->BindMaterialList(MaterialList);
    }
}

///////////////////////////////////////////////////////////////////////////////
// ST_MESHLODINFO
///////////////////////////////////////////////////////////////////////////////
ST_MESHLODINFO::ST_MESHLODINFO(void)
{
    Level = -1;
    memset(NodeName, 0, sizeof(NodeName));
}

ST_MESHLODINFO::ST_MESHLODINFO(const ST_MESHLODINFO & St_MeshLodInfo)
{
    (*this) = St_MeshLodInfo;
}

const ST_MESHLODINFO & ST_MESHLODINFO::operator = (const ST_MESHLODINFO & St_MeshLodInfo)
{
    memcpy(this, &St_MeshLodInfo, sizeof(ST_MESHLODINFO));
    
    return St_MeshLodInfo;
}

bool ST_MESHLODINFO::operator == (ST_MESHLODINFO & St_MeshLodInfo)
{
    if ( (this->Level == St_MeshLodInfo.Level) && 
         (strcmp(this->NodeName, St_MeshLodInfo.NodeName) == 0) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////

ST_MESHLODLIST::ST_MESHLODLIST(void)
{
    this->Clear();
}

ST_MESHLODLIST::~ST_MESHLODLIST(void)
{
    this->Clear();
}

void ST_MESHLODLIST::AddItem(ST_MESHLODINFO & St_MeshLodInfo)
{
    Items.push_back( St_MeshLodInfo );
}

void ST_MESHLODLIST::DeleteItem(int Index)
{
    ST_MESHLODINFOS::iterator it = Items.begin();
    it += Index;
    Items.erase( it );
}

void ST_MESHLODLIST::Clear(void)
{
    Items.clear();
}

bool ST_MESHLODLIST::operator == (ST_MESHLODLIST & St_MeshLodList)
{
    if (this->Items.size() != St_MeshLodList.Items.size())
    {
        return false;
    }

    for ( int i = 0 ; i < (int)this->Items.size() ; ++i)
    {
        if ((this->Items[i] == St_MeshLodList.Items[i]) == false)
        {
            return false;
        }
    }
    
    return true;
}

bool ST_MESHLODLIST::SaveToFile(TKFile *pFile)
{
    int iSize = (int)Items.size();
    pFile->Write(&iSize, sizeof(int), 1);
    pFile->Write(&Items[0], sizeof(ST_MESHLODINFO) * iSize, 1);

    return true;
}

bool ST_MESHLODLIST::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;

    WriteBuffer = "ST_MESHLODINFO\n";
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    int iSize = (int)Items.size();
    WriteBuffer.Format("ItemsCount\n", iSize);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    for ( int i = 0 ; i < iSize ; ++i )
    {
        WriteBuffer.Format("Level:%d NodeName:%s\n", Items[i].Level, Items[i].NodeName );
        pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);
    }

    return true;
}

bool ST_MESHLODLIST::LoadFromFile(TKFile *pFile)
{
    int iSize;
    pFile->Read(&iSize, sizeof(int), 1);
    Items.resize( iSize );
    if (iSize > 0)
        pFile->Read(&Items[0], sizeof(ST_MESHLODINFO) * iSize, 1);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//ST_ANIMOTIONINFO
///////////////////////////////////////////////////////////////////////////////
ST_ANIMOTIONINFO::ST_ANIMOTIONINFO(void)
{
    ZeroMemory(this, sizeof(ST_ANIMOTIONINFO));
    Index = -1;
}

ST_ANIMOTIONINFO::ST_ANIMOTIONINFO(const ST_ANIMOTIONINFO & St_AniMationInfo)
{
    memcpy(this, &St_AniMationInfo, sizeof(ST_ANIMOTIONINFO));
}

const ST_ANIMOTIONINFO & ST_ANIMOTIONINFO::operator = (const ST_ANIMOTIONINFO & St_AniMationInfo)
{
    memcpy(this, &St_AniMationInfo, sizeof(ST_ANIMOTIONINFO));
    
    return St_AniMationInfo;
}

bool ST_ANIMOTIONINFO::operator == (ST_ANIMOTIONINFO & St_AniMationInfo)
{
    return  (this->Index        == St_AniMationInfo.Index      ) &&
            (this->StartFrame   == St_AniMationInfo.StartFrame ) &&
            (this->EndFrame     == St_AniMationInfo.EndFrame   ) &&
            (strcmp(this->AnimationName, St_AniMationInfo.AnimationName) == 0);
}

bool ST_ANIMOTIONINFO::SaveToFile(TKFile *pFile)
{
    pFile->Write(this, sizeof(ST_ANIMOTIONINFO), 1);
    return true;
}

bool ST_ANIMOTIONINFO::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;

    WriteBuffer = "ST_ANIMOTIONINFO\n";
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    WriteBuffer.Format("Index:%d, Start:%d, End:%d, AniName:%s\n", Index, StartFrame, EndFrame, AnimationName);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    return true;
}

bool ST_ANIMOTIONINFO::LoadFromFile(TKFile *pFile)
{
    pFile->Read(this, sizeof(ST_ANIMOTIONINFO), 1);
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//ST_ANIMOTIONLIST
///////////////////////////////////////////////////////////////////////////////

ST_ANIMOTIONLIST::ST_ANIMOTIONLIST(void)
{
    this->Clear();
}

ST_ANIMOTIONLIST::~ST_ANIMOTIONLIST(void)
{
    this->Clear();
}

void ST_ANIMOTIONLIST::AddItem(ST_ANIMOTIONINFO & AniMationInfo)
{
    Items.push_back( AniMationInfo );
}

void ST_ANIMOTIONLIST::DeleteItem(int Index)
{
    ST_ANIMOTIONINFOS::iterator it = Items.begin();
    it += Index;
    Items.erase( it );
}

void ST_ANIMOTIONLIST::Clear(void)
{
    Items.clear();
}

bool ST_ANIMOTIONLIST::operator == (ST_ANIMOTIONLIST & St_AniMotionList)
{
    if (this->Items.size() != St_AniMotionList.Items.size())
    {
        return false;
    }

    for ( int i = 0 ; i < (int)this->Items.size() ; ++i)
    {
        if ((this->Items[i] == St_AniMotionList.Items[i]) == false)
        {
            return false;
        }
    }
    return true;
}

bool ST_ANIMOTIONLIST::SaveToFile(TKFile *pFile)
{
    int iSize = (int)Items.size();
    pFile->Write(&iSize, sizeof(int), 1);
    if ( iSize > 0 )
    {
        pFile->Write(&Items[0], sizeof(ST_ANIMOTIONINFO) * iSize, 1);
    }
    return true;
}

bool ST_ANIMOTIONLIST::SaveToText(TKFile *pTextFile)
{
    CString WriteBuffer;

    WriteBuffer = "ST_ANIMOTIONLIST\n";
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    int iSize = (int)Items.size();
    WriteBuffer.Format("ItemsCount\n", iSize);
    pTextFile->Write(WriteBuffer.GetBuffer(), WriteBuffer.GetLength(), 1);

    for ( int i = 0 ; i < iSize ; ++i )
    {
        Items[i].SaveToText(pTextFile);
    }

    return true;
}

bool ST_ANIMOTIONLIST::LoadFromFile(TKFile *pFile)
{
    int iSize;
    pFile->Read(&iSize, sizeof(int), 1);
    Items.resize( iSize );
    if (iSize > 0)
        pFile->Read(&Items[0], sizeof(ST_ANIMOTIONINFO) * iSize, 1);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//ST_COLLISION
///////////////////////////////////////////////////////////////////////////////
ST_COLLISION::ST_COLLISION(void)
{
    this->Clear();
}

ST_COLLISION::~ST_COLLISION(void)
{
    this->Clear();
}

void ST_COLLISION::Clear(void)
{
    ObjectType = eotObject;

    memset(NodeName, 0, sizeof(NodeName));
    memset(BoneNodeName, 0, sizeof(BoneNodeName));
    
    PosList.clear();
    NormalList.clear();
    FaceList.clear();
    FaceNormalList.clear();

    memset(&Center, 0, sizeof(D3DXVECTOR3));
    memset(&Min, 0, sizeof(D3DXVECTOR3));
    memset(&Max, 0, sizeof(D3DXVECTOR3));
    Radius = 0.0f;

    memset(AxisLen, 0, sizeof(AxisLen));
    memset(vecAxisDir, 0, sizeof(vecAxisDir));
    memset(vecOBB, 0, sizeof(vecOBB));
    
    IsDynamicObject = false;
    IsMeshAnimation = false;
    
    vector<ST_COLLISION*>::iterator it;
    for ( it = Items.begin() ; it != Items.end() ; ++it )
    {
        delete (*it);
    }
    Items.clear();
}

void ST_COLLISION::SaveToFile(TKFile *pFile)
{
    pFile->Write( &ObjectType, sizeof(ETKObjectType), 1);

    pFile->Write( NodeName, sizeof(NodeName), 1);
    pFile->Write( BoneNodeName, sizeof(BoneNodeName), 1);
    
    int iSize;

    iSize = (int)PosList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    if ( iSize > 0 ) pFile->Write(&PosList[0], sizeof(D3DXVECTOR3) * iSize, 1);
    
    iSize = (int)NormalList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    if ( iSize > 0 ) pFile->Write(&NormalList[0], sizeof(D3DXVECTOR3) * iSize, 1);

    iSize = (int)FaceList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    if ( iSize > 0 ) pFile->Write(&FaceList[0], sizeof(ST_IB_TRIANGLE) * iSize, 1);

    iSize = (int)FaceNormalList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    if ( iSize > 0 ) pFile->Write(&FaceNormalList[0], sizeof(D3DXVECTOR3) * iSize, 1);
    
    pFile->Write(&Center    , sizeof(D3DXVECTOR3)   , 1);
    pFile->Write(&Min       , sizeof(D3DXVECTOR3)   , 1);
    pFile->Write(&Max       , sizeof(D3DXVECTOR3)   , 1);
    pFile->Write(&Radius    , sizeof(float)         , 1);
    
    pFile->Write(AxisLen    , sizeof(AxisLen)       , 1);
    pFile->Write(vecAxisDir , sizeof(vecAxisDir)    , 1);
    pFile->Write(vecOBB     , sizeof(vecOBB)        , 1);
    
    pFile->Write(&IsDynamicObject, sizeof(bool), 1);
    pFile->Write(&IsMeshAnimation, sizeof(bool), 1);

    iSize = (int)Items.size();
    pFile->Write(&iSize, sizeof(int), 1);
    for ( int i = 0 ; i < iSize ; ++i )
    {
        Items[i]->SaveToFile( pFile );
    }
}

void ST_COLLISION::SaveToText(TKFile *pTextFile)
{
}

void ST_COLLISION::LoadFromFile(TKFile *pFile)
{
    this->Clear();
    
    pFile->Read( &ObjectType, sizeof(ETKObjectType), 1);

    pFile->Read( NodeName, sizeof(NodeName), 1);
    pFile->Read( BoneNodeName, sizeof(BoneNodeName), 1);
    
    int iSize;
    
    pFile->Read( &iSize, sizeof(int), 1);
    PosList.resize( iSize );
    if ( iSize > 0 ) pFile->Read(&PosList[0], sizeof(D3DXVECTOR3) * iSize, 1);
    
    pFile->Read( &iSize, sizeof(int), 1);
    NormalList.resize( iSize );
    if ( iSize > 0 ) pFile->Read(&NormalList[0], sizeof(D3DXVECTOR3) * iSize, 1);

    pFile->Read( &iSize, sizeof(int), 1);
    FaceList.resize( iSize );
    if ( iSize > 0 ) pFile->Read(&FaceList[0], sizeof(ST_IB_TRIANGLE) * iSize, 1);

    pFile->Read( &iSize, sizeof(int), 1);
    FaceNormalList.resize( iSize );
    if ( iSize > 0 ) pFile->Read(&FaceNormalList[0], sizeof(D3DXVECTOR3) * iSize, 1);

    pFile->Read(&Center    , sizeof(D3DXVECTOR3)   , 1);
    pFile->Read(&Min       , sizeof(D3DXVECTOR3)   , 1);
    pFile->Read(&Max       , sizeof(D3DXVECTOR3)   , 1);
    pFile->Read(&Radius    , sizeof(float)         , 1);
    
    pFile->Read(AxisLen    , sizeof(AxisLen)        , 1);
    pFile->Read(vecAxisDir , sizeof(vecAxisDir)     , 1);
    pFile->Read(vecOBB     , sizeof(vecOBB)         , 1);
    
    pFile->Read(&IsDynamicObject, sizeof(bool), 1);
    pFile->Read(&IsMeshAnimation, sizeof(bool), 1);

    pFile->Read( &iSize, sizeof(int), 1);
    Items.resize( iSize );
    for ( int i = 0 ; i < iSize ; ++i )
    {
        Items[i] = new ST_COLLISION;
        Items[i]->LoadFromFile( pFile );
    }        
}

///////////////////////////////////////////////////////////////////////////////
//ST_MATERIAL
///////////////////////////////////////////////////////////////////////////////
static int ST_MATERIAL_SIZE = 0;
ST_MATERIAL::ST_MATERIAL(void)
{
    if ( ST_MATERIAL_SIZE == 0 )
    {
        ST_MATERIAL_SIZE =  sizeof(NodeName)+
                            sizeof(TextureName)+
                            sizeof(AlphaBlend)+
                            sizeof(AlphaBlend_Src)+
                            sizeof(AlphaBlend_Dest)+
                            sizeof(AlphaBlend_OP)+
                            sizeof(AlphaTest)+
                            sizeof(AlphaTestFunc)+
                            sizeof(AlphaTestRef)+
                            sizeof(TwoSided)+
                            sizeof(ZWriteEnable);
    }
    this->Clear();
}

ST_MATERIAL::~ST_MATERIAL(void)
{
    this->Clear();
}

void ST_MATERIAL::Clear(void)
{
    memset(this, 0, ST_MATERIAL_SIZE);
    for ( int i = 0 ; i < (int)SubMaterialList.size() ; ++i )
    {
        delete SubMaterialList[i];
    }
    SubMaterialList.clear();
}

void ST_MATERIAL::SaveToFile(TKFile *pFile)
{
    pFile->Write(this, ST_MATERIAL_SIZE, 1);
    
    int iSize = (int)SubMaterialList.size();
    pFile->Write(&iSize, sizeof(iSize), 1);
    if ( iSize > 0 )
    {
        for ( int i = 0 ; i < (int)SubMaterialList.size() ; ++i )
        {
            SubMaterialList[i]->SaveToFile(pFile);
        }
    }
}

void ST_MATERIAL::SaveToText(TKFile *pTextFile)
{
}

void ST_MATERIAL::LoadFromFile(TKFile *pFile)
{
    this->Clear();
    
    pFile->Read(this, ST_MATERIAL_SIZE, 1);
    
    int iSize;
    pFile->Read(&iSize, sizeof(iSize), 1);
    if ( iSize > 0 )
    {
        SubMaterialList.resize(iSize);
        for ( int i = 0 ; i < iSize ; ++i )
        {
            SubMaterialList[i] = new ST_MATERIAL;
            SubMaterialList[i]->LoadFromFile(pFile);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//ST_GEOMETRY
///////////////////////////////////////////////////////////////////////////////
static int ST_GEOMETRY_SIZE = 0;
ST_GEOMETRY::ST_GEOMETRY(void)
{
    if ( ST_GEOMETRY_SIZE == 0 )
    {
        ST_GEOMETRY_SIZE =  sizeof(NodeName) +
                            sizeof(BoneNodeName) +
                            sizeof(AnimationMatrixIndex) +
                            sizeof(vecCenter) +
                            sizeof(vecAxis) +
                            sizeof(AxisLen) +
                            sizeof(vecOBB) +
                            sizeof(vecMin) +
                            sizeof(vecMax) +
                            sizeof(MeshVertexesType) +
                            sizeof(MaterialName) +
                            sizeof(SubMaterialName) +
                            sizeof(ShaderName);
    }
    this->Clear();
}

ST_GEOMETRY::~ST_GEOMETRY(void)
{
    this->Clear();
}

void ST_GEOMETRY::Clear(void)
{
    memset(this, 0, ST_GEOMETRY_SIZE );

    OriginPosList.clear();
    PosList.clear();
    MatrixWeightList.clear();
    MatrixIndexList.clear();
    TexPosList.clear();
    ColorList.clear();
    NormalList.clear();
    FaceList.clear();
    AniMatrixList.clear();
}

void ST_GEOMETRY::SaveToFile(TKFile *pFile)
{
    pFile->Write(this, ST_GEOMETRY_SIZE, 1);
    
    int iSize;

    iSize = (int)OriginPosList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    if ( iSize > 0 )
    {
        pFile->Write( &OriginPosList[0], sizeof(ST_VERTEX) * iSize, 1 );
    }
    
    iSize = (int)PosList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    if ( iSize > 0 )
    {
        pFile->Write( &PosList[0], sizeof(D3DXVECTOR3) * iSize, 1 );
    }

    iSize = (int)MatrixWeightList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    if ( iSize > 0 )
    {
        pFile->Write( &MatrixWeightList[0], sizeof(D3DXVECTOR4) * iSize, 1 );
    }

    iSize = (int)MatrixIndexList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    if ( iSize > 0 )
    {
        pFile->Write( &MatrixIndexList[0], sizeof(ST_MESHMATRIXINDEX) * iSize, 1 );
    }

    iSize = (int)TexPosList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    if ( iSize > 0 )
    {
        pFile->Write( &TexPosList[0], sizeof(D3DXVECTOR2) * iSize, 1 );
    }

    iSize = (int)ColorList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    if ( iSize > 0 )
    {
        pFile->Write( &ColorList[0], sizeof(D3DCOLOR) * iSize, 1 );
    }

    iSize = (int)NormalList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    if ( iSize > 0 )
    {
        pFile->Write( &NormalList[0], sizeof(D3DXVECTOR3) * iSize, 1 );
    }

    iSize = (int)FaceList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    if ( iSize > 0 )
    {
        pFile->Write( &FaceList[0], sizeof(ST_IB_TRIANGLE) * iSize, 1 );
    }

    iSize = (int)AniMatrixList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    if ( iSize > 0 )
    {
        pFile->Write( &AniMatrixList[0], sizeof(ST_ANIMATRIX) * iSize, 1 );
    }
}

void ST_GEOMETRY::SaveToText(TKFile *pTextFile)
{
}

void ST_GEOMETRY::LoadFromFile(TKFile *pFile)
{
    this->Clear();
    
    pFile->Read(this, ST_GEOMETRY_SIZE, 1);
    
    int iSize;

    pFile->Read(&iSize, sizeof(int), 1);
    OriginPosList.resize(iSize);
    if ( iSize > 0 )
    {
        pFile->Read( &OriginPosList[0], sizeof(ST_VERTEX) * iSize, 1 );
    }
    
    pFile->Read(&iSize, sizeof(int), 1);
    PosList.resize(iSize);
    if ( iSize > 0 )
    {
        pFile->Read( &PosList[0], sizeof(D3DXVECTOR3) * iSize, 1 );
    }
    
    pFile->Read(&iSize, sizeof(int), 1);
    MatrixWeightList.resize(iSize);
    if ( iSize > 0 )
    {
        pFile->Read( &MatrixWeightList[0], sizeof(D3DXVECTOR4) * iSize, 1 );
    }
    
    pFile->Read(&iSize, sizeof(int), 1);
    MatrixIndexList.resize(iSize);
    if ( iSize > 0 )
    {
        pFile->Read( &MatrixIndexList[0], sizeof(ST_MESHMATRIXINDEX) * iSize, 1 );
    }
    
    pFile->Read(&iSize, sizeof(int), 1);
    TexPosList.resize(iSize);
    if ( iSize > 0 )
    {
        pFile->Read( &TexPosList[0], sizeof(D3DXVECTOR2) * iSize, 1 );
    }
    
    pFile->Read(&iSize, sizeof(int), 1);
    ColorList.resize(iSize);
    if ( iSize > 0 )
    {
        pFile->Read( &ColorList[0], sizeof(D3DCOLOR) * iSize, 1 );
    }
    
    pFile->Read(&iSize, sizeof(int), 1);
    NormalList.resize(iSize);
    if ( iSize > 0 )
    {
        pFile->Read( &NormalList[0], sizeof(D3DXVECTOR3) * iSize, 1 );
    }
    
    pFile->Read(&iSize, sizeof(int), 1);
    FaceList.resize(iSize);
    if ( iSize > 0 )
    {
        pFile->Read( &FaceList[0], sizeof(ST_IB_TRIANGLE) * iSize, 1 );
    }                            
    
    pFile->Read(&iSize, sizeof(int), 1);
    AniMatrixList.resize(iSize);
    if ( iSize > 0 )
    {
        pFile->Read( &AniMatrixList[0], sizeof(ST_ANIMATRIX) * iSize, 1 );
    }                            
}

///////////////////////////////////////////////////////////////////////////////
//ST_GEOMETRY
///////////////////////////////////////////////////////////////////////////////
static int ST_MESHRES_SIZE = 0;
ST_MESHRES::ST_MESHRES(void)
{
    if ( ST_MESHRES_SIZE == 0 )
    {
        ST_MESHRES_SIZE =   sizeof(NodeName) +
                            sizeof(FirstFrame) +
                            sizeof(LastFrame) +
                            sizeof(FrameSpeed);
    }
    this->Clear();
}

ST_MESHRES::~ST_MESHRES(void)
{
    this->Clear();
}

void ST_MESHRES::Clear(void)
{
    memset(this, 0, ST_MESHRES_SIZE);
    for ( int i = 0 ; i < (int)MaterialList.size() ; ++i )
    {
        MaterialList[i]->Clear();
        delete MaterialList[i];
    }
    MaterialList.clear();
    for ( int i = 0 ; i < (int)GeomList.size() ; ++i )
    {
        GeomList[i]->Clear();
        delete GeomList[i];
    }
    GeomList.clear();
    for ( int i = 0 ; i < (int)BoneList.size() ; ++i )
    {
        BoneList[i]->Clear();
        delete BoneList[i];
    }
    BoneList.clear();
}

void ST_MESHRES::SaveToFile(TKFile *pFile)
{
    pFile->Write(this, ST_MESHRES_SIZE, 1);
    
    int iSize;
    
    iSize = (int)MaterialList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    for ( int i = 0 ; i < iSize ; ++i )
    {
        MaterialList[i]->SaveToFile(pFile);
    }
    
    iSize = (int)GeomList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    for ( int i = 0 ; i < iSize ; ++i )
    {
        GeomList[i]->SaveToFile(pFile);    
    }

    iSize = (int)BoneList.size();
    pFile->Write(&iSize, sizeof(int), 1);
    for ( int i = 0 ; i < iSize ; ++i )
    {
        BoneList[i]->SaveToFile(pFile);    
    }
}

void ST_MESHRES::SaveToText(TKFile *pTextFile)
{
}

void ST_MESHRES::LoadFromFile(TKFile *pFile)
{
    pFile->Read(this, ST_MESHRES_SIZE, 1);
    
    int iSize;
    
    pFile->Read(&iSize, sizeof(int), 1);
    MaterialList.resize(iSize);
    for ( int i = 0 ; i < iSize ; ++i )
    {
        MaterialList[i] = new ST_MATERIAL;
        MaterialList[i]->LoadFromFile(pFile);    
    }
    
    pFile->Read(&iSize, sizeof(int), 1);
    GeomList.resize(iSize);
    for ( int i = 0 ; i < iSize ; ++i )
    {
        GeomList[i] = new ST_GEOMETRY;
        GeomList[i]->LoadFromFile(pFile);    
    }

    pFile->Read(&iSize, sizeof(int), 1);
    BoneList.resize(iSize);
    for ( int i = 0 ; i < iSize ; ++i )
    {
        BoneList[i] = new ST_GEOMETRY;
        BoneList[i]->LoadFromFile(pFile);    
    }
}