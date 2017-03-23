#include "./TKCamera.h"
//#include "./TKSGObject.h"

////////////////////////////////////////////////////////////////////////////////
// TKCamera
////////////////////////////////////////////////////////////////////////////////
TKCamera::TKCamera(TKEngineObject *pParent, int ID, int Seq)
:TKEngineObject(pParent, ID, Seq)
{
    pData = new ST_CAMERADATA;
    memset(pData, 0, sizeof(ST_CAMERADATA));

	XMVECTOR LookAtPt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	pData->EyePt = XMVectorSet(0.0f, 0.0f, -100.0f, 0.0f);
	pData->UpVec = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	pData->LookVec = LookAtPt - pData->EyePt;
}

TKCamera::~TKCamera(void)
{
    SAFEDELETE(pData);
}

void TKCamera::SetViewMatrix(void)
{
    //ZeroMemory(&pData->matView, sizeof(XMMATRIX));
    //D3DXMatrixLookAtLH(&pData->matView, &pData->EyePt, &pData->LookVec, &pData->UpVec);

	//pData->matView = XMMatrixIdentity();

	pData->matView.r[0] = XMVectorSet(pData->RightVec._mX, pData->UpVec._mX, pData->LookVec._mX, 0.0f);
	pData->matView.r[1] = XMVectorSet(pData->RightVec._mY, pData->UpVec._mY, pData->LookVec._mY, 0.0f);
	pData->matView.r[2] = XMVectorSet(pData->RightVec._mZ, pData->UpVec._mZ, pData->LookVec._mZ, 0.0f);
	pData->matView.r[3] = XMVectorSet(
		(pData->EyePt._mX * pData->RightVec._mX + pData->EyePt._mY * pData->RightVec._mY + pData->EyePt._mZ * pData->RightVec._mZ) * -1.0f,
		(pData->EyePt._mX * pData->UpVec._mX + pData->EyePt._mY * pData->UpVec._mY + pData->EyePt._mZ * pData->UpVec._mZ) * -1.0f,
		(pData->EyePt._mX * pData->LookVec._mX + pData->EyePt._mY * pData->LookVec._mY + pData->EyePt._mZ * pData->LookVec._mZ) * -1.0f,
		0.0f);

#if 0
	pData->matView.r[0].n128_f32[0] = pData->RightVec.x;
    pData->matView.r[1].n128_f32[0] = pData->RightVec.y;
	pData->matView.r[2].n128_f32[0] = pData->RightVec.z;

    pData->matView._12 = pData->UpVec.x;
    pData->matView._22 = pData->UpVec.y;
    pData->matView._32 = pData->UpVec.z;

    pData->matView._13 = pData->LookVec.x;
    pData->matView._23 = pData->LookVec.y;
    pData->matView._33 = pData->LookVec.z;

    pData->matView._41 = (pData->EyePt.x * pData->RightVec.x + pData->EyePt.y * pData->RightVec.y + pData->EyePt.z * pData->RightVec.z) * -1.0f;
    pData->matView._42 = (pData->EyePt.x * pData->UpVec.x + pData->EyePt.y * pData->UpVec.y + pData->EyePt.z * pData->UpVec.z) * -1.0f;
    pData->matView._43 = (pData->EyePt.x * pData->LookVec.x + pData->EyePt.y * pData->LookVec.y + pData->EyePt.z * pData->LookVec.z) * -1.0f;
#endif

	//pData->matView._41 = -D3DXVec3Dot(&pData->RightVec, &pData->EyePt);
	//pData->matView._42 = -D3DXVec3Dot(&pData->UpVec, &pData->EyePt);
	//pData->matView._43 = -D3DXVec3Dot(&pData->LookVec, &pData->EyePt);
}

void TKCamera::Init(void)
{
    __super::Init();
    
	pData->LookVec = XMVector3Normalize(pData->LookVec);
	pData->RightVec = XMVector3Cross(pData->UpVec, pData->LookVec);
	pData->RightVec = XMVector3Normalize(pData->RightVec);
	
	pData->UpVec = XMVector3Cross(pData->LookVec, pData->RightVec);
	pData->UpVec = XMVector3Normalize(pData->UpVec);

#if 0
	D3DXVec3Normalize(&pData->LookVec, &pData->LookVec);

    D3DXVec3Cross(&pData->RightVec, &pData->UpVec, &pData->LookVec);
    D3DXVec3Normalize(&pData->RightVec, &pData->RightVec);

    D3DXVec3Cross(&pData->UpVec, &pData->LookVec, &pData->RightVec);
    D3DXVec3Normalize(&pData->UpVec, &pData->UpVec);
#endif

    SetViewMatrix();    
}

void TKCamera::Frame(void)
{
	__super::Frame();
}

void TKCamera::Release(void)
{

}