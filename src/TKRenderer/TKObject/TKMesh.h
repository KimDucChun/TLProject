#pragma once
#include "TKRenderObject.h"
#include "TKMeshResource.h"

class TKTexture;
class TKTextureList;
class TKSceneGraphNode;
class TKSGFSObject;

#define TKMeshRS_Class TKMeshRSObject<DATATYPE, RENDERRESOURCE>
#define TKMeshFS_Class TKMeshFSObject<DATATYPE, RENDERRESOURCE>
#define TKMesh_Class TKMesh<FRAMESIDEOBJECT, RENDERSIDEOBJECT, RENDERRESOURCE>

////////////////////////////////////////////////////////////////////////////////
// ST_MESHDATA
////////////////////////////////////////////////////////////////////////////////
struct ST_MESHDATA
{
    float       CameraDistance; // ī�޶���� �Ÿ�
    D3DXVECTOR4 BlendColor;     // ������ �÷�
    
    D3DXMATRIX  matWorld;       // �����Ʈ����
    D3DXMATRIX  matWVP;         // World * View * Projection
    D3DXMATRIX  matWIT;         // World(Scale*Rotation) Inverse
    D3DXMATRIX  matWLP;         // World * LightView * LightProjection
    D3DXMATRIX  matWLPB;        // World * LightView * LightProjection * ScaleBias
    D3DXVECTOR4 vLightDir;      // LightDir;
    
    TKSceneGraphNode *pNode;    // �ű׷��� ���
};

////////////////////////////////////////////////////////////////////////////////
// TKMeshRSObject
////////////////////////////////////////////////////////////////////////////////
template_R_FrameSide
class TKMeshRSObject : public TKRenderRSObject<DATATYPE, RENDERRESOURCE>
{
    template_R_RenderSide friend class TKMeshFSObject;
    template_R_Object friend class TKMesh;
    typedef TKRenderRSObject<DATATYPE, RENDERRESOURCE> BASECLASS;
    
protected:
#ifdef TK_D3D9
	void SetStreamSourceOnRS(int & StreamSourceIndex, LPDIRECT3DVERTEXBUFFER9 pVertexBuffer, int Stride)
    {
        if (pVertexBuffer)
        {
            R_D3DDEVICE->SetStreamSource( StreamSourceIndex, pVertexBuffer, 0, Stride );
            ++StreamSourceIndex;
        }    
    }
#endif

    void SetRenderStateOnRS(TKMeshMaterial *pMaterial);
    void SetTextureWithShader(TKMeshShaderEffect *pShaderEffect, TKMeshMaterial *pMaterial);

    void RenderDepthBufferShadow(GEOMLIST & GeomObjectList);
    void RenderBoneMesh(GEOMLIST & GeomObjectList);    
    void RenderGeometryWithShader(GEOMLIST & GeomObjectList);
    void RenderGeometryPipeline(GEOMLIST & GeomObjectList);
    void RenderGeometryTest(GEOMLIST & GeomObjectList);
        
public:
    TKMeshRSObject(TKEngine *pEngine, TKEngineObject *pParent);
    virtual ~TKMeshRSObject(void);

    virtual void Init(void);
    virtual void Render(const ERenderFuncType RenderType);

    float GetCameraDistance(void)           { return GetData()->CameraDistance; }
    void SetCameraDistance(float Distance)  { GetData()->CameraDistance = Distance; }
};

////////////////////////////////////////////////////////////////////////////////
// TKMeshFSObject
////////////////////////////////////////////////////////////////////////////////
template_R_RenderSide
class TKMeshFSObject : public TKRenderFSObject<DATATYPE, RENDERRESOURCE>
{
    template_R_Object friend class TKMesh;
    typedef TKRenderFSObject<DATATYPE, RENDERRESOURCE> BASECLASS;

private:
    TKSGFSObject* pParentSGFS;      // SGNode�� ������ �ִ� �θ� SGFS ��ü
    
    D3DXMATRIX  m_matLocal;         // ���ø�Ʈ����

public:
    TKMeshFSObject(TKEngine *pEngine, TKEngineObject *pParent);
    virtual ~TKMeshFSObject(void);
    
    virtual void DataSync(TKRenderSideObject *pRSObject)
    {
        __super::DataSync(pRSObject);
        SAMETYPEMEMCPY( ((TKMeshRSObject<DATATYPE, RENDERRESOURCE>*)pRSObject)->GetData(), this->GetData() );
    }    
    
    virtual void Init(void);
    virtual void Frame(void);
    virtual void Release(void);
    
    // ��ü�� ���� ��Ʈ������ �����´�.(�ֻ��� ��ü�� ��� �� ���� �����Ʈ������ ���ֵȴ�.)    
    D3DXMATRIX & GetmatLocal(void)          { return m_matLocal; }
    void SetmatLocal(D3DXMATRIX & matLocal) { this->m_matLocal = matLocal; }

    // ī�޶���� �Ÿ��� �����Ѵ�.
    float GetCameraDistance(void)           { return GetData()->CameraDistance; }
    void SetCameraDistance(float Distance)  { GetData()->CameraDistance = Distance; }

    // ���� ��ü�� SGObject �����͸� ������ �ִ´�.    
    TKSGFSObject * GetParentSGFS(void)              { return pParentSGFS; }
    void SetParentSGFS(TKSGFSObject * pParentSGFS)  { this->pParentSGFS = pParentSGFS; }
};

////////////////////////////////////////////////////////////////////////////////
// TKMesh
////////////////////////////////////////////////////////////////////////////////
template_R_Object
class TKMesh : public TKRenderObject<FRAMESIDEOBJECT, RENDERSIDEOBJECT, RENDERRESOURCE>
{
    typedef TKRenderObject<FRAMESIDEOBJECT, RENDERSIDEOBJECT, RENDERRESOURCE> BASECLASS;
public:
    TKMesh( TKEngine *pEngine, TKEngineObject *pParent, int ID, int Seq );
    virtual ~TKMesh(void);

	void SetMeshScale(float ScaleValue);
	void SetMeshRotationLeft(float RotationValue);
	void SetMeshRotationRight(float RotationValue);
	void SetMeshPosition(D3DXVECTOR3 PositionValue);
};

////////////////////////////////////////////////////////////////////////////////

#define TKBaseMeshRS TKMeshRSObject<ST_MESHDATA, TKMeshRes>
#define TKBaseMeshFS TKMeshFSObject<ST_MESHDATA, TKMeshRes>
#define TKBaseMesh TKMesh<TKBaseMeshRS, TKBaseMeshFS, TKMeshRes>

#include "./TKMesh.hpp"
