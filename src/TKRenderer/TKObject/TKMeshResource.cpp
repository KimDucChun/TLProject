#include "TKMeshResource.h"
#include "TKEngine.h"
#include "TKD3D.h"
#include "TKTime.h"
#include "TKTexture.h"
#include "TKFileConvertClasses.h"


///////////////////////////////////////////////////////////////////////////////
//TKMeshMaterial
///////////////////////////////////////////////////////////////////////////////
TKMeshMaterial::TKMeshMaterial(string NodeName)
    :TKEngineResource(-1)
    ,AlphaBlend(false)
{
    SetObjectType( eotMeshMaterial );
    SetNodeName( NodeName );

    for (int i = 0; i < MATERIAL_TEXTURE_COUNT ; ++i )
    {
        pTextures[i] = NULL;
    }

    AlphaBlend = false;
    AlphaBlend_Src = D3DBLEND_ONE;
    AlphaBlend_Dest = D3DBLEND_ZERO;
    AlphaBlend_OP = D3DBLENDOP_ADD;
    AlphaTest = false;
    AlphaTestFunc = D3DCMP_GREATER;
    AlphaTestRef = 0;
    TwoSided = false;
    ZWriteEnable = true;
}

TKMeshMaterial::~TKMeshMaterial(void)
{
#if 0
	MATERIALLIST::iterator it;
    for ( it = MaterialList.begin() ; it != MaterialList.end() ; ++it )
    {
        delete it->second;
    }
    for (int i = 0; i < MATERIAL_TEXTURE_COUNT ; ++i )
    {
        if (pTextures[i])
        {
            R_TEXTUREMANAGER->ReleaseTexture(pTextures[i]);
        }
    }
#endif
}

const TKMeshMaterial & TKMeshMaterial::operator = (const TKMeshMaterial & MeshMaterial)
{
#if 0
	TKEngineResource::operator=((TKEngineResource)MeshMaterial);

    TKMeshMaterial *pMeshMaterial;

    MATERIALLIST::iterator it;
    MATERIALLIST::const_iterator cit;
    for ( it = MaterialList.begin() ; it != MaterialList.end() ; ++it )
    {
        delete it->second;
    }
    MaterialList.clear();
    for (   cit = MeshMaterial.MaterialList.begin() ;
        cit != MeshMaterial.MaterialList.end() ;
        ++cit )
    {
        pMeshMaterial = this->NewMaterial( cit->second->GetNodeName() );
        (*pMeshMaterial) = (*cit->second);
    }

    for ( int i = 0 ; i < MATERIAL_TEXTURE_COUNT ; ++i )
    {
        this->pTextures[i] = NULL;
        if ( MeshMaterial.pTextures[i] )
        {
            this->pTextures[i] = R_TEXTUREMANAGER->LoadTexture( MeshMaterial.pTextures[i]->GetTextureFileName() );
        }
    }
    this->AlphaBlend = MeshMaterial.AlphaBlend;
    this->AlphaTest = MeshMaterial.AlphaTest;
    this->AlphaTestFunc = MeshMaterial.AlphaTestFunc;
    this->AlphaTestRef = MeshMaterial.AlphaTestRef;
    this->TwoSided = MeshMaterial.TwoSided;

#endif
	return MeshMaterial;
}

TKMeshMaterial * TKMeshMaterial::NewMaterial(string MaterialName)
{
#if 0
	TKMeshMaterial *pMaterial;
    MATERIALLIST::iterator it;

    if ( MaterialName.empty() ) return NULL;
    it = MaterialList.find( MaterialName );
    if ( it != MaterialList.end() ) return NULL;

    pMaterial = new TKMeshMaterial(GetEngine(), MaterialName);
    MaterialList.insert( make_pair( pMaterial->GetNodeName(), pMaterial ) );

    return pMaterial;
#else
	return NULL;
#endif
}

void TKMeshMaterial::DeleteMaterial(TKMeshMaterial * pMaterial)
{
    // TODO : �� ��ü�� ����ϴ� ������Ʈ���� ��ü �����͸� NULL �� �����Ѵ�

    MATERIALLIST::iterator it;
    GEOMLIST::iterator itGeom;

    if ( pMaterial )
    {
        for ( it = MaterialList.begin() ; it != MaterialList.end() ; ++it )
        {
            if ( it->second == pMaterial )
            {
                MaterialList.erase(it);
                break;
            }
        }

        pMaterial->Release();
        delete pMaterial;
    }
}

void TKMeshMaterial::DeleteMaterial(string MaterialName)
{
    // TODO : �� ��ü�� ����ϴ� ������Ʈ���� ��ü �����͸� NULL �� �����Ѵ�

    TKMeshMaterial *pMaterial;
    MATERIALLIST::iterator it;
    GEOMLIST::iterator itGeom;

    it = MaterialList.find( MaterialName );
    if ( it != MaterialList.end() )
    {
        pMaterial = it->second;
        MaterialList.erase( it );

        pMaterial->Release();
        delete pMaterial;
    }
}

////////////////////////////////////////////////////////////////////////////////
// TKMeshVertexes
////////////////////////////////////////////////////////////////////////////////
TKMeshVertexes::TKMeshVertexes(int ID)
    :TKVertexResource(ID)
    ,BoneNodeName("")
    ,AnimationMatrixIndex(0)
    ,IsSelected(false)
{
    SetObjectType( eotMeshVertexes );
}

TKMeshVertexes::~TKMeshVertexes(void)
{
}

bool TKMeshVertexes::InitVertexFormat(void)
{
    // ���ؽ����� ������ �����Ѵ�.
    D3DVERTEXELEMENT9 verElement[10];
    ZeroMemory(verElement, sizeof(D3DVERTEXELEMENT9) * 10);
    int verElementCnt = 0;

    // Position
    ///////////////////////////////////////////////////////
    if (!PosList.empty())
    {
        verElement[verElementCnt].Stream = verElementCnt;
        verElement[verElementCnt].Type = D3DDECLTYPE_FLOAT3;
        verElement[verElementCnt].Method = D3DDECLMETHOD_DEFAULT;
        verElement[verElementCnt].Usage = D3DDECLUSAGE_POSITION;
        ++verElementCnt;

        // Position
        ///////////////////////////////////////////////////////
        if (!MatrixWeightList.empty())
        {
            verElement[verElementCnt].Stream = verElementCnt;
            verElement[verElementCnt].Type = D3DDECLTYPE_FLOAT4;
            verElement[verElementCnt].Method = D3DDECLMETHOD_DEFAULT;
            verElement[verElementCnt].Usage = D3DDECLUSAGE_BLENDWEIGHT;
            ++verElementCnt;
        }

        // AnimationMatrix
        ///////////////////////////////////////////////////////
        if (!MatrixIndexList.empty())
        {
            verElement[verElementCnt].Stream = verElementCnt;
            verElement[verElementCnt].Type = D3DDECLTYPE_UBYTE4;
            verElement[verElementCnt].Method = D3DDECLMETHOD_DEFAULT;
            verElement[verElementCnt].Usage = D3DDECLUSAGE_BLENDINDICES;
            ++verElementCnt;
        }
        // TextureCoord
        ///////////////////////////////////////////////////////
        if (!TexPosList.empty())
        {
            verElement[verElementCnt].Stream = verElementCnt;
            verElement[verElementCnt].Type = D3DDECLTYPE_FLOAT2;
            verElement[verElementCnt].Method = D3DDECLMETHOD_DEFAULT;
            verElement[verElementCnt].Usage = D3DDECLUSAGE_TEXCOORD;
            ++verElementCnt;
        }
        // DiffuseColor
        ///////////////////////////////////////////////////////
        if (!ColorList.empty())
        {
            verElement[verElementCnt].Stream = verElementCnt;
            verElement[verElementCnt].Type = D3DDECLTYPE_D3DCOLOR;
            verElement[verElementCnt].Method = D3DDECLMETHOD_DEFAULT;
            verElement[verElementCnt].Usage = D3DDECLUSAGE_COLOR;
            ++verElementCnt;
        }
        // Normal
        ///////////////////////////////////////////////////////
        if (!NormalList.empty())
        {
            verElement[verElementCnt].Stream = verElementCnt;
            verElement[verElementCnt].Type = D3DDECLTYPE_FLOAT3;
            verElement[verElementCnt].Method = D3DDECLMETHOD_DEFAULT;
            verElement[verElementCnt].Usage = D3DDECLUSAGE_NORMAL;
            ++verElementCnt;
        }
    }

    if (verElementCnt > 0)
    {
        ++verElementCnt;

        D3DVERTEXELEMENT9 *pVerElement;
        pVerElement = new D3DVERTEXELEMENT9[verElementCnt];
        memcpy(pVerElement, &verElement, sizeof(D3DVERTEXELEMENT9) * verElementCnt);

        pVerElement[verElementCnt-1].Stream = 0xFF;
        pVerElement[verElementCnt-1].Offset = 0;
        pVerElement[verElementCnt-1].Type = D3DDECLTYPE_UNUSED;
        pVerElement[verElementCnt-1].Method = 0;
        pVerElement[verElementCnt-1].Usage = 0;
        pVerElement[verElementCnt-1].UsageIndex = 0;

#ifdef TK_D3D9
		if ( FAILED ( R_D3DDEVICE->CreateVertexDeclaration(pVerElement, &pVDEC) ) )
        {
            delete[verElementCnt] pVerElement;
            return false;
        }
        else
#endif
        {
            delete[verElementCnt] pVerElement;
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool TKMeshVertexes::InitVertexBuffers(void)
{
#ifdef TK_D3D9
	int BufferSize;

    // Position
    ///////////////////////////////////////////////////////
    if (!PosList.empty())
    {
        BufferSize = (int)PosList.size() * sizeof(D3DXVECTOR3);
        if ( FAILED ( R_D3DDEVICE->CreateVertexBuffer ( BufferSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &VertexBuffers.pVertexBuffer, NULL ) ) )
        {
            return false;
        }
        D3DXVECTOR3 *pVertexBuffer;
        VertexBuffers.pVertexBuffer->Lock( 0, BufferSize, (void**)&pVertexBuffer, 0 );
        memcpy(pVertexBuffer, &(PosList[0]), BufferSize);
        VertexBuffers.pVertexBuffer->Unlock();

        // MatrixWeight
        ///////////////////////////////////////////////////////
        if (!MatrixWeightList.empty())
        {
            BufferSize = (int)MatrixWeightList.size() * sizeof(D3DXVECTOR4);
            if ( FAILED ( R_D3DDEVICE->CreateVertexBuffer ( BufferSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &VertexBuffers.pMatrixWeight, NULL ) ) )
            {
                return false;
            }
            D3DXVECTOR4 *pMatrixWeight;
            VertexBuffers.pMatrixWeight->Lock( 0, BufferSize, (void**)&pMatrixWeight, 0 );
            memcpy(pMatrixWeight, &(MatrixWeightList[0]), BufferSize);
            VertexBuffers.pMatrixWeight->Unlock();
        }

        // MatrixIndex
        ///////////////////////////////////////////////////////
        if (!MatrixIndexList.empty())
        {
            BufferSize = (int)MatrixIndexList.size() * sizeof(ST_MESHMATRIXINDEX);
            if ( FAILED ( R_D3DDEVICE->CreateVertexBuffer ( BufferSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &VertexBuffers.pMatrixIndex, NULL ) ) )
            {
                return false;
            }
            ST_MESHMATRIXINDEX *pMatrixIndex;
            VertexBuffers.pMatrixIndex->Lock( 0, BufferSize, (void**)&pMatrixIndex, 0 );
            memcpy(pMatrixIndex, &(MatrixIndexList[0]), BufferSize);
            VertexBuffers.pMatrixIndex->Unlock();
        }

        // TexPosList
        ///////////////////////////////////////////////////////
        if (!TexPosList.empty())
        {
            BufferSize = (int)TexPosList.size() * sizeof(D3DXVECTOR2);
            if ( FAILED ( R_D3DDEVICE->CreateVertexBuffer ( BufferSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &VertexBuffers.pTexelBuffer, NULL ) ) )
            {
                return false;
            }
            D3DXVECTOR2 *pTexel;
            VertexBuffers.pTexelBuffer->Lock( 0, BufferSize, (void**)&pTexel, 0 );
            memcpy(pTexel, &(TexPosList[0]), BufferSize);
            VertexBuffers.pTexelBuffer->Unlock();
        }

        // ColorList
        ///////////////////////////////////////////////////////
        if (!ColorList.empty())
        {
            BufferSize = (int)ColorList.size() * sizeof(D3DCOLOR);
            if ( FAILED ( R_D3DDEVICE->CreateVertexBuffer ( BufferSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &VertexBuffers.pColorBuffer, NULL ) ) )
            {
                return false;
            }
            D3DCOLOR *pVColor;
            VertexBuffers.pColorBuffer->Lock( 0, BufferSize, (void**)&pVColor, 0 );
            memcpy(pVColor, &(ColorList[0]), BufferSize);
            VertexBuffers.pColorBuffer->Unlock();
        }

        // NormalList
        ///////////////////////////////////////////////////////
        if (!NormalList.empty())
        {
            BufferSize = (int)NormalList.size() * sizeof(D3DXVECTOR3);
            if ( FAILED ( R_D3DDEVICE->CreateVertexBuffer ( BufferSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &VertexBuffers.pNormalBuffer, NULL ) ) )
            {
                return false;
            }
            D3DXVECTOR3 *pNormal;
            VertexBuffers.pNormalBuffer->Lock( 0, BufferSize, (void**)&pNormal, 0 );
            memcpy(pNormal, &(NormalList[0]), BufferSize);
            VertexBuffers.pNormalBuffer->Unlock();
        }

#ifdef DEBUG_MODE
        // OBB
        ///////////////////////////////////////////////////////
        BufferSize = sizeof(ST_VB_POSDIF) * 8;

        ST_VB_POSDIF *pOBBVB;

        if ( FAILED ( R_D3DDEVICE->CreateVertexBuffer( BufferSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &VertexBuffers.pOBBVB, NULL ) ) )
        {
            return false;
        }
        VertexBuffers.pOBBVB->Lock( 0, BufferSize, (void**)&pOBBVB, 0 );
        for ( int i = 0 ; i < 8 ; ++i )
        {
            (*pOBBVB).Pos = vecOBB[i];
            (*pOBBVB).Color = D3DCOLOR_ARGB(255, 127, 127, 0);
            pOBBVB++;
        }
        VertexBuffers.pOBBVB->Unlock();

        // OBB_Selected
        ///////////////////////////////////////////////////////
        BufferSize = sizeof(ST_VB_POSDIF) * 8;
        if ( FAILED ( R_D3DDEVICE->CreateVertexBuffer( BufferSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &VertexBuffers.pOBBVB_Selected, NULL ) ) )
        {
            return false;
        }
        VertexBuffers.pOBBVB_Selected->Lock( 0, BufferSize, (void**)&pOBBVB, 0 );
        for ( int i = 0 ; i < 8 ; ++i )
        {
            (*pOBBVB).Pos = vecOBB[i];
            (*pOBBVB).Color = D3DCOLOR_ARGB(255, 255, 255, 127);
            pOBBVB++;
        }
        VertexBuffers.pOBBVB_Selected->Unlock();

        // OBB IndexBuffer
        ///////////////////////////////////////////////////////
        BufferSize = sizeof(ST_IB_LINE) * 12;
        if (FAILED( R_D3DDEVICE->CreateIndexBuffer( BufferSize,
            D3DUSAGE_WRITEONLY,
            D3DFMT_INDEX16,
            D3DPOOL_DEFAULT,
            &VertexBuffers.pOBBIB,
            NULL)))
        {
            return false;
        }
        ST_IB_LINE *pLineIndex;
        VertexBuffers.pOBBIB->Lock(0, BufferSize, (void**)&pLineIndex, 0);
        pLineIndex->_0 = 0;     pLineIndex->_1 = 1;     ++pLineIndex;
        pLineIndex->_0 = 1;     pLineIndex->_1 = 2;     ++pLineIndex;
        pLineIndex->_0 = 2;     pLineIndex->_1 = 3;     ++pLineIndex;
        pLineIndex->_0 = 3;     pLineIndex->_1 = 0;     ++pLineIndex;
        pLineIndex->_0 = 0;     pLineIndex->_1 = 4;     ++pLineIndex;
        pLineIndex->_0 = 1;     pLineIndex->_1 = 5;     ++pLineIndex;
        pLineIndex->_0 = 2;     pLineIndex->_1 = 6;     ++pLineIndex;
        pLineIndex->_0 = 3;     pLineIndex->_1 = 7;     ++pLineIndex;
        pLineIndex->_0 = 4;     pLineIndex->_1 = 5;     ++pLineIndex;
        pLineIndex->_0 = 5;     pLineIndex->_1 = 6;     ++pLineIndex;
        pLineIndex->_0 = 6;     pLineIndex->_1 = 7;     ++pLineIndex;
        pLineIndex->_0 = 7;     pLineIndex->_1 = 4;     ++pLineIndex;
        VertexBuffers.pOBBIB->Unlock();
#endif DEBUG_MODE
    }
#endif
    return true;
}

void TKMeshVertexes::Init(void)
{
    __super::Init();

#if 0

    switch ( MeshVertexesType )
    {
    case emvtMESH:
        {
            if ( GetPosList().size() != GetOriginVertexList().size() )  // ������ �� ������ ������ �ع���.
            {
                GetPosList().resize(GetOriginVertexList().size());
                GetMatrixWeightList().resize(GetOriginVertexList().size());
                GetMatrixIndexList().resize(GetOriginVertexList().size());

                int iSize = (int)GetOriginVertexList().size();
                for ( int j = 0 ; j < iSize ; ++j )
                {
                    GetMatrixWeightList()[j].x = GetOriginVertexList()[j].Weights[0].Weight;
                    GetMatrixWeightList()[j].y = GetOriginVertexList()[j].Weights[1].Weight;
                    GetMatrixWeightList()[j].z = GetOriginVertexList()[j].Weights[2].Weight;
                    GetMatrixWeightList()[j].w = GetOriginVertexList()[j].Weights[3].Weight;

                    GetMatrixIndexList()[j].x = GetOriginVertexList()[j].Weights[0].RefNodeIndex;
                    GetMatrixIndexList()[j].y = GetOriginVertexList()[j].Weights[1].RefNodeIndex;
                    GetMatrixIndexList()[j].z = GetOriginVertexList()[j].Weights[2].RefNodeIndex;
                    GetMatrixIndexList()[j].w = GetOriginVertexList()[j].Weights[3].RefNodeIndex;

                    GetPosList()[j] = GetOriginVertexList()[j].Pos;
                }            
            }
            // ���ؽ� ���� ������ �����Ѵ�.
            InitVertexFormat();
            // ����̽����� ���ؽ����۵��� �����Ѵ�.
            InitVertexBuffers();
        }
        break;
#ifdef DEBUG_MODE
    case emvtBONE:
        {
            if ( GetPosList().size() != GetOriginVertexList().size() )  // ������ �� ������ ������ �ع���.
            {
                GetPosList().resize(GetOriginVertexList().size());
                GetMatrixWeightList().resize(GetOriginVertexList().size());
                GetMatrixIndexList().resize(GetOriginVertexList().size());

                int iSize = (int)GetOriginVertexList().size();
                for ( int j = 0 ; j < iSize ; ++j )
                {
                    GetMatrixWeightList()[j].x = GetOriginVertexList()[j].Weights[0].Weight;
                    GetMatrixWeightList()[j].y = GetOriginVertexList()[j].Weights[1].Weight;
                    GetMatrixWeightList()[j].z = GetOriginVertexList()[j].Weights[2].Weight;
                    GetMatrixWeightList()[j].w = GetOriginVertexList()[j].Weights[3].Weight;

                    GetMatrixIndexList()[j].x = GetOriginVertexList()[j].Weights[0].RefNodeIndex;
                    GetMatrixIndexList()[j].y = GetOriginVertexList()[j].Weights[1].RefNodeIndex;
                    GetMatrixIndexList()[j].z = GetOriginVertexList()[j].Weights[2].RefNodeIndex;
                    GetMatrixIndexList()[j].w = GetOriginVertexList()[j].Weights[3].RefNodeIndex;

                    GetPosList()[j] = GetOriginVertexList()[j].Pos;
                }
            }

            // ���ؽ� ���� ������ �����Ѵ�.
            InitVertexFormat();
            // ����̽����� ���ؽ����۵��� �����Ѵ�.
            InitVertexBuffers();
        }
        break;
#endif DEBUG_MODE
    }

    BuildOBB( PosList, vecCenter, vecAxis, AxisLen, vecOBB );

    // Min, Max ���.
    if (!PosList.empty())
    {
        vecMin = PosList[0];
        vecMax = PosList[0];
    }

    D3DXVECTOR3S::iterator it;
    for ( it = PosList.begin() ; it != PosList.end() ; ++it )
    {
        vecMax.x = max( (*it).x, vecMax.x );
        vecMax.y = max( (*it).y, vecMax.y );
        vecMax.z = max( (*it).z, vecMax.z );

        vecMin.x = min( (*it).x, vecMin.x );
        vecMin.y = min( (*it).y, vecMin.y );
        vecMin.z = min( (*it).z, vecMin.z );
    }
    vecCenter = (vecMax + vecMin) / 2.0f;
#endif
}

void TKMeshVertexes::Release(void)
{
    VertexBuffers.Release();
    
    __super::Release();
}

const TKMeshVertexes & TKMeshVertexes::operator = (const TKMeshVertexes & MeshVertexes)
{
    // Init �� �Ŀ��� �� ���簡 �̷�������� �ȵȴ�.
    TKVertexResource::operator = ( (TKVertexResource&)MeshVertexes );

    this->BoneNodeName          = MeshVertexes.BoneNodeName;
    this->AnimationMatrixIndex  = MeshVertexes.AnimationMatrixIndex;

    this->OriginVertexList      = MeshVertexes.OriginVertexList;
    this->PosList               = MeshVertexes.PosList;
    this->MatrixWeightList      = MeshVertexes.MatrixWeightList;
    this->MatrixIndexList       = MeshVertexes.MatrixIndexList;
    this->TexPosList            = MeshVertexes.TexPosList;
    this->ColorList             = MeshVertexes.ColorList;
    this->NormalList            = MeshVertexes.NormalList;
    this->AniMatrixList         = MeshVertexes.AniMatrixList;

	this->vecCenter				= MeshVertexes.vecCenter;
	memcpy(	this->vecAxis, MeshVertexes.vecAxis, sizeof(this->vecAxis) );
	memcpy(	this->AxisLen, MeshVertexes.vecAxis, sizeof(this->AxisLen) );
	memcpy(	this->vecOBB,  MeshVertexes.vecAxis, sizeof(this->vecOBB) );
	this->vecMax				= MeshVertexes.vecMax;

    // this->VertexBuffers // �������̹Ƿ� �� ���� ����.
    this->MeshVertexesType      = MeshVertexes.MeshVertexesType;

    return MeshVertexes;
}

void TKMeshVertexes::Copy_BaseInfo( TKMeshVertexes & Dest, TKMeshVertexes & Src )
{
    Dest.BoneNodeName          = Src.BoneNodeName;
    Dest.AnimationMatrixIndex  = Src.AnimationMatrixIndex;
}

////////////////////////////////////////////////////////////////////////////////
// TKMeshFaces
////////////////////////////////////////////////////////////////////////////////
TKMeshFaces::TKMeshFaces(int ID)
:TKIndexResource(ID)
,pMeshVertexes(NULL)
{
    SetObjectType( eotMeshFaces );
}

TKMeshFaces::~TKMeshFaces(void)
{
}

bool TKMeshFaces::InitIndexBuffers(void)
{
    ST_IB_TRIANGLE Index;
    int BufferSize;

    BufferSize = sizeof(ST_IB_TRIANGLE) * (int)FaceList.size();
    if (BufferSize > 0)
    {
#ifdef TK_D3D9
		// Mesh
        /////////////////////////////////////////////////////
        if (FAILED( R_D3DDEVICE->CreateIndexBuffer( BufferSize,
            D3DUSAGE_WRITEONLY,
            D3DFMT_INDEX16,
            D3DPOOL_DEFAULT,
            &pIB,
            NULL)))
        {
            return false;
        }
        ST_IB_TRIANGLE *pIndex;
        pIB->Lock(0, BufferSize, (void**)&pIndex, 0);
        memcpy(pIndex, &(FaceList[0]), BufferSize);
        pIB->Unlock();
#endif
    }

    return true;
}

bool TKMeshFaces::InitNormalSmoothing(void)
{
    if ( !pMeshVertexes ) return true;

#if 0

    //Smoothing
    //�ΰ��̻��� FACE����,
    //���ؽ��� ��ġ(X,Y,Z) �� ������, ���ؽ��� �ִ� ��� �ش� ���ؽ����� �븻���� ������������μ�
    //(�Ϲ������� �ش� ������ �븻������ ���� ����ȭ���ִ� ó���� �Ѵ�.)
    //������ ������ �� �ε巯�� ó���� �����ϰ� �ϴ� ���� ���Ѵ�.
    //������...������ �� �ƽ����� �̻ڰ� ó���� �븻���� �����Ƿ� �״� �ǹ̴�..-.-a
    D3DXVECTOR3S FaceNormalList;
    vector<ST_GEOM_VERTEX> & OriginVertexList = pMeshVertexes->GetOriginVertexList();
    D3DXVECTOR3S & NormalList = pMeshVertexes->GetNormalList();
    D3DXVECTOR3 vecDir[2];

    // ���̽� �븻�� ���Ѵ�
    FaceNormalList.resize( FaceList.size() );
    for ( int i = 0 ; i < (int)FaceList.size() ; ++i )
    {
        vecDir[0] = OriginVertexList[ FaceList[i]._1 ].Pos - OriginVertexList[ FaceList[i]._0 ].Pos;
        vecDir[1] = OriginVertexList[ FaceList[i]._2 ].Pos - OriginVertexList[ FaceList[i]._0 ].Pos;
        D3DXVec3Cross( &FaceNormalList[i], &vecDir[0], &vecDir[1] );
        D3DXVec3Normalize( &FaceNormalList[i], &FaceNormalList[i] );
    }

    // ������ ���̽� �븻���� ���ؽ� �븻�� ���Ѵ�.
    NormalList.resize( OriginVertexList.size() );
    if ( NormalList.size () > 0 )
    {
        const size_t iSize = sizeof(D3DXVECTOR3) * OriginVertexList.size();
        memset( &NormalList[0], 0, iSize );
        for ( int i = 0 ; i < (int)FaceList.size() ; ++i )
        {
            NormalList[ FaceList[i]._0 ] += FaceNormalList[i];
            NormalList[ FaceList[i]._1 ] += FaceNormalList[i];
            NormalList[ FaceList[i]._2 ] += FaceNormalList[i];
        }
        for ( int i = 0 ; i < (int)NormalList.size() ; ++i )
        {
            D3DXVec3Normalize( &NormalList[i], &NormalList[i] );
        }
    }

    // ������ ���ؽ� �븻�� �������� �Ͽ� ���ؽ� ���� ���� ��� ������ ���ؽ��� �����ϰ�.
    // �븻���� ����� ���Ѵ�.
    map<D3DXVECTOR3, vector<D3DXVECTOR3*>*> NormalizeList;
    map<D3DXVECTOR3, vector<D3DXVECTOR3*>*>::iterator it;
    vector<D3DXVECTOR3*> * pSameNormalPointers;
    for ( int i = 0 ; i < (int)OriginVertexList.size()-1 ; ++i )
    {
        //it = NormalizeList.find( OriginVertexList[i].Pos );
        it = NormalizeList.end();
        for ( it = NormalizeList.begin() ; it != NormalizeList.end() ; ++it )
        {
            if ( it->first == OriginVertexList[i].Pos )
            break;
        }

        if ( it == NormalizeList.end() )
        {
            pSameNormalPointers = new vector<D3DXVECTOR3*>;
            pSameNormalPointers->push_back( &NormalList[i] );

            NormalizeList.insert( make_pair(OriginVertexList[i].Pos, pSameNormalPointers) );
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

#endif
    return true;
}

void TKMeshFaces::Init(void)
{
    __super::Init();

    if ( !pMeshVertexes )
    {
        MessageBox(0, "MeshVertexes is not defined", "TKMeshFaces::Init", MB_ICONERROR);
        _ASSERT(false);
    }

    switch ( pMeshVertexes->GetMeshVertexesType() )
    {
    case emvtMESH:
        {
            // ���ĵ� FaceList�� ������� �ε������۸� �����Ѵ�.
            InitIndexBuffers();
        }
        break;
#ifdef DEBUG_MODE
    case emvtBONE:
        {
            // ���ĵ� FaceList�� ������� �ε������۸� �����Ѵ�.
            InitIndexBuffers();
        }
        break;
#endif DEBUG_MODE
    }
}

void TKMeshFaces::Release(void)
{
    __super::Release();
}

const TKMeshFaces & TKMeshFaces::operator = (const TKMeshFaces & MeshFaces)
{
    TKIndexResource::operator=((TKIndexResource)MeshFaces);
    this->FaceList = MeshFaces.FaceList;

    return MeshFaces;
}

void TKMeshFaces::Copy_BaseInfo( TKMeshFaces & Dest, TKMeshFaces & Src )
{
}

////////////////////////////////////////////////////////////////////////////////
// TKMeshGeometry
////////////////////////////////////////////////////////////////////////////////
TKMeshGeometry::TKMeshGeometry(string NodeName)
:TKEngineResource(-1)
,pMeshVertexes(NULL)
,pMeshFaces(NULL)
,MaterialName("")
,SubMaterialName("")
,pMaterial(NULL)
,pSubMaterial(NULL)
,pShaderEffect(NULL)
{
	SetObjectType( eotMeshGeometry );
#if 0
    this->SetNodeName( NodeName );
#endif
}

TKMeshGeometry::~TKMeshGeometry(void)
{
}

void TKMeshGeometry::Init(void)
{
    __super::Init();

    if ( !pMeshVertexes )
    {
        MessageBox(0, "MeshVertexes is not defined", "TKMeshGeometry::Init", MB_ICONERROR);
        _ASSERT(false);
    }

    if ( !pMeshFaces )
    {
        MessageBox(0, "MeshFaces is not defined", "TKMeshGeometry::Init", MB_ICONERROR);
        _ASSERT(false);
    }
}

void TKMeshGeometry::Release(void)
{
    pMeshVertexes = NULL;
    pMeshFaces = NULL;

    __super::Release();
}

const TKMeshGeometry & TKMeshGeometry::operator = (const TKMeshGeometry & MeshGeometry)
{
    // Init �� �Ŀ��� �� ���簡 �̷�������� �ȵȴ�.
    TKEngineResource::operator = ( (TKVertexResource&)MeshGeometry );

    pMeshVertexes               = MeshGeometry.pMeshVertexes;
    pMeshFaces                = MeshGeometry.pMeshFaces;

    this->MaterialName          = MeshGeometry.MaterialName;
    this->SubMaterialName       = MeshGeometry.SubMaterialName;
    // this->pMaterial // �������̹Ƿ� �� ���� ����.
    // this->pSubMaterial // �������̹Ƿ� �� ���� ����.
    this->pShaderEffect         = MeshGeometry.pShaderEffect;

    return MeshGeometry;
}

void TKMeshGeometry::Copy_BaseInfo( TKMeshGeometry & Dest, TKMeshGeometry & Src )
{
    TKMeshVertexes::Copy_BaseInfo((*Dest.pMeshVertexes), (*Src.pMeshVertexes));
    TKMeshFaces::Copy_BaseInfo((*Dest.pMeshFaces), (*Src.pMeshFaces));

    Dest.MaterialName          = Src.MaterialName;
    Dest.SubMaterialName       = Src.SubMaterialName;
    Dest.pShaderEffect         = Src.pShaderEffect;
}

bool TKMeshGeometry::CreateOBBGeomList( GEOMLIST & GeomList, D3DXVECTOR3 & vecCenter, D3DXVECTOR3 vecAxis[3], float AxisLen[3], D3DXVECTOR3 vecOBB[8])
{
#if 0
	GEOMLIST::iterator it;
    D3DXVECTOR3S PosList;

    int nPoints = 0;
    int nOffSet = 0;

    // ������Ʈ������ �� ���ؽ� ������ �ջ�.
    for ( it = GeomList.begin() ; it != GeomList.end() ; ++it )
    {
        nPoints += (int)(*it)->GetPosList().size();
        if (!(*it)->GetPosList().empty())
        {
            PosList.resize(nPoints);
            memcpy(&PosList[nOffSet], &(*it)->GetPosList()[0], sizeof(D3DXVECTOR3) * nPoints);
        }
        nOffSet += (int)(*it)->GetPosList().size();
    }

    return BuildOBB( PosList, vecCenter, vecAxis, AxisLen, vecOBB );
#else
	return false;
#endif
}

////////////////////////////////////////////////////////////////////////////////
// TKMeshRes
////////////////////////////////////////////////////////////////////////////////
TKMeshRes::TKMeshRes(int ID)
:TKEngineResource(ID)
,pShaderEffect(NULL)
{
    SetObjectType( eotMeshRes );
    // �⺻������ �����ܰ��� LOD�� ������ �ִ´�.
    // �� ��ü�� �ı��Ǳ� ������ �ش� ����Ʈ�� �������� �ʴ´�.
    NewLODDistance( LOD_INFINITY_DISTANCE );
}

TKMeshRes::~TKMeshRes(void)
{
    ClearDelete();
}

bool TKMeshRes::CompareGEOMLIST_ALPHA(TKMeshGeometry *pA, TKMeshGeometry *pB)
{
    if ( !pA->GetMaterial() )
    {
        return false;
    }
    if ( !pB->GetMaterial() )
    {
        return true;
    }
    return ( !pA->GetMaterial()->GetAlphaBlend() && pB->GetMaterial()->GetAlphaBlend() );
}

void TKMeshRes::MaterialBind(GEOMLIST & ParamList)
{
    MATERIALLIST::iterator it;
    MATERIALLIST::iterator itSub;
    for (int i = 0 ; i < (int)ParamList.size() ; ++i )
    {
        it = MaterialList.find( ParamList[i]->GetMaterialName() );
        if ( it != MaterialList.end() )
        {
            ParamList[i]->SetMaterial( it->second );
            itSub = it->second->GetMaterialList().find( ParamList[i]->GetSubMaterialName() );
            if ( itSub != it->second->GetMaterialList().end() )
            {
                ParamList[i]->SetSubMaterial( itSub->second );
            }
            else
            {
                ParamList[i]->SetSubMaterial( NULL );
            }
        }
        else
        {
            ParamList[i]->SetMaterial( NULL );
            ParamList[i]->SetSubMaterial( NULL );
        }
    }
}

void TKMeshRes::Init(void)
{
    __super::Init();

    // ���͸���� �޽��� ���ε�
    MaterialBind(BoneList);
    MaterialBind(GeomList);

    // ���͸��� ����Ʈ �ʱ�ȭ
    MATERIALLIST::iterator it;
    for ( it = MaterialList.begin() ; it != MaterialList.end() ; ++it )
    {
        it->second->Init();
    }

    // ���ؽ��� ����Ʈ �ʱ�ȭ
    for ( int i = 0 ; i < (int)VertexesList.size() ; ++i )
    {
        VertexesList[i]->Init();
    }

    // �ε����� ����Ʈ �ʱ�ȭ
    for ( int i = 0 ; i < (int)FacesList.size() ; ++i )
    {
        FacesList[i]->Init();
    }

    // �� ����Ʈ �ʱ�ȭ
    for ( int i = 0 ; i < (int)BoneList.size() ; ++i )
    {
        BoneList[i]->Init();
    }

    // �޽� ���� �ʱ�ȭ
    for ( int i = 0 ; i < (int)GeomList.size() ; ++i )
    {
        GeomList[i]->Init();
    }
}

void TKMeshRes::Release(void)
{
    MATERIALLIST::iterator it;

    for ( int i = 0 ; i < (int)GeomList.size() ; ++i )
    {
        GeomList[i]->Release();
    }

    for ( int i = 0 ; i < (int)BoneList.size() ; ++i )
    {
        BoneList[i]->Release();
    }

    for ( int i = 0 ; i < (int)FacesList.size() ; ++i )
    {
        FacesList[i]->Release();
    }

    for ( int i = 0 ; i < (int)VertexesList.size() ; ++i )
    {
        VertexesList[i]->Release();
    }

    for ( it = MaterialList.begin() ; it != MaterialList.end() ; ++it )
    {
        it->second->Release();
    }

    __super::Release();
}

void TKMeshRes::ClearDelete(void)
{
    MATERIALLIST::iterator it;
    LODLIST::iterator   itLOD;

    for ( it = MaterialList.begin() ; it != MaterialList.end() ; ++it ) delete it->second;
    MaterialList.clear();
    for ( int i = 0 ; i < (int)VertexesList.size() ; ++i ) delete VertexesList[i];
    VertexesList.clear();
    for ( int i = 0 ; i < (int)FacesList.size() ; ++i ) delete FacesList[i];
    FacesList.clear();
    for ( int i = 0 ; i < (int)BoneList.size() ; ++i ) delete BoneList[i];
    BoneList.clear();
    for ( int i = 0 ; i < (int)GeomList.size() ; ++i ) delete GeomList[i];
    GeomList.clear();
    for ( itLOD = LodList.begin() ; itLOD != LodList.end() ; ++itLOD ) delete itLOD->second;
    LodList.clear();
}

void TKMeshRes::Copy_BaseInfo( TKMeshRes & DestMeshRes, TKMeshRes & SrcMeshRes )
{
    // ��������/�޽������͸� ������ ������ �����͸� �����Ѵ�.
    MATERIALLIST::iterator it;
    TKMeshMaterial *pMeshMaterial;
    for ( it = DestMeshRes.GetMaterialList().begin() ; it != DestMeshRes.GetMaterialList().end() ; ++it )
    {
        it->second->Release();
        delete it->second;
    }
    DestMeshRes.GetMaterialList().clear();
    for ( it = SrcMeshRes.GetMaterialList().begin() ; it != SrcMeshRes.GetMaterialList().end() ; ++it )
    {
        pMeshMaterial = DestMeshRes.NewMaterial( it->second->GetNodeName() );
        (*pMeshMaterial) = (*it->second);
    }

    // DestMeshRes.m_pMatrixArray; <- �������. ��? ��ü�ϱ�.
    DestMeshRes.pShaderEffect = SrcMeshRes.pShaderEffect;
}

void TKMeshRes::SetShaderEffect(TKMeshShaderEffect *pShaderEffect)
{
#if 0
	this->pShaderEffect = pShaderEffect;
    for ( int i = 0 ; i < (int)GeomList.size() ; ++i )
    {
        GeomList[i]->SetShaderEffect(pShaderEffect);
    };

    TKMeshShaderEffect *pBoneShaderEffect;
    pBoneShaderEffect = GetEngine()->ShaderLoad<TKMeshShaderEffect>("BoneMesh.fx", "TShader");
    for ( int i = 0 ; i < (int)BoneList.size() ; ++i )
    {
        BoneList[i]->SetShaderEffect( pBoneShaderEffect );
    };
#endif
}

bool TKMeshRes::NewLODDistance(int Distance)
{
    LODLIST::iterator   itLOD;

    itLOD = LodList.find( Distance );
    if ( itLOD == LodList.end() )
    {
        GEOMLIST *pGeomList = new GEOMLIST;
        LodList.insert( itLOD, make_pair(Distance, pGeomList) );
        return true;
    }
    return false;
}

bool TKMeshRes::DeleteLODDistance(int Distance)
{
    LODLIST::iterator   itLOD;

    itLOD = LodList.find( Distance );
    if ( itLOD != LodList.end() )
    {
        delete itLOD->second;
        LodList.erase( itLOD );
        return true;
    }
    return false;
}

bool TKMeshRes::ClearLODGeometry(int Distance)
{
    LODLIST::iterator   itLOD;

    itLOD = LodList.find( Distance );
    if ( itLOD != LodList.end() )
    {
        itLOD->second->clear();
        return true;
    }
    return false;
}

bool TKMeshRes::AddLODGeometry(int Distance, TKMeshGeometry *pGeometry)
{
    LODLIST::iterator   itLOD;

    itLOD = LodList.find( Distance );
    if ( itLOD != LodList.end() )
    {
        itLOD->second->push_back( pGeometry );
        return true;
    }
    return false;
}

bool TKMeshRes::ClearAllLODGeometroy(void)
{
    LODLIST::iterator   itLOD;

    for ( itLOD = LodList.begin() ; itLOD != LodList.begin() ; )
    {
        if ( itLOD->first == LOD_INFINITY_DISTANCE )
        {
            itLOD->second->clear();
            ++itLOD;
            continue;
        }
        else
        {
            itLOD->second->clear();
            delete itLOD->second;
            itLOD = LodList.erase( itLOD );
            continue;
        }
    }

    return true;
}

bool TKMeshRes::AddAllGeometryToInfinityLevel(void)
{
    GEOMLIST::iterator  itGEOM;
    LODLIST::iterator   itLOD;

    itLOD = LodList.find( LOD_INFINITY_DISTANCE );
    if ( itLOD != LodList.end() )
    {
        itLOD->second->clear();
        for ( itGEOM = GeomList.begin() ; itGEOM != GeomList.end() ; ++itGEOM )
        {
            itLOD->second->push_back ( (*itGEOM) );
        };
        return true;
    }
    else
    {
        MessageBox(0, "LOD_INFINITY_DISTANCE not exists", "TKMeshRes::AddAllGeometryToInfinityLevel", MB_ICONERROR );
        _ASSERT(0);
        return false;
    }
}

TKMeshMaterial * TKMeshRes::NewMaterial(string MaterialName)
{
	TKMeshMaterial *pMaterial = NULL;
#if 0
	MATERIALLIST::iterator it;

    if ( MaterialName.empty() ) return NULL;
    it = MaterialList.find( MaterialName );
    if ( it != MaterialList.end() ) return NULL;

    pMaterial = new TKMeshMaterial(GetEngine(), MaterialName);
    MaterialList.insert( make_pair( pMaterial->GetNodeName(), pMaterial ) );
#endif
    return pMaterial;
}

void TKMeshRes::DeleteMaterial(TKMeshMaterial * pMaterial)
{
    MATERIALLIST::iterator it;
    GEOMLIST::iterator itGeom;

    // TODO : �ش� ���͸����� ����ϴ� ������Ʈ���� ���͸��� �����͸� NULL �� �����Ѵ�.
    if ( pMaterial )
    {
        for ( it = MaterialList.begin() ; it != MaterialList.end() ; ++it )
        {
            if ( it->second == pMaterial )
            {
                MaterialList.erase(it);
                break;
            }
        }

        for ( itGeom = GeomList.begin() ; itGeom != GeomList.end() ; ++itGeom )
        {
            if ( (*itGeom)->GetMaterial() == pMaterial )
            {
                (*itGeom)->SetMaterial(NULL);
                (*itGeom)->GetMaterialName().clear();
                (*itGeom)->SetSubMaterial(NULL);
                (*itGeom)->GetSubMaterialName().clear();
            }
        }

        pMaterial->Release();
        delete pMaterial;
    }
}

void TKMeshRes::DeleteMaterial(string MaterialName)
{
    // TODO : �� ��ü�� ����ϴ� ������Ʈ���� ��ü �����͸� NULL �� �����Ѵ�

    TKMeshMaterial *pMaterial;
    MATERIALLIST::iterator it;
    GEOMLIST::iterator itGeom;

    it = MaterialList.find( MaterialName );
    if ( it != MaterialList.end() )
    {
        pMaterial = it->second;
        MaterialList.erase( it );

        for ( itGeom = GeomList.begin() ; itGeom != GeomList.end() ; ++itGeom )
        {
            if ( (*itGeom)->GetMaterial() == pMaterial )
            {
                (*itGeom)->SetMaterial(NULL);
                (*itGeom)->GetMaterialName().clear();
                (*itGeom)->SetSubMaterial(NULL);
                (*itGeom)->GetSubMaterialName().clear();
            }
        }

        pMaterial->Release();
        delete pMaterial;
    }
}

void TKMeshRes::DeleteNoReferenceMaterial(void)
{
    MATERIALLIST UsingMaterialList;
    MATERIALLIST::iterator itMat, itMatUsing;
    GEOMLIST::iterator itGeom;
    TKMeshMaterial *pMaterial;

    for ( itGeom = GeomList.begin() ; itGeom != GeomList.end() ; ++itGeom )
    {
        if ( (*itGeom)->GetMaterial() )
        {
            UsingMaterialList.insert( make_pair( (*itGeom)->GetMaterial()->GetNodeName(), (*itGeom)->GetMaterial() ) );
        }
    }

    for ( itMat = MaterialList.begin() ; itMat != MaterialList.end() ; )
    {
        itMatUsing = UsingMaterialList.find( itMat->first );
        if ( itMatUsing == UsingMaterialList.end() )
        {
            pMaterial = itMat->second;
            itMat = MaterialList.erase( itMat );
            pMaterial->Release();
            delete pMaterial;
        }
        else
        {
            ++itMat;
        }
    }
}

TKMeshVertexes * TKMeshRes::NewVertexes(string NodeName, EMeshVertexesType MeshVertexesType)
{
#if 0
	if ( !NodeName.empty() )
    {
        VERTEXESLIST::iterator it;
        for ( it = VertexesList.begin() ; it != VertexesList.end() ; ++it )
        {
            if ( (*it)->GetNodeName() == NodeName )
            {
                return (*it);
            }
        }
    }
    TKMeshVertexes * pMeshVertexes = new TKMeshVertexes(GetEngine(), -1);
    VertexesList.push_back( pMeshVertexes );
    pMeshVertexes->SetNodeName( NodeName );
    pMeshVertexes->SetMeshVertexesType( MeshVertexesType );

    return pMeshVertexes;
#else
	return nullptr;
#endif
}

void TKMeshRes::DeleteVertexes(TKMeshVertexes * pMeshVertexes)
{
    // TODO : �� ��ü�� ����ϴ� ������Ʈ���� ��ü �����͸� NULL �� �ʱ�ȭ �ؾ� �Ѵ�.

    VERTEXESLIST::iterator it;
    for ( it = VertexesList.begin() ; it != VertexesList.end() ; ++it )
    {
        if ( (*it) == pMeshVertexes )
        {
            VertexesList.erase(it);
            delete pMeshVertexes;
            return;
        }
    }
}

void TKMeshRes::DeleteNoReferenceBoneVertexes(void)
{
    set<TKMeshVertexes *> UsingList;
    set<TKMeshVertexes *>::iterator itUL;
    GEOMLIST::iterator itGeom;
    VERTEXESLIST::iterator itVL;

    for ( itGeom = BoneList.begin() ; itGeom != BoneList.end() ; ++itGeom )
    {
        UsingList.insert( (*itGeom)->GetMeshVertexes() );
    }
    for ( itGeom = GeomList.begin() ; itGeom != GeomList.end() ; ++itGeom )
    {
        UsingList.insert( (*itGeom)->GetMeshVertexes() );
    }

    itVL = VertexesList.begin();
    while ( itVL != VertexesList.end() )
    {
        itUL = UsingList.find( (*itVL) );
        if ( itUL == UsingList.end() )
        {
            itVL = VertexesList.erase(itVL);
            continue;
        }
        ++itVL;
    }
}

TKMeshVertexes * TKMeshRes::GetVertexesByNodeName(const string NodeName)
{
    VERTEXESLIST::iterator itVL;

    for ( itVL = VertexesList.begin() ; itVL != VertexesList.end() ; ++itVL )
    {
        if ( (*itVL)->GetNodeName() == NodeName )
        {
            return (*itVL);
        }
    }

    return NULL;
}

TKMeshFaces * TKMeshRes::NewFaces(string NodeName, TKMeshVertexes * pVertexes)
{
#if 0
	if ( !NodeName.empty() )
    {
        FACESLIST::iterator it;
        for ( it = FacesList.begin() ; it != FacesList.end() ; ++it )
        {
            if ( (*it)->GetNodeName() == NodeName )
            {
                return (*it);
            }
        }
    }
    TKMeshFaces * pMeshFaces = new TKMeshFaces(GetEngine(), -1);
    FacesList.push_back( pMeshFaces );
    pMeshFaces->SetNodeName( NodeName );
    pMeshFaces->SetMeshVertexes( pVertexes );

    return pMeshFaces;
#else
	return nullptr;
#endif
}

void TKMeshRes::DeleteFaces(TKMeshFaces * pMeshFaces)
{
    // TODO : �� ��ü�� ����ϴ� ������Ʈ���� ��ü �����͸� NULL �� �ʱ�ȭ �ؾ� �Ѵ�.

    FACESLIST::iterator it;
    for ( it = FacesList.begin() ; it != FacesList.end() ; ++it )
    {
        if ( (*it) == pMeshFaces )
        {
            FacesList.erase(it);
            delete pMeshFaces;
            return;
        }
    }
}

void TKMeshRes::DeleteNoRefFaces(void)
{
    set<TKMeshFaces *> UsingList;
    set<TKMeshFaces *>::iterator itUL;
    GEOMLIST::iterator itGeom;
    FACESLIST::iterator itIL;

    for ( itGeom = BoneList.begin() ; itGeom != BoneList.end() ; ++itGeom )
    {
        UsingList.insert( (*itGeom)->GetMeshFaces() );
    }
    for ( itGeom = GeomList.begin() ; itGeom != GeomList.end() ; ++itGeom )
    {
        UsingList.insert( (*itGeom)->GetMeshFaces() );
    }

    itIL = FacesList.begin();
    while ( itIL != FacesList.end() )
    {
        itUL = UsingList.find( (*itIL) );
        if ( itUL == UsingList.end() )
        {
            itIL = FacesList.erase(itIL);
            continue;
        }
        ++itIL;
    }
}

TKMeshFaces * TKMeshRes::GetFacesByNodeName(const string NodeName)
{
    FACESLIST::iterator itIL;

    for ( itIL = FacesList .begin() ; itIL != FacesList.end() ; ++itIL )
    {
        if ( (*itIL)->GetNodeName() == NodeName )
        {
            return (*itIL);
        }
    }

    return NULL;
}

TKMeshGeometry * TKMeshRes::NewGeometry(string NodeName, TKMeshVertexes * pVertexes, TKMeshFaces * pFaces)
{
    TKMeshGeometry * pMeshGeometry = NULL;
#if 0
	GEOMLIST::iterator it;
    GEOMLIST *pGeomList;

    if ( pVertexes->GetMeshVertexesType() == emvtMESH )
        pGeomList = &GeomList;
    else if ( pVertexes->GetMeshVertexesType() == emvtBONE )
        pGeomList = &BoneList;
    else
        return NULL;

    if ( !NodeName.empty() )
    {
        for ( it = pGeomList->begin() ; it != pGeomList->end() ; ++it )
        {
            if ( (*it)->GetNodeName() == NodeName )
            {
                return (*it);
            }
        }
    }

    pMeshGeometry = new TKMeshGeometry( GetEngine(), NodeName );
    pGeomList->push_back( pMeshGeometry );
    pMeshGeometry->SetMeshVertexes( pVertexes );
    pMeshGeometry->SetMeshFaces( pFaces );
#endif
    return pMeshGeometry;
}

void TKMeshRes::DeleteGeometry(TKMeshGeometry * pGeometry)
{
    GEOMLIST::iterator it;
    if ( pGeometry )
    {
        it = GeomList.begin();
        while ( it != GeomList.end() )
        {
            if ( (*it) == pGeometry )
            {
                it = GeomList.erase(it);
                continue;
            }
            ++it;
        }

        it = BoneList.begin();
        while ( it != BoneList.end() )
        {
            if ( (*it) == pGeometry )
            {
                it = BoneList.erase(it);
                continue;
            }
            ++it;
        }

        delete pGeometry;
    }
}

void TKMeshRes::DeleteNoReferenceBone(void)
{
    map<string, TKMeshGeometry *>::iterator itList;
    map<string, TKMeshGeometry *> UsingGeomList;

    TKMeshGeometry *pGeom;

    VERTEXESLIST::iterator itVL;
    vector<ST_GEOM_VERTEX>::iterator itOrigin;

    for ( itVL = VertexesList.begin() ; itVL != VertexesList.end() ; ++itVL )
    {
        for ( itOrigin = (*itVL)->GetOriginVertexList().begin() ;
              itOrigin != (*itVL)->GetOriginVertexList().end() ;
              ++itOrigin )
        {
            for ( int i = 0 ; i < 4 ; ++i )
            {
                // �ش� ���ؽ����� ����ϴ� ���� ã�´�.
                for ( int j = 0 ; j < (int)BoneList.size() ; ++j )
                {
                    if ( BoneList[j]->GetMeshVertexes() == itOrigin->Weights[i].pRefNode )
                    {
                        UsingGeomList.insert( make_pair(BoneList[j]->GetNodeName(), BoneList[j] ) );
                    }
                }
            }
        }
    }

    this->Release();

    GEOMLIST::iterator itMesh = BoneList.begin();
    while ( itMesh != BoneList.end() )
    {
        itList = UsingGeomList.find( (*itMesh)->GetNodeName() );
        if ( itList == UsingGeomList.end() )
        {
            // ����
            pGeom = (*itMesh);
            itMesh = BoneList.erase( itMesh );
            delete pGeom;
        }
        else
        {
            // �������� �Ѿ
            ++itMesh;
        }
    }

    this->Init();
}

void TKMeshRes::SortGeomListByAlpha(void)
{
    map<string, TKMeshGeometry*> TempGeomList;
    map<string, TKMeshGeometry*>::iterator it;

    for ( int i = 0 ; i < (int)GeomList.size() ; ++i )
    {
        TempGeomList.insert( make_pair( GeomList[i]->GetNodeName(), GeomList[i] ) );
    }
    GeomList.clear();
    for ( it = TempGeomList.begin() ; it != TempGeomList.end() ; ++it )
    {
        GeomList.push_back( it->second );
    }
    sort( GeomList.begin(), GeomList.end(), CompareGEOMLIST_ALPHA );
}

void TKMeshRes::MoveGeomList(const int Index, TKMeshGeometry *pMeshGeom)
{
    GEOMLIST::iterator it;

    for ( it = GeomList.begin() ; it != GeomList.end() ; ++it )
    {
        if ( (*it) == pMeshGeom )
        {
            GeomList.erase(it);
            break;
        }
    }

    it = GeomList.begin();
    it += Index;
    GeomList.insert( it, pMeshGeom );
}

TKMeshGeometry * TKMeshRes::GetGeomByNodeName(const string NodeName)
{
    TKMeshGeometry *pResult = NULL;

    if ( !NodeName.empty() )
    {
        for ( int i = 0 ; i < (int)BoneList.size() ; ++i )
        {
            if ( BoneList[i]->GetNodeName() == NodeName )
            {
                pResult = BoneList[i];
                break;
            }
        }
        if (pResult == NULL)
        {
            for ( int i = 0 ; i < (int)GeomList.size() ; ++i )
            {
                if ( GeomList[i]->GetNodeName() == NodeName )
                {
                    pResult = GeomList[i];
                }
            }
        }
    }
    return pResult;
}

D3DXVECTOR3 TKMeshRes::GetMin(void)
{
    D3DXVECTOR3 Result;
    GEOMLIST::iterator itGeom;
    Result.x = FLT_MAX;
    Result.y = FLT_MAX;
    Result.z = FLT_MAX;

    for ( itGeom = GeomList.begin() ; itGeom != GeomList.end() ; ++itGeom )
    {
        Result.x = min( (*itGeom)->GetMin().x, Result.x );
        Result.y = min( (*itGeom)->GetMin().y, Result.y );
        Result.z = min( (*itGeom)->GetMin().z, Result.z );
    }

    return Result;
}

D3DXVECTOR3 TKMeshRes::GetMax(void)
{
    D3DXVECTOR3 Result;
    GEOMLIST::iterator itGeom;
    Result.x = FLT_MAX * -1;
    Result.y = FLT_MAX * -1;
    Result.z = FLT_MAX * -1;

    for ( itGeom = GeomList.begin() ; itGeom != GeomList.end() ; ++itGeom )
    {
        Result.x = max( (*itGeom)->GetMax().x, Result.x );
        Result.y = max( (*itGeom)->GetMax().y, Result.y );
        Result.z = max( (*itGeom)->GetMax().z, Result.z );
    }

    return Result;
}

////////////////////////////////////////////////////////////////////////////////
// TKAniMeshRes
////////////////////////////////////////////////////////////////////////////////
TKAniMeshRes::TKAniMeshRes(int ID)
:BASECLASS(ID)
,m_FirstFrame(-1)
,m_LastFrame(-1)
,m_FrameSpeed(-1)
,m_MatrixArrayRowCount(0)
,m_MatrixArrayColCount(0)
,m_MatrixArrayTotalCount(0)
,m_pMatrixArray(NULL)
{
    SetObjectType( eotAniMeshRes );
    // �⺻������ �����ܰ��� LOD�� ������ �ִ´�.
    // �� ��ü�� �ı��Ǳ� ������ �ش� ����Ʈ�� �������� �ʴ´�.
    NewLODDistance( LOD_INFINITY_DISTANCE );
}

TKAniMeshRes::~TKAniMeshRes(void)
{
    ClearDelete();
}

void TKAniMeshRes::Build60FPSAnimation(GEOMLIST & ParamList)
{
    //D3DXMATRIX      matAni1 , matAni2;
    //D3DXVECTOR3     vecPos1 , vecPos2;
    //D3DXQUATERNION  quatRot1, quatRot2;
    //float           value;
    //int             NewFrame;

    //map<int, ST_ANIMATRIX> NewMatrixList;

    //if ( FrameSpeed == 30 )
    //{
    //    for ( int i = 0 ; i < ParamGeomList.Count() ; ++i )
    //    {
    //        if ( !ParamGeomList[i]->AniMatrixList.empty() )
    //        {
    //            NewMatrixList.clear();
    //            value = 0.0f;
    //            NewFrame = 0;

    //            for ( int j = 0 ; j < (int)ParamGeomList[i]->AniMatrixList.size()-1 ; ++j )
    //            {
    //                matAni1 = ParamGeomList[i]->AniMatrixList[j+0].matAni;
    //                vecPos1.x = matAni1._41;
    //                vecPos1.y = matAni1._42;
    //                vecPos1.z = matAni1._43;
    //                matAni1._41 = 0.0f;
    //                matAni1._42 = 0.0f;
    //                matAni1._43 = 0.0f;
    //                D3DXQuaternionRotationMatrix(&quatRot1, &matAni1);

    //                matAni2 = ParamGeomList[i]->AniMatrixList[j+1].matAni;
    //                vecPos2.x = matAni1._41;
    //                vecPos2.y = matAni1._42;
    //                vecPos2.z = matAni1._43;
    //                matAni2._41 = 0.0f;
    //                matAni2._42 = 0.0f;
    //                matAni2._43 = 0.0f;
    //                D3DXQuaternionRotationMatrix(&quatRot2, &matAni2);
    //            }
    //        }
    //    }
    //}
}

void TKAniMeshRes::SetAnimationMatrixIndex(void)
{
    // �ִϸ��̼� ��Ʈ���� �ε��� ���� �ʱ�ȭ
    m_MatrixArrayRowCount = 0;
    m_MatrixArrayColCount = 0;
    m_MatrixArrayTotalCount = 0;

    // ������ ���鼭 ���������� �ִϸ��̼� �ε����� �����ϰ�,
    // �ִϸ��̼� ��Ʈ������ ������ �� �迭�� ũ�⸦ �����Ѵ�.
    for ( int i = 0 ; i < (int)VertexesList.size() ; ++i )
    {
        if ( VertexesList[i]->GetAniMatrixList().empty() == false ) // �ִϸ��̼��� �ִ� ���
        {
            VertexesList[i]->SetAnimationMatrixIndex( m_MatrixArrayColCount ); // �ִϸ��̼� �ε����� �����ϰ�.
            ++m_MatrixArrayColCount;    // �ִϸ��̼� ī��Ʈ�� ����.

            if (m_MatrixArrayRowCount == 0)
            {
                m_MatrixArrayRowCount = (int)VertexesList[i]->GetAniMatrixList().size();
            }
            else
            {
                if (m_MatrixArrayRowCount != VertexesList[i]->GetAniMatrixList().size())
                {
                    MessageBox(0, "�ִϸ��̼� ��Ʈ���� ī��Ʈ ����", "TKAniMeshRes::SetAnimationMatrixIndex", MB_ICONERROR);
                }
            }
        }
        else
        {
            // �ִϸ��̼��� ������, �ִϸ��̼� �ε����� 0.
            // Bone�� Mesh�� ���ο� ������� �ִϸ��̼� �ε����� 0.
            VertexesList[i]->SetAnimationMatrixIndex( 0 );
        }
    }
}

void TKAniMeshRes::SetAnimationMatrixData(void)
{
    // �ִϸ��̼� �ε��� ������ �������, �ִϸ��̼� ��Ʈ���� �迭 ����
    m_MatrixArrayTotalCount = m_MatrixArrayRowCount * m_MatrixArrayColCount;
    if ( m_MatrixArrayTotalCount > 0 )
    {
        m_pMatrixArray = new D3DXMATRIX [m_MatrixArrayTotalCount];
        memset(m_pMatrixArray, 0, sizeof(D3DXMATRIX) * m_MatrixArrayTotalCount);

        // �ִϸ��̼� �ε��� ������ �������, �ִϸ��̼� ��Ʈ���� �迭 �����͸� ä����.
        for ( int i = 0 ; i < (int)VertexesList.size() ; ++i )
        {
            if ( !VertexesList[i]->GetAniMatrixList().empty() )
            {
                for ( int j = 0 ; j < m_MatrixArrayRowCount ; ++j )
                {
                    const int index = VertexesList[i]->GetAnimationMatrixIndex() + j * m_MatrixArrayColCount;
                    m_pMatrixArray[index] = VertexesList[i]->GetAniMatrixList()[j].matAni;
                }
            }
        }
    }
}

void TKAniMeshRes::MeshAnimationBind(void)
{
#if 0
	TKMeshVertexes *pVertexes = NULL;

    for ( int i = 0 ; i < (int)VertexesList.size() ; ++i )
    {
        if (VertexesList[i]->GetMeshVertexesType() == emvtMESH)
        {
            VertexesList[i]->GetPosList().resize(VertexesList[i]->GetOriginVertexList().size());
            VertexesList[i]->GetMatrixWeightList().resize(VertexesList[i]->GetOriginVertexList().size());
            VertexesList[i]->GetMatrixIndexList().resize(VertexesList[i]->GetOriginVertexList().size());

            int iSize = (int)VertexesList[i]->GetOriginVertexList().size();
            for ( int j = 0 ; j < iSize ; ++j )
            {
                for (int k = 0 ; k < 4 ; ++k )
                {
                    pVertexes = GetVertexesByNodeName( VertexesList[i]->GetOriginVertexList()[j].Weights[k].GetParentNodeName() );
                    if (pVertexes && pVertexes != VertexesList[i])
                    {
                        VertexesList[i]->GetOriginVertexList()[j].Weights[k].RefNodeIndex = pVertexes->GetAnimationMatrixIndex();
                        VertexesList[i]->GetOriginVertexList()[j].Weights[k].pRefNode = pVertexes;
                    }
                }

                VertexesList[i]->GetMatrixWeightList()[j].x = VertexesList[i]->GetOriginVertexList()[j].Weights[0].Weight;
                VertexesList[i]->GetMatrixWeightList()[j].y = VertexesList[i]->GetOriginVertexList()[j].Weights[1].Weight;
                VertexesList[i]->GetMatrixWeightList()[j].z = VertexesList[i]->GetOriginVertexList()[j].Weights[2].Weight;
                VertexesList[i]->GetMatrixWeightList()[j].w = VertexesList[i]->GetOriginVertexList()[j].Weights[3].Weight;

                VertexesList[i]->GetMatrixIndexList()[j].x = VertexesList[i]->GetOriginVertexList()[j].Weights[0].RefNodeIndex;
                VertexesList[i]->GetMatrixIndexList()[j].y = VertexesList[i]->GetOriginVertexList()[j].Weights[1].RefNodeIndex;
                VertexesList[i]->GetMatrixIndexList()[j].z = VertexesList[i]->GetOriginVertexList()[j].Weights[2].RefNodeIndex;
                VertexesList[i]->GetMatrixIndexList()[j].w = VertexesList[i]->GetOriginVertexList()[j].Weights[3].RefNodeIndex;

                VertexesList[i]->GetPosList()[j] = VertexesList[i]->GetOriginVertexList()[j].Pos;
            }
        }
        else if (VertexesList[i]->GetMeshVertexesType() == emvtBONE)
        {
            pVertexes = GetVertexesByNodeName( VertexesList[i]->GetBoneNodeName() );
            if (pVertexes && pVertexes != VertexesList[i])
            {
                VertexesList[i]->SetAnimationMatrixIndex( pVertexes->GetAnimationMatrixIndex() );
            }
#ifdef DEBUG_MODE
            VertexesList[i]->GetPosList().resize(VertexesList[i]->GetOriginVertexList().size());
            VertexesList[i]->GetMatrixWeightList().resize(VertexesList[i]->GetOriginVertexList().size());
            VertexesList[i]->GetMatrixIndexList().resize(VertexesList[i]->GetOriginVertexList().size());

            int iSize = (int)VertexesList[i]->GetOriginVertexList().size();
            for ( int j = 0 ; j < iSize ; ++j )
            {
                pVertexes = GetVertexesByNodeName( VertexesList[i]->GetOriginVertexList()[j].Weights[0].GetParentNodeName() );
                if (pVertexes)
                {
                    VertexesList[i]->GetOriginVertexList()[j].Weights[0].RefNodeIndex = pVertexes->GetAnimationMatrixIndex();
                    VertexesList[i]->GetOriginVertexList()[j].Weights[0].pRefNode = pVertexes;
                }

                VertexesList[i]->GetMatrixWeightList()[j].x = VertexesList[i]->GetOriginVertexList()[j].Weights[0].Weight;
                VertexesList[i]->GetMatrixWeightList()[j].y = VertexesList[i]->GetOriginVertexList()[j].Weights[1].Weight;
                VertexesList[i]->GetMatrixWeightList()[j].z = VertexesList[i]->GetOriginVertexList()[j].Weights[2].Weight;
                VertexesList[i]->GetMatrixWeightList()[j].w = VertexesList[i]->GetOriginVertexList()[j].Weights[3].Weight;

                VertexesList[i]->GetMatrixIndexList()[j].x = VertexesList[i]->GetOriginVertexList()[j].Weights[0].RefNodeIndex;
                VertexesList[i]->GetMatrixIndexList()[j].y = VertexesList[i]->GetOriginVertexList()[j].Weights[1].RefNodeIndex;
                VertexesList[i]->GetMatrixIndexList()[j].z = VertexesList[i]->GetOriginVertexList()[j].Weights[2].RefNodeIndex;
                VertexesList[i]->GetMatrixIndexList()[j].w = VertexesList[i]->GetOriginVertexList()[j].Weights[3].RefNodeIndex;

                VertexesList[i]->GetPosList()[j] = VertexesList[i]->GetOriginVertexList()[j].Pos;
            }
#endif DEBUG_MODE
        }
    }
#endif
}

void TKAniMeshRes::Init(void)
{
    __super::Init();

    // �� �޽����� �ִϸ��̼� ������ Ȯ���Ͽ�, �ִϸ��̼� ��Ʈ���� �ε��� ����
    SetAnimationMatrixIndex();

    // �ִϸ��̼� ��Ʈ�����迭�� ��Ʈ���������͸� ����
    SetAnimationMatrixData();

    // �޽��� �ִϸ��̼��� ���ε�(�� �޽���, �θ� �� ��ü�� ������ش�.)
    // ���ÿ� �����������͸� �������� VertexList/����ġ/�ε��� �� �����������̳ʿ� ���� ä���ش�.
    MeshAnimationBind();

    //// ���͸���� �޽��� ���ε�
    //MaterialBind(BoneList);
    //MaterialBind(GeomList);

    //// ���͸��� ����Ʈ �ʱ�ȭ
    //MATERIALLIST::iterator it;
    //for ( it = MaterialList.begin() ; it != MaterialList.end() ; ++it )
    //{
    //    it->second->Init();
    //}

    //// ���ؽ��� ����Ʈ �ʱ�ȭ
    //for ( int i = 0 ; i < (int)VertexesList.size() ; ++i )
    //{
    //    VertexesList[i]->Init();
    //}

    //// �ε����� ����Ʈ �ʱ�ȭ
    //for ( int i = 0 ; i < (int)FacesList.size() ; ++i )
    //{
    //    FacesList[i]->Init();
    //}

    //// �� ����Ʈ �ʱ�ȭ
    //for ( int i = 0 ; i < (int)BoneList.size() ; ++i )
    //{
    //    BoneList[i]->Init();
    //}

    //// �޽� ���� �ʱ�ȭ
    //for ( int i = 0 ; i < (int)GeomList.size() ; ++i )
    //{
    //    GeomList[i]->Init();
    //}
}

void TKAniMeshRes::Release(void)
{
    if (m_pMatrixArray)
    {
        SAFEARRAYDELETE<D3DXMATRIX>(m_pMatrixArray);
    }

    __super::Release();
}

void TKAniMeshRes::Copy_BaseInfo( TKAniMeshRes & DestMeshRes, TKAniMeshRes & SrcMeshRes )
{
    // ��������/�޽������͸� ������ ������ �����͸� �����Ѵ�.
    __super::Copy_BaseInfo( DestMeshRes, SrcMeshRes );

    DestMeshRes.m_FirstFrame = SrcMeshRes.m_FirstFrame;
    DestMeshRes.m_LastFrame = SrcMeshRes.m_LastFrame;
    DestMeshRes.m_FrameSpeed = SrcMeshRes.m_FrameSpeed;
    DestMeshRes.AniMotionList = SrcMeshRes.AniMotionList;
    // DestMeshRes.m_pMatrixArray; <- �������. ��? ��ü�ϱ�.
    DestMeshRes.pShaderEffect = SrcMeshRes.pShaderEffect;
}

pair<int, D3DXMATRIX *> TKAniMeshRes::GetAnimationMatrix(int CurrFrame)
{
    pair<int, D3DXMATRIX *> RtnValue;
#if 0
    if ( m_MatrixArrayColCount > 0 )
    {
        RtnValue.first = m_MatrixArrayColCount;
        RtnValue.second = m_pMatrixArray + CurrFrame * m_MatrixArrayColCount;
    }
    else
    {
        RtnValue.first = 1;
        RtnValue.second = GetEngine()->GetMatrixIdentity();
    }
#endif
    return RtnValue;
}

const ST_ANIMOTION TKAniMeshRes::GetAniMotionByName(string MotionName)
{
    ST_ANIMOTION result;
    MOTIONLIST::iterator it;

    it = AniMotionList.find( MotionName );
    if (it != AniMotionList.end())
    {
        return it->second;
    }
    else
    {
        return result;
    }
}

bool TKAniMeshRes::LoadMotionFile(string FileName)
{
#if 0
	TKMotionFileConverter MotionFileConverter;
    return MotionFileConverter.ConvertFromKNF( FileName, GetEngine(), this );
#else
	return false;
#endif
}

bool TKAniMeshRes::SaveMotionFile(string FileName)
{
#if 0
	TKMotionFileConverter MotionFileConverter;
    return MotionFileConverter.ConvertToKNF( FileName, GetEngine(), this );
#else
	return false;
#endif
}

bool TKAniMeshRes::LoadFromFile(string FileName)
{
    if (strstr(FileName.data(), ".KMF"))
    {
        TKMAXFileConverter MAXFileConverter;
        return MAXFileConverter.ConvertFromKMF(FileName, this);
    }
    if (strstr(FileName.data(), ".KAF"))
    {
        TKMAXFileConverter MAXFileConverter;
        return MAXFileConverter.ConvertFromKMF(FileName, this);
    }
    if (strstr(FileName.data(), ".KMT"))
    {
        TKModelFileConverter ModelFileConverter;
        return ModelFileConverter.ConvertFromKMT(FileName, this);
    }
    return false;
}

bool TKAniMeshRes::SaveToFile(string FileName)
{
    if (strstr(FileName.data(), ".KMT"))
    {
        TKModelFileConverter ModelFileConverter;
        return ModelFileConverter.ConvertToKMT(FileName, this);
    }
    return false;
}

bool TKAniMeshRes::LoadLodFile(string FileName)
{
    TKLODFileConverter LODFileConverter;
    return LODFileConverter.ConvertFromKLF( FileName, this );
}

bool TKAniMeshRes::SaveLodFile(string FileName)
{
    TKLODFileConverter LODFileConverter;
    return LODFileConverter.ConvertToKLF( FileName, this );
}