//#include "TKCollisionChecker.h"
//#include "TKFileConvertClasses.h"
#include "TKTexture.h"
#include "TKIMEInput.h"

//#include "TKCustomScene.h"
//#include "TKSceneManager.h"

//#include "TKTR_Render.h"
//#include "TKTR_Frame.h"
//#include "TKTR_DataSync.h"
//#include "TKTR_ApplyContainer.h"

#include "TKEngine.h"
#include "TKD3D.h"
#include "TKInput.h"
//#include "TKPicker.h"
#include "TKTime.h"

//#include "TKEngineResource.h"
//#include "TKBaseObjectList.h"
//#include "TKRenderFuncList.h"
//#include "TKEngineObjectList.h"
//#include "TKSGObjectList.h"
//#include "TKRenderQueue.h"
//#include "TKSceneGraph.h"
//#include "TKSceneGraphManager.h"

//#include "TKRenderFunc.h"
//
//#include "TKGeneralRender.h"
//#include "TKDepthBufferShadow.h"
//
//#include "TKCamera.h"
//#include "TKDebugCamera.h"
//#include "TKSpringCamera.h"
//#include "TKStaticCamera.h"
//
//#include "TKLight.h"
//
//#include "TKRenderObject.h"
//#include "TKCollisionObject.h"
//
//#include "TKSprite.h"
//#include "TKHeightMap.h"
//#include "TKBillBoard.h"
//#include "TKDebug.h"
//#include "TKSkyBox.h"
//#include "TKFrustum.h"
//#include "TKCustomUI.h"

#define MULTI_THREAD

TKEngine::TKEngine(HINSTANCE hInstance, HWND hWnd)
:TKBaseObject(NULL, -1)
//,EngineRootDir("")
//,DeviceLost(false)
//,pExternFrame(NULL)
//,pTextures(NULL)
//,pCameraList(NULL)
//,pEngineResourceList(NULL)
//,pLightList(NULL)
//,pSGObjectList(NULL)
//,p3DObjectList(NULL)
//,p2DObjectList(NULL)
//,pRenderQueue(NULL)
//,pSceneGraphManager(NULL)
//,pGeneralRender(NULL)
//,pDepthBufferShadow(NULL)
//,pDebug(NULL)
//,pSkyBox(NULL)
//,pFrustum(NULL)
//,pFocusedControl(NULL)
//,m_pD3D(NULL)
//,pTime(NULL)
//,pInput(NULL)
//,pPicker(NULL)
//,pDebugRes(NULL)
//,pSpriteRes(NULL)
//,pBillBoardRes(NULL)
//,pUIRes(NULL)
//,pCollisionRenderRes(NULL)
//,pDummyCollRes(NULL)
//,pLightGlobal(NULL)
//,EngineShadowType(eestDepthBufferProjected)
//// 포그 관련 초기화
//,bFogEnable(FALSE)
//,fFogNear(0.0f)
//,fFogFar(20000.0f)
//,vecFogColor(1.0f, 1.0f, 1.0f, 1.0f)
//,vecFogVector(0.0f, 0.0f, 0.0f, 0.0f)
//// D3D파라메터 초기화
//,D3DFILLMODE( D3DFILL_SOLID )
//,NearDistance( 10.0f )
//,FarDistance( 100000.0f )
//,ModelDir("")
//,FXDir("")
//,CollAreaVisible(false)
//,BoneVisible(false)
//,MeshVisible(true)
{
//#ifdef DEBUG_MODE
//    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
//#endif DEBUG_MODE
//    // Fog기본값 설정
//    vecFogVector.x = fFogFar/(fFogFar-fFogNear);
//	vecFogVector.y = -1.0f/(fFogFar-fFogNear);
//
//    D3DXMatrixIdentity(&matIdentity);
//    
//    this->hInstance = hInstance;
//    this->hWnd = hWnd;
//
//    m_pD3D = new TKD3D(this);
//    pTime = new TKTime(this);
//    pInput = new TKInput(this);
//    pPicker = new TKPicker(this);
//    pIMEInput = new TKIMEInput(this);
//
//    pSceneManager = new TKSceneManager(this);
//    pCollisionChecker = new TKCollisionChecker(this);
//
//    pTextures = new TKTextureManager(this, true);
//    pEngineResourceList = new TKBaseObjectList(this, true);
//    
//    pCameraList = new TKEngineObjectList(this, NULL, true);
//    pLightList = new TKEngineObjectList(this, NULL, true);
//    pSGObjectList = new TKSGObjectList(this, NULL, true);
//    p3DObjectList = new TKEngineObjectList(this, NULL, true);
//    p2DObjectList = new TKEngineObjectList(this, NULL, true);
//    pRenderQueue = new TKRenderQueue(this);
//    pSceneGraphManager = new TKSceneGraphManager(this);
//    pRenderFuncList = new TKRenderFuncList(this);
//    
//    pFrustum = new TKFrustum(this);
//#ifdef DEBUG_MODE
//    memset(DebugStr, 0, 100);
//    pDebug = new TKDebug(this);    
//    pDebugRes = pEngineResourceList->New<TKDebugRes>();
//#endif DEBUG_MODE
//    pSpriteRes = pEngineResourceList->New<TKSpriteRes>();
//    pBillBoardRes = pEngineResourceList->New<TKBillBoardRes>();
//    pUIRes = pEngineResourceList->New<TKCustomUIRes>();
//    pCollisionRenderRes = pEngineResourceList->New<TKCollisionRenderRes>();
//    pDummyCollRes = pEngineResourceList->New<TKCollisionObjectRes>();
//    pDummyCollRes->SetIsDynamicObject(false);
//    pDummyCollRes->SetIsMeshAnimation(false);
//    pDummyCollRes->SetMeshRes(NULL);
//    pDummyCollRes->SetBoneNodeName( string("") );
//    
//    pEngineResourceList->ApplyNew();
//    
//    // 전역 확산광/환경광 객체
//    pLightGlobal = pLightList->New<TKCustomLight<TKLightFSObject, TKLightRSObject>>(0);
//    pLightGlobal->GetFS()->SetLightType(eltDIRECTION);
//    pLightGlobal->GetFS()->SetDynamicLight( true );
//    pLightGlobal->GetFS()->SetPos( XMFLOAT3( 0, 0, 0) );
//    pLightGlobal->GetFS()->SetDiffuse( XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) );
//    pLightGlobal->GetFS()->SetAmbient( XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f) );
//    pLightGlobal->GetFS()->SetSpecular( XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) );
//    pLightGlobal->GetFS()->SetDir( XMFLOAT3( 0.0f, -1.0, 1.0f ) );
//    pLightList->ApplyNew();
//
//    // 기본 카메라 객체 생성
//    pCameraList->New<TKDebugCamera>(0, string("DEBUGCAMERA"));
//    pCameraList->New<TKSpringCamera>(0, string("SPRINGCAMERA"));
//    pCameraList->New<TKStaticCamera>(0, string("STATICCAMERA"));
//    this->SetCamera( pCameraList->Item<TKDebugCamera>(string("DEBUGCAMERA")) );
//    pCameraList->ApplyNew();
//    
//    // 렌더객체 생성
//    pDepthBufferShadow = pRenderFuncList->New<TKDepthBufferShadow>(0x7FFFFFFE);
//    pGeneralRender = pRenderFuncList->New<TKGeneralRender>(0x7FFFFFFF);
}

TKEngine::~TKEngine(void)
{   
//	SAFEDELETE(pEngineResourceList);
//    SAFEDELETE(pCameraList);    
//    SAFEDELETE(pLightList);
//    SAFEDELETE(pSGObjectList);
//    SAFEDELETE(p3DObjectList);
//    SAFEDELETE(p2DObjectList);    
//    SAFEDELETE(pRenderQueue);
//    SAFEDELETE(pSceneGraphManager);
//    SAFEDELETE(pRenderFuncList);
//
//    SAFEDELETE(pSkyBox);
//#ifdef DEBUG_MODE
//    SAFEDELETE(pDebug);
//#endif DEBUG_MODE
//    SAFEDELETE(pFrustum);
//
//    SAFEDELETE(pSceneManager);
//    SAFEDELETE(pCollisionChecker);
//
//    SAFEDELETE(pIMEInput);
//    SAFEDELETE(pInput);
//    SAFEDELETE(pPicker);
//    SAFEDELETE(pTime);
//    SAFEDELETE(m_pD3D);
//    SAFEDELETE(pTextures);
}

void TKEngine::Init(void)
{
//    __super::Init();
//
//    m_pD3D->Init();
//    pTime->Init();
//    pInput->Init();
//    pPicker->Init();
//    pFrustum->Init();
//    if (pSkyBox) pSkyBox->Init();
//    
//    pEngineResourceList->Init();
//    pCameraList->Init();
//    pLightList->Init();
//    pRenderFuncList->Init();
//    
//#ifdef DEBUG_MODE
//    if (pDebug) pDebug->Init();
//#endif DEBUG_MODE
//
//    pSGObjectList->Init();
//    p3DObjectList->Init();
//    p2DObjectList->Init();
//
//    ThreadExit = false;
//    ThreadFirstStep = true;
//    
//#ifdef MULTI_THREAD
//    hFrameRenderStart[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
//    hFrameRenderFinish[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
//    pTR_Frame = new TKTR_Frame(this);
//    pTR_Frame->Resume();
//    
//    SetThreadAffinityMask( pTR_Frame->GetThreadHandle(), 0x01 );
//    
//    hFrameRenderStart[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
//    hFrameRenderFinish[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
//    pTR_Render = new TKTR_Render(this);
//    pTR_Render->Resume();
//    
//    SetThreadAffinityMask( pTR_Render->GetThreadHandle(), 0x02 );
//#endif MULTI_THREAD
}

void TKEngine::Release(void)
{
//	ThreadExit = true;
//#ifdef MULTI_THREAD
//
//	SAFEDELETE(pTR_Render);
//	SAFEDELETE(pTR_Frame);
//	SAFEHANDLECLOSE(hFrameRenderStart[0]);
//	SAFEHANDLECLOSE(hFrameRenderFinish[0]);
//	SAFEHANDLECLOSE(hFrameRenderStart[1]);
//	SAFEHANDLECLOSE(hFrameRenderFinish[1]);
//
//	//delete pTR_DataSync_0;
//	//delete pTR_DataSync_1;
//	//CloseHandle(hDataSyncStartEvent[0]);
//	//CloseHandle(hDataSyncFinishEvent[0]);
//	//CloseHandle(hDataSyncStartEvent[1]);
//	//CloseHandle(hDataSyncFinishEvent[1]);
//
//	//delete pTR_ApplyContainer_0;
//	//delete pTR_ApplyContainer_1;
//	//CloseHandle(hApplyContainerStartEvent[0]);
//	//CloseHandle(hApplyContainerFinishEvent[0]);
//	//CloseHandle(hApplyContainerStartEvent[1]);
//	//CloseHandle(hApplyContainerFinishEvent[1]);
//
//#endif MULTI_THREAD
//
//	if (pSkyBox) pSkyBox->Release();
//#ifdef DEBUG_MODE
//	if (pDebug) pDebug->Release();
//#endif DEBUG_MODE
//	pSGObjectList->Release();
//	p3DObjectList->Release();
//	p2DObjectList->Release();
//	pCameraList->Release();
//	pLightList->Release();
//	pSceneGraphManager->Release();
//	pRenderFuncList->Release();
//	pEngineResourceList->Release();
//
//	pFrustum->Release();
//	pInput->Release();
//	pPicker->Release();
//	pTime->Release();
//	m_pD3D->Release();
//
//	__super::Release();
}

void TKEngine::Frame(void)
{
    //// 고정객체 프레임시퀀스
    //pTime->Frame();
    //pInput->Frame();
    //pCameraList->Frame();
    //pPicker->Frame();    
    //pFrustum->GetFS()->Frame();
    //if (pSkyBox) pSkyBox->GetFS()->Frame();
    //if (pDebug) pDebug->GetFS()->Frame();
    //if (pExternFrame) pExternFrame(pExternFrameObject);
    //
    //pLightList->Frame();
    //pSGObjectList->Frame();
    //p3DObjectList->Frame();
    //p2DObjectList->Frame();

    //int i = 0;
    //for ( i = 0 ; i < pLightList->Count() ; ++i )
    //    pSceneGraphManager->GetSceneGraph()->ClearLightNodes( pLightList->ItemBySeq<TKLight>(i) );
    //for ( i = 0 ; i < pSGObjectList->Count() ; ++i )
    //    pSceneGraphManager->GetSceneGraph()->ClearObjectNodes ( pSGObjectList->ItemBySeq(i) );
    //for ( i = 0 ; i < pLightList->Count() ; ++i )
    //    pSceneGraphManager->GetSceneGraph()->InitLightNodes( pLightList->ItemBySeq<TKLight>(i) );
    //for ( i = 0 ; i < pSGObjectList->Count() ; ++i )
    //    pSceneGraphManager->GetSceneGraph()->InitObjectNodes ( pSGObjectList->ItemBySeq(i) );

    //pSceneGraphManager->Frame();
    //pRenderQueue->Frame();
}

void TKEngine::Render(void)
{
//#if defined(TK_D3D11)
//	if (m_pD3D->GetD3DContext() != nullptr)
//	{
//		//TotalVertexCount = 0;
//		pRenderFuncList->Render();
//	}
//#elif defined(TK_D3D9)
//	if ( m_pD3D->GetD3DDevice() != NULL)
//    {
//        //TotalVertexCount = 0;
//        pRenderFuncList->Render();
//    }
//#endif
}

void TKEngine::ApplyContainer_T0(void)
{
    //pCameraList->ApplyDelete();
    //pCameraList->ApplyNew();

    //pLightList->ApplyDelete();
    //pLightList->ApplyNew();
}

void TKEngine::ApplyContainer_T1(void)
{
    //pSGObjectList->ApplyDelete();
    //pSGObjectList->ApplyNew();
    //
    //p3DObjectList->ApplyDelete();
    //p3DObjectList->ApplyNew();
    //    
    //p2DObjectList->ApplyDelete();
    //p2DObjectList->ApplyNew();
    //
    //pEngineResourceList->ApplyDelete();
    //pEngineResourceList->ApplyNew();
}

void TKEngine::DataSync_T0(void)
{
//    if (pSkyBox) pSkyBox->DataSync();
//#ifdef DEBUG_MODE
//    if (pDebug) pDebug->DataSync();
//#endif DEBUG_MODE
//    pFrustum->DataSync();
//    p3DObjectList->DataSync();
//    p2DObjectList->DataSync();
//    pRenderQueue->DataSync();
}

void TKEngine::DataSync_T1(void)
{
    //pLightList->DataSync();
    //pSGObjectList->DataSync();
    //pSceneGraphManager->GetSceneGraph()->DataSync();
}

void TKEngine::ClearDeleteAllEngineItem(void)
{
    //SAFERELEASEDELETE(pSkyBox);

    //GetCameraList()->Release();
    //GetLightList()->Release();
    //GetSGObjectList()->Release();
    //Get3DObjectList()->Release();
    //Get2DObjectList()->Release();
    //GetEngineResourceList()->Release();

    //GetCameraList()->ClearDelete();
    //GetLightList()->ClearDelete();
    //GetSGObjectList()->ClearDelete();
    //Get3DObjectList()->ClearDelete();
    //Get2DObjectList()->ClearDelete();    
    //GetEngineResourceList()->ClearDelete();
}

XMMATRIX * TKEngine::GetViewMatrix(void)
{
    //return &(m_pD3D->GetCamera()->GetFS()->GetViewMatrix());
	return NULL;
}

XMMATRIX * TKEngine::GetProjectionMatrix(void)
{
    //return m_pD3D->GetMatProjection();
	return NULL;
}

void TKEngine::SetCamera(TKCamera *pCamera)
{
    //m_pD3D->SetCamera(pCamera);
}

TKCamera * TKEngine::GetCamera(void)
{
    //return m_pD3D->GetCamera();
	return NULL;
}

TKTexture * TKEngine::GetTexture(const int Index)
{
    return pTextures->GetTexture(Index);
}

void TKEngine::ShowDebugMessage(string String, int x, int y)
{
    //pDebug->GetRS()->ShowMessage(&String[0], x, y);
}


string & TKEngine::GetEngineRootDir(void)
{
    return EngineRootDir;
}

void TKEngine::SetEngineRootDir(string & RootDir)
{
    EngineRootDir = RootDir;
    string SystemTextureDir;
    SystemTextureDir = EngineRootDir + "Textures\\";
    pTextures->SetSystemTextureDir( SystemTextureDir );
}

string & TKEngine::GetSystemTextureDir(void)
{
    return pTextures->GetSystemTextureDir();
}

string & TKEngine::GetUserTextureDir(void)
{
    return pTextures->GetUserTextureDir();
}

void TKEngine::SetUserTextureDir(string & TextureDir)
{
    pTextures->SetUserTextureDir(TextureDir);
}

string & TKEngine::GetModelDir(void)
{
    return ModelDir;
}

void TKEngine::SetModelDir(string & ModelDir)
{
    this->ModelDir = ModelDir;
}

string & TKEngine::GetFXDir(void)
{
    return FXDir;
}

void TKEngine::SetFXDir(string & FXDir)
{
    this->FXDir = FXDir;
}

bool TKEngine::CheckEngineRootValidate( string & EngineRootDir,
                                        string & UserTextureDir,
                                        string & ModelDir,
                                        string & FXDir)
{
    vector<string> SystemTextureList;
    vector<string> SystemFXList;
    string SystemTextureDir;
    
    if ( SystemTextureList.empty() ) 
    {
        SystemTextureList.push_back( string("ControlBackground.bmp") );
        SystemTextureList.push_back( string("NullAlphaMap.tga") );
        SystemTextureList.push_back( string("ShadowAlphaMap.tga") );
    }
    if ( SystemFXList.empty() ) 
    {
        SystemFXList.push_back( string("Shadow_DepthBuffer.fx") );
        SystemFXList.push_back( string("BoneMesh.fx") );
    }
    SystemTextureDir = EngineRootDir + "Textures\\";

    string strFileCheck;
    for ( int i = 0 ; i < (int)SystemTextureList.size() ; ++i )
    {
        strFileCheck = SystemTextureDir + SystemTextureList[i];
        if ( !IsFileExist( strFileCheck.data() ) ) 
            return false;
    }
    for ( int i = 0 ; i < (int)SystemFXList.size() ; ++i )
    {
        strFileCheck = FXDir + SystemFXList[i];
        if ( !IsFileExist( strFileCheck.data() ) ) 
            return false;
    }
    return true;
}

bool TKEngine::GetFrameShow(void)
{
    //return pDebug->GetFS()->GetData()->FrameShow;
	return false;
}

void TKEngine::SetFrameShow(bool Value)
{
    //pDebug->GetFS()->GetData()->FrameShow = Value;
}

bool TKEngine::GetPosShow(void)
{
    //return pDebug->GetFS()->GetData()->PosShow;
	return false;
}

void TKEngine::SetPosShow(bool Value)
{
    //pDebug->GetFS()->GetData()->PosShow = Value;
}

bool TKEngine::GetAxisShow(void)
{
    //return pDebug->GetFS()->GetData()->AxisShow;
	return false;
}

void TKEngine::SetAxisShow(bool Value)
{
    //pDebug->GetFS()->GetData()->AxisShow = Value;
}

bool TKEngine::GetDeviceLost(void)
{
	//return DeviceLost;
	return false;
}

void TKEngine::SetDeviceLost(bool DeviceLost)
{
	//this->DeviceLost = DeviceLost;
}

TKD3D * TKEngine::GetD3D(void)
{
	return m_pD3D;
}

#if defined(TK_D3D9)
LPDIRECT3DDEVICE9 TKEngine::GetD3DDevice(void)
{
	return m_pD3D->GetD3DDevice();
}
#elif defined(TK_D3D11)
ID3D11DeviceContext * TKEngine::GetD3DContext(void)
{
	return m_pD3D->GetD3DContext();
}
#endif

HWND TKEngine::GethWnd(void)
{
	return hWnd;
}

HINSTANCE TKEngine::GethInstance(void)
{
	return hInstance;
}

TKInput * TKEngine::GetInput(void)
{
	return pInput;
}

TKPicker * TKEngine::GetPicker(void)
{
	return pPicker;
}

TKIMEInput * TKEngine::GetIMEInput(void)
{
	return pIMEInput;
}

TKTime * TKEngine::GetTime(void)
{
	return pTime;
}

TKFrustum * TKEngine::GetFrustum(void)
{
	return pFrustum;
}

TKDebug * TKEngine::GetDebug(void)
{
	return pDebug;
}

void TKEngine::SetSkyBox(TKSkyBox *pSkyBox)
{
	this->pSkyBox = pSkyBox;
}

TKSkyBox * TKEngine::GetSkyBox(void)
{
	return pSkyBox;
}

void * TKEngine::GetFocusedControl(void)
{
	return pFocusedControl;
}

void TKEngine::SetFocusedControl(void * pFocusedControl)
{
	this->pFocusedControl = pFocusedControl;
}

void TKEngine::Lock(void)
{
	CSProcess.Enter();
}

void TKEngine::UnLock(void)
{
	CSProcess.Leave();
}

void TKEngine::SetExternFrameFunc(bool(*pFunc)(void *pObject))
{
	this->pExternFrame = pFunc;
}

void TKEngine::SetExternFrameObject(void *pExternFrameObject)
{
	this->pExternFrameObject = pExternFrameObject;
}

TKBaseObjectList * TKEngine::GetEngineResourceList(void)
{
	return pEngineResourceList;
}

TKEngineObjectList * TKEngine::GetCameraList(void)
{
	return pCameraList;
}

TKEngineObjectList * TKEngine::GetLightList(void)
{
	return pLightList;
}

TKSGObjectList * TKEngine::GetSGObjectList(void)
{
	return pSGObjectList;
}

TKEngineObjectList * TKEngine::Get3DObjectList(void)
{
	return p3DObjectList;
}

TKEngineObjectList * TKEngine::Get2DObjectList(void)
{
	return p2DObjectList;
}

TKTextureManager * TKEngine::GetTextures(void)
{
	return pTextures;
}

int TKEngine::GetTotalVertexCount(void)
{
	return TotalVertexCount;
}

XMMATRIX * TKEngine::GetMatrixIdentity(void)
{
	return &matIdentity;
}

TKSceneManager * TKEngine::GetSceneManager(void)
{
	return pSceneManager;
}

TKCollisionChecker * TKEngine::GetCollisionChecker(void)
{
	return pCollisionChecker;
}

TKDebugRes * TKEngine::GetDebugRes(void)
{
	return pDebugRes;
}

TKSpriteRes * TKEngine::GetSpriteRes(void)
{
	return pSpriteRes;
}

TKBillBoardRes * TKEngine::GetBillBoardRes(void)
{
	return pBillBoardRes;
}

TKCustomUIRes * TKEngine::GetUIRes(void)
{
	return pUIRes;
}

TKCollisionRenderRes * TKEngine::GetCollisionRenderRes(void)
{
	return pCollisionRenderRes;
}

TKCollisionObjectRes * TKEngine::GetDummyCollRes(void)
{
	return pDummyCollRes;
}

TKLight * TKEngine::GetLightGlobal(void)
{
	return pLightGlobal;
}

TKRenderQueue * TKEngine::GetRenderQueue(void)
{
	return pRenderQueue;
}

TKSceneGraphManager * TKEngine::GetSceneGraphManager(void)
{
	return pSceneGraphManager;
}

TKRenderFuncList * TKEngine::GetRenderFuncList(void)
{
	return pRenderFuncList;
}

TKGeneralRender * TKEngine::GetGeneralRender(void)
{
	return pGeneralRender;
}

TKDepthBufferShadow * TKEngine::GetDepthBufferShadow(void)
{
	return pDepthBufferShadow;
}

EEngineShadowType TKEngine::GetShadowType(void)
{
	return EngineShadowType;
}

void TKEngine::SetShadowType(EEngineShadowType EngineShadowType)
{
	this->EngineShadowType = EngineShadowType;
}

BOOL TKEngine::GetFogEnable(void)
{
	return bFogEnable;
}

void TKEngine::SetFogEnable(BOOL FogEnable)
{
	vecFogVector.x = fFogFar / (fFogFar - fFogNear);
	vecFogVector.y = -1.0f / (fFogFar - fFogNear);
	this->bFogEnable = FogEnable;
}

float TKEngine::GetFogNear(void)
{
	return fFogNear;
}

void TKEngine::SetFogNear(float fFogNear)
{
	vecFogVector.x = fFogFar / (fFogFar - fFogNear);
	vecFogVector.y = -1.0f / (fFogFar - fFogNear);
	this->fFogNear = fFogNear;
}

float TKEngine::GetFogFar(void)
{
	return fFogFar;
}

void TKEngine::SetFogFar(float fFogFar)
{
	vecFogVector.x = fFogFar / (fFogFar - fFogNear);
	vecFogVector.y = -1.0f / (fFogFar - fFogNear);
	this->fFogFar = fFogFar;
}

XMFLOAT4 & TKEngine::GetFogColor(void)
{
	return vecFogColor;
}

void TKEngine::SetFogColor(XMFLOAT4 & FogColor)
{
	this->vecFogColor = FogColor;
}

XMFLOAT4 & TKEngine::GetFogVector(void)
{
	return vecFogVector;
}

DWORD TKEngine::GetFillMode(void)
{
	return D3DFILLMODE;
}

void TKEngine::SetFillMode(DWORD FILLMODE)
{
	this->D3DFILLMODE = FILLMODE;
}

float TKEngine::GetNearDistance(void)
{
	return NearDistance;
}

void TKEngine::SetNearDistance(float NearDistance)
{
	this->NearDistance = NearDistance;
}

float TKEngine::GetFarDistance(void)
{
	return FarDistance;
}

void TKEngine::SetFarDistance(float FarDistance)
{
	this->FarDistance = FarDistance;
}

bool TKEngine::GetCollAreaVisible(void)
{
	return CollAreaVisible;
}

void TKEngine::SetCollAreaVisible(bool Visible)
{
	this->CollAreaVisible = Visible;
}

bool TKEngine::GetBoneVisible(void)
{
	return BoneVisible;
}

void TKEngine::SetBoneVisible(bool Visible)
{
	this->BoneVisible = Visible;
}

bool TKEngine::GetMeshVisible(void)
{
	return MeshVisible;
}

void TKEngine::SetMeshVisible(bool Visible)
{
	this->MeshVisible = Visible;
}

bool TKEngine::DoProcess(void)
{
#if 0
	CSProcess.Enter();

    // 디바이스 로스트 확인
    CheckDeviceLost();
    
    // 객체 컨테이너 갱신
#ifdef MULTI_THREAD
    ApplyContainer_T0();
    ApplyContainer_T1();

    //SetEvent(hApplyContainerStartEvent[0]);
    //SetEvent(hApplyContainerStartEvent[1]);
    //WaitForMultipleObjects(2, hApplyContainerFinishEvent, TRUE, INFINITE);    
#else

    ApplyContainer_T0();
    ApplyContainer_T1();
    
#endif MULTI_THREAD

    // 씬관리자 실행
    pSceneManager->Execute();
    pSceneManager->ApplyState();

    //Frame()/Render()/DataSync() 실행한다.
#ifdef MULTI_THREAD
    //if (ThreadFirstStep)
    //{
    //    SetEvent(hFrameRenderStart[0]);
    //    WaitForSingleObject(hFrameRenderFinish[0], INFINITE);
    //    ThreadFirstStep = false;
    //}
    //else
    //{
    //    SetEvent(hFrameRenderStart[0]);
    //    SetEvent(hFrameRenderStart[1]);
    //    WaitForMultipleObjects(2, hFrameRenderFinish, TRUE, INFINITE);
    //}

    SetEvent(hFrameRenderStart[0]);
    SetEvent(hFrameRenderStart[1]);
    WaitForMultipleObjects(2, hFrameRenderFinish, TRUE, INFINITE);
    
    //SetEvent(hFrameRenderStart[0]);
    //WaitForSingleObject(hFrameRenderFinish[0], INFINITE);
    //SetEvent(hFrameRenderStart[1]);
    //WaitForSingleObject(hFrameRenderFinish[1], INFINITE);

    this->DataSync_T0();
    this->DataSync_T1();
        
    //SetEvent(hDataSyncStartEvent[0]);
    //SetEvent(hDataSyncStartEvent[1]);
    //WaitForMultipleObjects(2, hDataSyncFinishEvent, TRUE, INFINITE);
#else
    this->Frame();
    this->DataSync_T0();
    this->DataSync_T1();
    this->Render();
    
#endif MULTI_THREAD

    CSProcess.Leave();
#endif
    return true;
}

TKAniMeshRes * TKEngine::AniMeshResLoad(TKMeshShaderEffect * pMeshShaderEffect, string  MeshFileName, string  AniFileName, string  MotionFile)
{
#if 0
	TKAniMeshRes   *pAniMeshRes;

	pAniMeshRes = this->GetEngineResourceList()->Item<TKAniMeshRes>(MeshFileName);
	if (!pAniMeshRes)
	{
		pAniMeshRes = this->GetEngineResourceList()->New<TKAniMeshRes>(MeshFileName);
		if (!pAniMeshRes->LoadFromFile(this->ModelDir + MeshFileName))
		{
			this->GetEngineResourceList()->Delete(pAniMeshRes);
			return NULL;
		}
		if (!AniFileName.empty())
		{
			if (!pAniMeshRes->LoadFromFile(this->ModelDir + AniFileName))
			{
				this->GetEngineResourceList()->Delete(pAniMeshRes);
				return NULL;
			}
		}
		if (!MotionFile.empty())
		{
			if (!pAniMeshRes->LoadMotionFile(this->ModelDir + MotionFile))
			{
				this->GetEngineResourceList()->Delete(pAniMeshRes);
				return NULL;
			}
		}
		pAniMeshRes->SetShaderEffect(pMeshShaderEffect);
		pAniMeshRes->Init();
	}

	return pAniMeshRes;
#else
	return NULL;
#endif
}

TKCollisionObjectRes * TKEngine::CollResLoad(string  CollFileName, ECollisionObjectType CollObjectType, bool IsDynamicObject, bool IsMeshAnimation, TKMeshRes * pBindMeshRes, string BindNodeName)
{
#if 0
	TKCollisionObjectRes *pCollObjectRes;

	pCollObjectRes = this->GetEngineResourceList()->Item<TKCollisionObjectRes>(CollFileName);
	if (!pCollObjectRes)
	{
		pCollObjectRes = this->GetEngineResourceList()->New<TKCollisionObjectRes>(CollFileName);    // 충돌 리소스 생성
		pCollObjectRes->SetIsDynamicObject(IsDynamicObject);                                    // 동적객체(이동이 가능한, 매 프레임마다 위치를 갱신해야 하는) 객체를 의미
		pCollObjectRes->SetIsMeshAnimation(IsMeshAnimation);                                    // (동적객체 중에서)애니메이션이 가능한 객체라는 것을 의미
		pCollObjectRes->SetMeshRes(pBindMeshRes);                                               // 애니메이션/메쉬 정보를 가진 메쉬 리소스 바인드
		pCollObjectRes->SetBoneNodeName(BindNodeName);                                      // 애니메이션 정보를 참조할 메쉬의 최상위 본 오브젝트 네임(3DSMAX에서의 Bone/Biped 이름을 의미한다)
		if (!CollFileName.empty())
		{
			// 충돌데이터 파일 로딩 (파일명이 없을경우에는 로딩하지 않는다.)
			pCollObjectRes->LoadFromFile(this->ModelDir + CollFileName, CollObjectType);
		}
		pCollObjectRes->Init();
	}

	return pCollObjectRes;
#else
	return NULL;
#endif
}

void TKEngine::CheckDeviceLost(void)
{
#ifdef TK_D3D9
	if (!DeviceLost) return;

	HRESULT hr = m_pD3D->GetD3DDevice()->TestCooperativeLevel();

	if (hr == D3DERR_DEVICELOST)
	{
		return;
	}
	else if (hr == D3DERR_DEVICENOTRESET)
	{
		// 리소스 해제
		pRenderFuncList->Release();
		pEngineResourceList->Release();

		// D3D리셋
		hr = m_pD3D->GetD3DDevice()->Reset(m_pD3D->GetD3Dpp());
		if (FAILED(hr)) return; // 실패했으면 다음루프에 다시 처리

								// 리소스 재생성
		pRenderFuncList->Init();
		pEngineResourceList->Init();

		// D3D 상태 초기화
		m_pD3D->SetInitRenderStates();

		DeviceLost = false;
	}
#endif
}