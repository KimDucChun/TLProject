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
	XMMATRIX	matView;
    //TKSGObject *pSGObject;
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
	TKCamera(TKEngineObject *pParent, int ID, int Seq);
	~TKCamera();
            
    TKEngineObjectList * GetEngineObjectList(void) { return NULL; }

	void Init(void);
	void Frame(void);
	void Release(void);

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

	//TKSGObject * GetLookAtObject(void) { return pData->pSGObject; }
	//void SetLookAtObject(TKSGObject *pSGObject) { pData->pSGObject = pSGObject; }
};
