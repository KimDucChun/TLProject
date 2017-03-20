#pragma once
#include "TKEngineObject.h"

class TKSGObject;

////////////////////////////////////////////////////////////////////////////////
// ST_CAMERADATA
////////////////////////////////////////////////////////////////////////////////
struct ST_CAMERADATA
{
    D3DXVECTOR3 UpVec;
    D3DXVECTOR3 LookVec;
    D3DXVECTOR3 RightVec;
    D3DXVECTOR3 EyePt;
    D3DXVECTOR3 LightLookAtPt;
    D3DXMATRIX  matView;
    TKSGObject *pSGObject;
};

////////////////////////////////////////////////////////////////////////////////
// TKCameraRSObject
////////////////////////////////////////////////////////////////////////////////
class TKCameraRSObject : public TKRenderSideObject
{
    friend class TKCameraFSObject;
private:
    virtual void BeforeRender(const ERenderFuncType RenderType) {;}
    virtual void Render(const ERenderFuncType RenderType)       {;}
    virtual void AfterRender(const ERenderFuncType RenderType)  {;}
protected:
    ST_CAMERADATA   *pData;

public:
    TKCameraRSObject(TKEngine *pEngine, TKEngineObject *pParent);
    virtual ~TKCameraRSObject(void);
    
    D3DXVECTOR3 & GetUpVec(void)                        { return pData->UpVec; }
    void SetUpVec(D3DXVECTOR3 & UpVec)                  { pData->UpVec = UpVec; }

    D3DXVECTOR3 & GetLookVec(void)                      { return pData->LookVec; }
    void SetLookVec(D3DXVECTOR3 & LookVec)              { pData->LookVec = LookVec; }
    
    D3DXVECTOR3 & GetRightVec(void)                     { return pData->RightVec; }
    void SetRightVec(D3DXVECTOR3 & RightVec)            { pData->RightVec = RightVec; }
    
    D3DXVECTOR3 & GetEyePt(void)                        { return pData->EyePt; }
    void SetEyePt(D3DXVECTOR3 & EyePt)                  { pData->EyePt = EyePt; }
    
    D3DXVECTOR3 & GetLightLookAtPt(void)                { return pData->LightLookAtPt; }
    void SetLightLookAtPt(D3DXVECTOR3 & LightLookAtPt)  { pData->LightLookAtPt = LightLookAtPt; }
    
    D3DXMATRIX & GetViewMatrix(void)                    { return pData->matView; }
    void SetViewMatrix(D3DXMATRIX & matView)            { pData->matView = matView; }

    TKSGObject * GetLookAtObject(void)            { return pData->pSGObject; }
    void SetLookAtObject(TKSGObject *pSGObject)   { pData->pSGObject = pSGObject; }
};

////////////////////////////////////////////////////////////////////////////////
// TKCameraFSObject
////////////////////////////////////////////////////////////////////////////////
class TKCameraFSObject : public TKFrameSideObject
{
protected:
    ST_CAMERADATA   *pData;
    
    void SetViewMatrix(void);
    
public:
    TKCameraFSObject(TKEngine *pEngine, TKEngineObject *pParent);
    virtual ~TKCameraFSObject(void);

    virtual void Init(void);
    virtual void BeforeFrame(void)  {;}
    virtual void Frame(void);
    virtual void AfterFrame(void)   {;}
    
    virtual void DataSync( TKRenderSideObject *pRSObject )
    {
        DataSyncFlag( pRSObject );
        
        SAMETYPEMEMCPY( ((TKCameraRSObject*)pRSObject)->pData, this->pData );
    }
    
    D3DXVECTOR3 & GetUpVec(void)                        { return pData->UpVec; }
    void SetUpVec(D3DXVECTOR3 & UpVec)                  { pData->UpVec = UpVec; }

    D3DXVECTOR3 & GetLookVec(void)                      { return pData->LookVec; }
    void SetLookVec(D3DXVECTOR3 & LookVec)              { pData->LookVec = LookVec; }
    
    D3DXVECTOR3 & GetRightVec(void)                     { return pData->RightVec; }
    void SetRightVec(D3DXVECTOR3 & RightVec)            { pData->RightVec = RightVec; }
    
    D3DXVECTOR3 & GetEyePt(void)                        { return pData->EyePt; }
    void SetEyePt(D3DXVECTOR3 & EyePt)                  { pData->EyePt = EyePt; }
    
    D3DXVECTOR3 & GetLightLookAtPt(void)                { return pData->LightLookAtPt; }
    void SetLightLookAtPt(D3DXVECTOR3 & LightLookAtPt)  { pData->LightLookAtPt = LightLookAtPt; }

    D3DXMATRIX & GetViewMatrix(void)                    { return pData->matView; }
    void SetViewMatrix(D3DXMATRIX & matView)            { pData->matView = matView; }

    TKSGObject * GetLookAtObject(void)            { return pData->pSGObject; }
    void SetLookAtObject(TKSGObject *pSGObject)   { pData->pSGObject = pSGObject; }
};

//////////////////////////////////////////////////////////////////////////////
// TKCamera
//////////////////////////////////////////////////////////////////////////////
class TKCamera : public TKEngineObject
{
    void SetRes(TKEngineResource * pRes)    {;}
    TKEngineResource * GetRes(void)         {return NULL;}
    
public:
    TKCamera(TKEngine *pEngine, TKEngineObject *pParent, int ID, int Seq)
        :TKEngineObject(pEngine, pParent, ID, Seq)
        {;}    
    virtual ~TKCamera(void)
        {;}
            
    TKEngineObjectList * GetEngineObjectList(void) { return NULL; }

    virtual TKCameraFSObject * GetFS(void) = 0;
    virtual TKCameraRSObject * GetRS(void) = 0;

    virtual void BeforeInit(void) = 0;
    virtual void AfterInit(void) = 0;   
};

//////////////////////////////////////////////////////////////////////////////
// TKCustomCamera
//////////////////////////////////////////////////////////////////////////////
template<class FRAMESIDEOBJECT, class RENDERSIDEOBJECT>
class TKCustomCamera : public TKCamera
{
protected:
    FRAMESIDEOBJECT * pFSObject;
    RENDERSIDEOBJECT * pRSObject;

public:
    TKCustomCamera(TKEngine *pEngine, TKEngineObject *pParent, int ID, int Seq);
    virtual ~TKCustomCamera(void);

    virtual FRAMESIDEOBJECT * GetFS(void)   { return pFSObject; }
    virtual RENDERSIDEOBJECT * GetRS(void)  { return pRSObject; }

	TKFrameSideObject * GetFSRaw(void)	{ return pFSObject; }
	TKRenderSideObject * GetRSRaw(void) { return pRSObject; }

    virtual void BeforeInit(void)   {;}
    virtual void Init(void);
    virtual void AfterInit(void)    {;}
  
    virtual void Release(void);    
};

//////////////////////////////////////////////////////////////////////////////
#include "./TKCamera.hpp"
