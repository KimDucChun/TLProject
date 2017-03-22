#include "./TKCamera.h"
#include "./TKSGObject.h"

////////////////////////////////////////////////////////////////////////////////
// TKCameraRSObject
////////////////////////////////////////////////////////////////////////////////
TKCameraRSObject::TKCameraRSObject(TKEngine *pEngine, TKEngineObject *pParent)
:TKRenderSideObject(pEngine, pParent)
{
    pData = new ST_CAMERADATA;
    memset(pData, 0, sizeof(ST_CAMERADATA));
}

TKCameraRSObject::~TKCameraRSObject(void)
{
    SAFEDELETE(pData);
}

////////////////////////////////////////////////////////////////////////////////
// TKCameraFSObject
////////////////////////////////////////////////////////////////////////////////
TKCameraFSObject::TKCameraFSObject(TKEngine *pEngine, TKEngineObject *pParent)
:TKFrameSideObject(pEngine, pParent)
{
    pData = new ST_CAMERADATA;
    memset(pData, 0, sizeof(ST_CAMERADATA));

    XMVECTOR LookAtPt(0.0f, 0.0f, 0.0f);
    
    pData->EyePt = XMVECTOR(0.0f, 0.0f, -100.0f);
    pData->UpVec = XMVECTOR(0.0f, 1.0f, 0.0f);
    pData->LookVec = LookAtPt - pData->EyePt;
}

TKCameraFSObject::~TKCameraFSObject(void)
{
    SAFEDELETE(pData);
}

void TKCameraFSObject::SetViewMatrix(void)
{
    //ZeroMemory(&pData->matView, sizeof(XMMATRIX));
    //D3DXMatrixLookAtLH(&pData->matView, &pData->EyePt, &pData->LookVec, &pData->UpVec);
    
    D3DXMatrixIdentity(&pData->matView);

    pData->matView._11 = pData->RightVec.x;
    pData->matView._21 = pData->RightVec.y;
    pData->matView._31 = pData->RightVec.z;

    pData->matView._12 = pData->UpVec.x;
    pData->matView._22 = pData->UpVec.y;
    pData->matView._32 = pData->UpVec.z;

    pData->matView._13 = pData->LookVec.x;
    pData->matView._23 = pData->LookVec.y;
    pData->matView._33 = pData->LookVec.z;

    pData->matView._41 = (pData->EyePt.x * pData->RightVec.x + pData->EyePt.y * pData->RightVec.y + pData->EyePt.z * pData->RightVec.z) * -1.0f;;
    pData->matView._42 = (pData->EyePt.x * pData->UpVec.x + pData->EyePt.y * pData->UpVec.y + pData->EyePt.z * pData->UpVec.z) * -1.0f;
    pData->matView._43 = (pData->EyePt.x * pData->LookVec.x + pData->EyePt.y * pData->LookVec.y + pData->EyePt.z * pData->LookVec.z) * -1.0f;

	//pData->matView._41 = -D3DXVec3Dot(&pData->RightVec, &pData->EyePt);
	//pData->matView._42 = -D3DXVec3Dot(&pData->UpVec, &pData->EyePt);
	//pData->matView._43 = -D3DXVec3Dot(&pData->LookVec, &pData->EyePt);
}

void TKCameraFSObject::Init(void)
{
    TKFrameSideObject::Init();
    
    D3DXVec3Normalize(&pData->LookVec, &pData->LookVec);

    D3DXVec3Cross(&pData->RightVec, &pData->UpVec, &pData->LookVec);
    D3DXVec3Normalize(&pData->RightVec, &pData->RightVec);

    D3DXVec3Cross(&pData->UpVec, &pData->LookVec, &pData->RightVec);
    D3DXVec3Normalize(&pData->UpVec, &pData->UpVec);

    SetViewMatrix();    
}

void TKCameraFSObject::Frame(void)
{
    TKFrameSideObject::Frame();
}