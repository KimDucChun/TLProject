/*
    엔진의 주 클래스.

    이 클래스에서는 3D에 대한 직접적 처리는 담당하지 않고, 다른 모듈을 구동시켜주거나,
    리소스에 대한 관리, 객체들에 대한 관리 등을 담당함.

    특징이라면 CPU처리와, GPU 처리를 동시에 진행한다는 것.
    CPU처리시에 GPU가 가지는 유휴시간 발생, 또는 그 반대 상황의 발생을 최소화 시킴으로서,
    최소 FPS 를 증가시켰다는데 의의를 둠.
*/

#pragma once

#include "TKBaseObject.h"
#include "TKEngine2.h"

#if 0

// 전방선언
class TKD3D;
class TKInput;
class TKPicker;
class TKIMEInput;
class TKTime;
class TKCamera;
class TKTexture;
class TKTextureManager;
class TKDebug;
class TKSkyBox;
class TKFrustum;
class TKEngineResource;

class TKDebugRes;
class TKSpriteRes;
class TKBillBoardRes;
class TKCustomUIRes;
class TKCollisionRenderRes;
class TKMeshRes;
class TKAniMeshRes;
class TKCollisionObjectRes;
class TKMeshShaderEffect;

class TKThread;
class TKCriticalSection;

class TKTR_Frame;
class TKTR_Render;
class TKTR_DataSync_0;
class TKTR_DataSync_1;
class TKTR_ApplyContainer_0;
class TKTR_ApplyContainer_1;

class TKBaseObjectList;
class TKEngineObject;
class TKEngineObjectList;
class TKSGObjectList;
class TKRenderQueue;
class TKSceneGraphManager;
class TKCustomScene;
class TKSceneManager;
class TKCollisionChecker;

class TKLight;

class TKRenderFuncList;
class TKRenderFunc;
class TKGeneralRender;
class TKDepthBufferShadow;

enum ECollisionObjectType;

class TKEngine : public TKBaseObject
{
    friend class TKTR_Render;
    friend class TKTR_Frame;
    friend class TKTR_DataSync_0;
    friend class TKTR_DataSync_1;
    friend class TKTR_ApplyContainer_0;
    friend class TKTR_ApplyContainer_1;
private:
#ifdef DEBUG_MODE
    char                DebugStr[100];
#endif DEBUG_MODE

    string              EngineRootDir;

	XMMATRIX			matIdentity;            // 단위매트릭스
    int                 m_TotalVertexCount;       // 렌더링되는 토탈 버텍스 카운트
    HWND                hWnd;                   // 현재 윈도 핸들
    HINSTANCE           hInstance;              // 현재 윈도 인스턴스

    // 이하 기본 객체
    TKD3D               *m_pD3D;                  // D3D 디바이스 객체
    TKInput             *pInput;                // Input 객체
    TKPicker            *pPicker;               // 마우스 픽킹 객체
    TKIMEInput          *pIMEInput;             // 한글입력을 위한 IME input 객체
    TKTime              *pTime;                 // 타이머 객체
    bool (*pExternFrame)(void *pObject);        // 외부 오브젝트의 프레임 함수 포인터
    void                *pExternFrameObject;    // 외부 오브젝트의 프레임 객체 포인터

    TKSceneManager      *pSceneManager;         // 장면 관리자 객체
    TKCollisionChecker  *pCollisionChecker;     // 충돌 검사객체

    TKDebug             *pDebug;                // 디버그 객체
    TKSkyBox            *pSkyBox;               // 스카이박스 객체
    TKFrustum           *pFrustum;              // 장면 컬링용 프러스텀 객체
    void                *pFocusedControl;       // UI 제어용 포커스 객체

    TKDebugRes          *pDebugRes;             // 디버그용 리소스
    TKSpriteRes         *pSpriteRes;            // 스프라이트 렌더링용 버텍스리소스
    TKBillBoardRes      *pBillBoardRes;         // 빌보드 렌더링용 버텍스리소스
    TKCustomUIRes       *pUIRes;                // UI 렌더링용 리소스
    TKCollisionRenderRes *pCollisionRenderRes;  // Collision 영역 렌더링용 리소스
    TKCollisionObjectRes *pDummyCollRes;        // Dummy충돌리소스
    
    TKLight             *pLightGlobal;          // 전역 확산광/환경광/집중광.

    // 이하 객체 리스트/관리자
    TKTextureManager    *pTextures;                 // 텍스쳐 관리자
    TKBaseObjectList    *pEngineResourceList;       // 엔진 리소스 관리자

    TKEngineObjectList  *pCameraList;               // 카메라 객체 관리자
    TKEngineObjectList  *pLightList;                // 광원 리스트
    TKSGObjectList      *pSGObjectList;             // 씬그래프 일반객체 리스트(이 리스트가 SceneGraph에 연동된다.)
    TKEngineObjectList  *p3DObjectList;             // 3D객체 리스트
    TKEngineObjectList  *p2DObjectList;             // 2D객체 리스트
    TKEngineObjectList  *pCollisionObjectList;      // 충돌 객체 관리자
    TKRenderQueue       *pRenderQueue;              // 렌더링 큐
    TKSceneGraphManager *pSceneGraphManager;        // 씬 그래프 관리자
    TKRenderFuncList    *m_pRenderFuncList;           // RenderFunc List
    
    // TKRenderFunc 객체    
    TKGeneralRender     *pGeneralRender;            // 일반 렌더 객체
    TKDepthBufferShadow *pDepthBufferShadow;        // 깊이버퍼그림자 렌더 객체
    
    // 디바이스 복구 관련
    bool                DeviceLost;
    void                CheckDeviceLost(void);      // 디바이스가 파괴되었을때 디바이스 복귀 함수

    // 스레드 관련
    bool                ThreadExit;
    bool                ThreadFirstStep;

    TKCriticalSection   CSProcess;
    
    TKTR_Render             *pTR_Render;                // 렌더링 스레드
    TKTR_Frame              *pTR_Frame;                 // 프레임 스레드
    HANDLE                  hFrameRenderStart[2];
    HANDLE                  hFrameRenderFinish[2];

    TKTR_DataSync_0         *pTR_DataSync_0;            // 렌더링과 프레임 데이터 동기화 스레드
    TKTR_DataSync_1         *pTR_DataSync_1;            // 렌더링과 프레임 데이터 동기화 스레드
    HANDLE                  hDataSyncStartEvent[2];
    HANDLE                  hDataSyncFinishEvent[2];

    TKTR_ApplyContainer_0   *pTR_ApplyContainer_0;      // 렌더링과 프레임 데이터 동기화 스레드
    TKTR_ApplyContainer_1   *pTR_ApplyContainer_1;      // 렌더링과 프레임 데이터 동기화 스레드
    HANDLE                  hApplyContainerStartEvent[2];
    HANDLE                  hApplyContainerFinishEvent[2];
    
    // 그림자
    EEngineShadowType       EngineShadowType;           // 그림자 타입
    
    // 이하 Fog관련
    BOOL                bFogEnable;
    float               fFogNear;
    float               fFogFar;
	XMFLOAT4			vecFogColor;
	XMFLOAT4			vecFogVector;
    
    // D3DDEVICE 파라메터
    DWORD               D3DFILLMODE;    // D3DRS_FILLMODE
    float               NearDistance;   // NEAR PLANE DISTANCE
    float               FarDistance;    // FAR PLANE DISTANCE
    
    // 그외 환경변수
    string              ModelDir;       // Model Directroy;
    string              FXDir;          // FXFile Directroy;
    bool                CollAreaVisible;// CollisionArea Visible;
    bool                BoneVisible;    // Bone Visible;
    bool                MeshVisible;    // Mesh Visible;
public:

    TKEngine(HINSTANCE hInstance, HWND hWnd);
    virtual ~TKEngine(void);

    TKD3D * GetD3D(void);
#if defined(TK_D3D9)
	LPDIRECT3DDEVICE9 GetD3DDevice(void);
#elif defined(TK_D3D11)
	ID3D11DeviceContext * GetD3DContext(void);
#endif

    HWND GethWnd(void);
    HINSTANCE GethInstance(void);
    TKInput * GetInput(void);
    TKPicker * GetPicker(void);
    TKIMEInput * GetIMEInput(void);
    TKTime * GetTime(void);
    TKFrustum * GetFrustum(void);
    TKDebug * GetDebug(void);

    TKSkyBox * GetSkyBox(void);
    void SetSkyBox(TKSkyBox *pSkyBox);

    void * GetFocusedControl(void);
    void SetFocusedControl(void * pFocusedControl);
    
    XMMATRIX * GetViewMatrix(void);
	XMMATRIX * GetProjectionMatrix(void);
    
    template <class SHADEREFFECT> SHADEREFFECT * ShaderLoad( string FXFileName, string TechniqueName);
    TKAniMeshRes * AniMeshResLoad(TKMeshShaderEffect * pMeshShaderEffect, string  MeshFileName, string  AniFileName, string  MotionFile);
    TKCollisionObjectRes * CollResLoad( string  CollFileName, ECollisionObjectType CollObjectType, bool IsDynamicObject, bool IsMeshAnimation, TKMeshRes * pBindMeshRes, string  BindNodeName);

    void Init(void);            // 엔진 초기화
    void Frame(void);           // Frame 처리
    void Render(void);          // Render 처리
    void Release(void);         // 엔진 해제

    void Lock(void);
    void UnLock(void);

    void ApplyContainer_T0(void);   // 객체 컨테이너 갱신 함수1
    void ApplyContainer_T1(void);   // 객체 컨테이너 갱신 함수2
    void DataSync_T0(void);         // 데이터 동기화 함수1
    void DataSync_T1(void);         // 데이터 동기화 함수2

    void SetExternFrameFunc(bool (*pFunc)(void *pObject));   // 외부 프레임 함수 정의
    void SetExternFrameObject(void *pExternFrameObject);     // 외부 프레임 객체 정의
    
    TKBaseObjectList * GetEngineResourceList(void);
    TKEngineObjectList * GetCameraList(void);
    TKEngineObjectList * GetLightList(void);
    TKSGObjectList * GetSGObjectList(void);
    TKEngineObjectList * Get3DObjectList(void);
    TKEngineObjectList * Get2DObjectList(void);    
    TKRenderQueue * GetRenderQueue(void);
    TKSceneGraphManager * GetSceneGraphManager(void);    
    TKRenderFuncList * GetRenderFuncList(void);

    TKGeneralRender * GetGeneralRender(void);
    TKDepthBufferShadow * GetDepthBufferShadow(void);
    
    void ClearDeleteAllEngineItem(void);

    void SetCamera(TKCamera *pCamera);
    TKCamera * GetCamera(void);

    TKTextureManager * GetTextures(void);
    TKTexture * GetTexture(const int Index);

    int GetTotalVertexCount(void);
	XMMATRIX * GetMatrixIdentity(void);

    TKSceneManager * GetSceneManager(void);
    TKCollisionChecker * GetCollisionChecker(void);

    void ShowDebugMessage(string String, int x, int y);

    TKDebugRes * GetDebugRes(void);
    TKSpriteRes * GetSpriteRes(void);
    TKBillBoardRes * GetBillBoardRes(void);
    TKCustomUIRes * GetUIRes(void);
    TKCollisionRenderRes * GetCollisionRenderRes(void);
    TKCollisionObjectRes * GetDummyCollRes(void);
    TKLight * GetLightGlobal(void);

    bool GetDeviceLost(void); 
    void SetDeviceLost(bool DeviceLost);

    string & GetEngineRootDir(void);
    void SetEngineRootDir(string & RootDir);
    string & GetSystemTextureDir(void);
    string & GetUserTextureDir(void);
    void SetUserTextureDir(string & TextureDir);
    string & GetModelDir(void);
    void SetModelDir(string & ModelDir);
    string & GetFXDir(void);
    void SetFXDir(string & FXDir);
    
    static bool CheckEngineRootValidate(string & EngineRootDir,
                                        string & UserTextureDir,
                                        string & ModelDir,
                                        string & FXDir );
    
    EEngineShadowType GetShadowType(void);
    void SetShadowType(EEngineShadowType EngineShadowType);

    BOOL GetFogEnable(void);
    void SetFogEnable(BOOL FogEnable);
    float GetFogNear(void);
    void SetFogNear(float fFogNear);
    float GetFogFar(void);
    void SetFogFar(float fFogFar);
    XMFLOAT4 & GetFogColor(void);
    void SetFogColor(XMFLOAT4 & FogColor);
	XMFLOAT4 & GetFogVector(void);
    
    DWORD GetFillMode(void);
    void SetFillMode(DWORD FILLMODE);
    float GetNearDistance(void);
    void SetNearDistance(float NearDistance);
    float GetFarDistance(void);
    void SetFarDistance(float FarDistance);
    
    bool GetFrameShow(void);
    void SetFrameShow(bool Value);
    bool GetPosShow(void);
    void SetPosShow(bool Value);
    bool GetAxisShow(void);
    void SetAxisShow(bool Value);
    bool GetCollAreaVisible(void);
    void SetCollAreaVisible(bool Visible);
    bool GetBoneVisible(void);
    void SetBoneVisible(bool Visible);
    bool GetMeshVisible(void);
    void SetMeshVisible(bool Visible);
    
    // 매 틱당 Frame, Render 등의 처리를 호출
    bool DoProcess(void);
};

#define R_D3DDEVICE                     GetEngine()->GetD3DDevice()
#define R_D3D                           GetEngine()->GetD3D()
#define R_KEYDOWN(key)                  GetEngine()->GetInput()->KeyDown(key)
#define R_KEYUP(key)                    GetEngine()->GetInput()->KeyUp(key)
#define R_KEYPRESSED(key)               GetEngine()->GetInput()->KeyPressed(key)
#define R_MOUSEBUTTONDOWN(Index)        GetEngine()->GetInput()->MouseButtonDown(Index)
#define R_MOUSEBUTTONUP(Index)          GetEngine()->GetInput()->MouseButtonUp(Index)
#define R_MOUSEBUTTONPRESSED(Index)     GetEngine()->GetInput()->MouseButtonPressed(Index)
#define R_DIMOUSEINFO                   GetEngine()->GetInput()->GetDIMouseInfo()
#define R_APIMOUSEINFO                  GetEngine()->GetInput()->GetAPIMouseInfo()
#define R_APIMOUSEMOVE                  GetEngine()->GetInput()->GetAPIMouseMove()
#define R_TIMEDELTA                     GetEngine()->GetTime()->GetTimeDelta()
#define R_GETFPS                        GetEngine()->GetTime()->GetFPS()
#define R_SHOWDEBUGMSG(pString, x, y)   GetEngine()->ShowDebugMessage(pString, x, y)
#define R_CAMERAMANAGER                 GetEngine()->GetCameraList()
#define R_ENGINERESOURCEMANAGER         GetEngine()->GetEngineResourceList()
#define R_LIGHTLIST                     GetEngine()->GetLightList()
#define R_SGOBJECTMANAGER               GetEngine()->GetSGObjectList()
#define R_3DOBJECTMANAGER               GetEngine()->Get3DObjectList()
#define R_2DOBJECTMANAGER               GetEngine()->Get2DObjectList()
#define R_TEXTUREMANAGER                GetEngine()->GetTextures()
#define R_EFFECTCONTAINER               GetEngine()->GetEffectContainer()
#define R_RENDERQUEUE                   GetEngine()->GetRenderQueue()
#define R_SCENEGRAPHMANAGER             GetEngine()->GetSceneGraphManager()
#define R_RENDERFUNCLIST                GetEngine()->GetRenderFuncList()
#define R_GENERALRENDER                 GetEngine()->GetGeneralRender()
#define R_DEPTHBUFFERSHADOW             GetEngine()->GetDepthBufferShadow()
#define R_FRUSTUM                       GetEngine()->GetFrustum()
#define R_SKYBOX                        GetEngine()->GetSkyBox()
#define R_DEBUG                         GetEngine()->GetDebug()
#define R_PICKER                        GetEngine()->GetPicker()
#define R_COLLISIONCHECKER              GetEngine()->GetCollisionChecker()
#define R_LIGHTGLOBAL                   GetEngine()->GetLightGlobal()

/////////////////////////////////////////////////////////////////////////

#include "./TKEngine.hpp"

#endif