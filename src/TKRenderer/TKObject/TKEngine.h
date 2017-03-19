/*
    ������ �� Ŭ����.

    �� Ŭ���������� 3D�� ���� ������ ó���� ������� �ʰ�, �ٸ� ����� ���������ְų�,
    ���ҽ��� ���� ����, ��ü�鿡 ���� ���� ���� �����.

    Ư¡�̶�� CPUó����, GPU ó���� ���ÿ� �����Ѵٴ� ��.
    CPUó���ÿ� GPU�� ������ ���޽ð� �߻�, �Ǵ� �� �ݴ� ��Ȳ�� �߻��� �ּ�ȭ ��Ŵ���μ�,
    �ּ� FPS �� �������״ٴµ� ���Ǹ� ��.
*/

#pragma once

#include "TKBaseObject.h"
#include "TKEngine2.h"

#if 0

// ���漱��
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

	XMMATRIX			matIdentity;            // ������Ʈ����
    int                 m_TotalVertexCount;       // �������Ǵ� ��Ż ���ؽ� ī��Ʈ
    HWND                hWnd;                   // ���� ���� �ڵ�
    HINSTANCE           hInstance;              // ���� ���� �ν��Ͻ�

    // ���� �⺻ ��ü
    TKD3D               *m_pD3D;                  // D3D ����̽� ��ü
    TKInput             *pInput;                // Input ��ü
    TKPicker            *pPicker;               // ���콺 ��ŷ ��ü
    TKIMEInput          *pIMEInput;             // �ѱ��Է��� ���� IME input ��ü
    TKTime              *pTime;                 // Ÿ�̸� ��ü
    bool (*pExternFrame)(void *pObject);        // �ܺ� ������Ʈ�� ������ �Լ� ������
    void                *pExternFrameObject;    // �ܺ� ������Ʈ�� ������ ��ü ������

    TKSceneManager      *pSceneManager;         // ��� ������ ��ü
    TKCollisionChecker  *pCollisionChecker;     // �浹 �˻簴ü

    TKDebug             *pDebug;                // ����� ��ü
    TKSkyBox            *pSkyBox;               // ��ī�̹ڽ� ��ü
    TKFrustum           *pFrustum;              // ��� �ø��� �������� ��ü
    void                *pFocusedControl;       // UI ����� ��Ŀ�� ��ü

    TKDebugRes          *pDebugRes;             // ����׿� ���ҽ�
    TKSpriteRes         *pSpriteRes;            // ��������Ʈ �������� ���ؽ����ҽ�
    TKBillBoardRes      *pBillBoardRes;         // ������ �������� ���ؽ����ҽ�
    TKCustomUIRes       *pUIRes;                // UI �������� ���ҽ�
    TKCollisionRenderRes *pCollisionRenderRes;  // Collision ���� �������� ���ҽ�
    TKCollisionObjectRes *pDummyCollRes;        // Dummy�浹���ҽ�
    
    TKLight             *pLightGlobal;          // ���� Ȯ�걤/ȯ�汤/���߱�.

    // ���� ��ü ����Ʈ/������
    TKTextureManager    *pTextures;                 // �ؽ��� ������
    TKBaseObjectList    *pEngineResourceList;       // ���� ���ҽ� ������

    TKEngineObjectList  *pCameraList;               // ī�޶� ��ü ������
    TKEngineObjectList  *pLightList;                // ���� ����Ʈ
    TKSGObjectList      *pSGObjectList;             // ���׷��� �Ϲݰ�ü ����Ʈ(�� ����Ʈ�� SceneGraph�� �����ȴ�.)
    TKEngineObjectList  *p3DObjectList;             // 3D��ü ����Ʈ
    TKEngineObjectList  *p2DObjectList;             // 2D��ü ����Ʈ
    TKEngineObjectList  *pCollisionObjectList;      // �浹 ��ü ������
    TKRenderQueue       *pRenderQueue;              // ������ ť
    TKSceneGraphManager *pSceneGraphManager;        // �� �׷��� ������
    TKRenderFuncList    *m_pRenderFuncList;           // RenderFunc List
    
    // TKRenderFunc ��ü    
    TKGeneralRender     *pGeneralRender;            // �Ϲ� ���� ��ü
    TKDepthBufferShadow *pDepthBufferShadow;        // ���̹��۱׸��� ���� ��ü
    
    // ����̽� ���� ����
    bool                DeviceLost;
    void                CheckDeviceLost(void);      // ����̽��� �ı��Ǿ����� ����̽� ���� �Լ�

    // ������ ����
    bool                ThreadExit;
    bool                ThreadFirstStep;

    TKCriticalSection   CSProcess;
    
    TKTR_Render             *pTR_Render;                // ������ ������
    TKTR_Frame              *pTR_Frame;                 // ������ ������
    HANDLE                  hFrameRenderStart[2];
    HANDLE                  hFrameRenderFinish[2];

    TKTR_DataSync_0         *pTR_DataSync_0;            // �������� ������ ������ ����ȭ ������
    TKTR_DataSync_1         *pTR_DataSync_1;            // �������� ������ ������ ����ȭ ������
    HANDLE                  hDataSyncStartEvent[2];
    HANDLE                  hDataSyncFinishEvent[2];

    TKTR_ApplyContainer_0   *pTR_ApplyContainer_0;      // �������� ������ ������ ����ȭ ������
    TKTR_ApplyContainer_1   *pTR_ApplyContainer_1;      // �������� ������ ������ ����ȭ ������
    HANDLE                  hApplyContainerStartEvent[2];
    HANDLE                  hApplyContainerFinishEvent[2];
    
    // �׸���
    EEngineShadowType       EngineShadowType;           // �׸��� Ÿ��
    
    // ���� Fog����
    BOOL                bFogEnable;
    float               fFogNear;
    float               fFogFar;
	XMFLOAT4			vecFogColor;
	XMFLOAT4			vecFogVector;
    
    // D3DDEVICE �Ķ����
    DWORD               D3DFILLMODE;    // D3DRS_FILLMODE
    float               NearDistance;   // NEAR PLANE DISTANCE
    float               FarDistance;    // FAR PLANE DISTANCE
    
    // �׿� ȯ�溯��
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

    void Init(void);            // ���� �ʱ�ȭ
    void Frame(void);           // Frame ó��
    void Render(void);          // Render ó��
    void Release(void);         // ���� ����

    void Lock(void);
    void UnLock(void);

    void ApplyContainer_T0(void);   // ��ü �����̳� ���� �Լ�1
    void ApplyContainer_T1(void);   // ��ü �����̳� ���� �Լ�2
    void DataSync_T0(void);         // ������ ����ȭ �Լ�1
    void DataSync_T1(void);         // ������ ����ȭ �Լ�2

    void SetExternFrameFunc(bool (*pFunc)(void *pObject));   // �ܺ� ������ �Լ� ����
    void SetExternFrameObject(void *pExternFrameObject);     // �ܺ� ������ ��ü ����
    
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
    
    // �� ƽ�� Frame, Render ���� ó���� ȣ��
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