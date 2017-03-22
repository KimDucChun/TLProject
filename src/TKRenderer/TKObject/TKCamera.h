#pragma once
#include "TKEngineObject.h"

class TKSGObject;

////////////////////////////////////////////////////////////////////////////////
// ST_CAMERADATA
////////////////////////////////////////////////////////////////////////////////
struct ST_CAMERADATA
{
    XMVECTOR	UpVec;
	XMVECTOR	LookVec;
	XMVECTOR	RightVec;
	XMVECTOR	EyePt;
	XMVECTOR	LightLookAtPt;
	XMVECTOR	matView;
    //TKSGObject *pSGObject;
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
    
    XMVECTOR & GetUpVec(void)                        { return pData->UpVec; }
    void SetUpVec(XMVECTOR & UpVec)                  { pData->UpVec = UpVec; }

    XMVECTOR & GetLookVec(void)                      { return pData->LookVec; }
    void SetLookVec(XMVECTOR & LookVec)              { pData->LookVec = LookVec; }
    
    XMVECTOR & GetRightVec(void)                     { return pData->RightVec; }
    void SetRightVec(XMVECTOR & RightVec)            { pData->RightVec = RightVec; }
    
    XMVECTOR & GetEyePt(void)                        { return pData->EyePt; }
    void SetEyePt(XMVECTOR & EyePt)                  { pData->EyePt = EyePt; }
    
    XMVECTOR & GetLightLookAtPt(void)                { return pData->LightLookAtPt; }
    void SetLightLookAtPt(XMVECTOR & LightLookAtPt)  { pData->LightLookAtPt = LightLookAtPt; }

    XMMATRIX & GetViewMatrix(void)                    { return pData->matView; }
    void SetViewMatrix(XMMATRIX & matView)            { pData->matView = matView; }

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

protected:
	ST_CAMERADATA   *pData;

	void SetViewMatrix(void);
    
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

	XMVECTOR & GetUpVec(void) { return pData->UpVec; }
	void SetUpVec(XMVECTOR & UpVec) { pData->UpVec = UpVec; }

	XMVECTOR & GetLookVec(void) { return pData->LookVec; }
	void SetLookVec(XMVECTOR & LookVec) { pData->LookVec = LookVec; }

	XMVECTOR & GetRightVec(void) { return pData->RightVec; }
	void SetRightVec(XMVECTOR & RightVec) { pData->RightVec = RightVec; }

	XMVECTOR & GetEyePt(void) { return pData->EyePt; }
	void SetEyePt(XMVECTOR & EyePt) { pData->EyePt = EyePt; }

	XMVECTOR & GetLightLookAtPt(void) { return pData->LightLookAtPt; }
	void SetLightLookAtPt(XMVECTOR & LightLookAtPt) { pData->LightLookAtPt = LightLookAtPt; }

	XMMATRIX & GetViewMatrix(void) { return pData->matView; }
	void SetViewMatrix(XMMATRIX & matView) { pData->matView = matView; }

	TKSGObject * GetLookAtObject(void) { return pData->pSGObject; }
	void SetLookAtObject(TKSGObject *pSGObject) { pData->pSGObject = pSGObject; }
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
