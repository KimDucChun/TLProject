#pragma once

#pragma warning(disable:4996) 
#pragma warning(disable:4005)

//#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

#define TK_D3D11
//#define TK_D3D9

#if defined(TK_D3D9)
	#pragma comment (lib, "d3dx9.lib")
	#pragma comment (lib, "d3dxof.lib")
	#pragma comment (lib, "d3dx9d.lib")
	#pragma comment (lib, "d3d9.lib"  )
#elif defined(TK_D3D11)
	#pragma comment (lib, "d3d11.lib")
	#pragma comment (lib, "dxgi.lib")
#if 0
//	#pragma comment (lib, "d3dx11.lib")
//	#pragma comment (lib, "d3dx10.lib")
#endif
#endif

#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib" )
#pragma comment (lib, "ws2_32.lib" )
//#pragma comment (lib, "strmbasd.lib")

#define DEBUG_MODE
//#define WIREFRAME_MODE
//#define SHADER_DEBUGMODE

#define MASKCOLOR 0xFF000000

#define DIRECTINPUT_VERSION     0x0800
#define SBUFSIZE 100
#define TINYSBUFSIZE 30

#define WINDOW_MODE
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SHADOW_MAP_SIZE 512

#define FREEKEY	0x10
#define PULLKEY	0x20
#define PUSHKEY	0x40
#define HOLDKEY	0x80

#ifdef DEBUG_MODE
    #define CRTDBG_MAP_ALLOC #include <stdlib.h> #include <crtdbg.h>
    #include <malloc.h>
#endif DEBUG_MODE

#include <windows.h>
#include <crtdbg.h>
#include <atlstr.h>
#include <ASSERT.h>
#include <stdio.h>
//#include <d3dxmath.h>
#include <tchar.h>

#if defined(TK_D3D9)
	#include <d3dx9.h>
	#include <D3d9.h>
#elif defined(TK_D3D11)
	#include <dxgi.h>
	#include <d3dcommon.h>
	#include <d3d11.h>
	#include <DirectXMath.h>
	
	//#include <xnamath.h>
#endif

#include <d3d9types.h>
#include <dinput.h>
#include <process.h>
#include <mmsystem.h>
#include <winuser.h>
#include <string.h>

#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <list>
#include <algorithm>
#include <string>

//#include "./TKContainer.h"

//#include "./Mathmatics/Bounding.h"
//#include "./Mathmatics/Eigen.h"
//#include "./Mathmatics/Vector3.h"
//#include "./Collision/CollisionCheck.h"

//using namespace KContainer;
using namespace std;
using namespace DirectX;

#ifdef SHADER_DEBUGMODE
    #define ENGINE_DEVICETYPE       D3DDEVTYPE_REF
    #define ENGINE_BEHAVIORFLAG     D3DCREATE_MULTITHREADED | D3DCREATE_SOFTWARE_VERTEXPROCESSING
    #define ENGINE_SHADERFLAG       D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION | D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT | D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT
#else
    #define ENGINE_DEVICETYPE       D3DDEVTYPE_HAL
    #define ENGINE_BEHAVIORFLAG     D3DCREATE_MULTITHREADED | D3DCREATE_HARDWARE_VERTEXPROCESSING
    //#define ENGINE_BEHAVIORFLAG     D3DCREATE_MULTITHREADED | D3DCREATE_MIXED_VERTEXPROCESSING
    #define ENGINE_SHADERFLAG       0
#endif

enum ETKObjectType
{
    eotNULL,
    eotObject,
    eotBaseObject,
    eotBaseObjectList,
    eotCustomParser,
    
    // 씬 그래프 관련
    eotSceneGraphManager,
    eotSceneGraph,
    eotSceneGraphNode,
    eotSGNodeContainer,
    eotOcTree,
    eotOcTreeNode,
    
    // 리소스
    eotVertexResource,
    eotIndexResource,
    
    // 메쉬리소스
    eotMeshShaderEffect,
    eotDepthShadowShaderEffect,
    eotMeshMaterial,
    eotMeshVertexes,
    eotMeshFaces,
    eotMeshGeometry,
    eotMeshRes,
    eotAniMeshRes,
    eotTerrainRes,
    
    //이펙트 관련
    eotParticleEmitter,
	eotFSParticleEmitter,
	eotRSParticleEmitter,
    eotBillboardParticleEmitter,
	eotFSBillboardParticleEmitter,
	eotRSBillboardParticleEmitter,
    eotPointParticleEmitter,
	eotFSPointParticleEmitter,
	eotRSPointParticleEmitter,
    eotMeshParticleEmitter,
	eotFSMeshParticleEmitter,
	eotRSMeshParticleEmitter,
	eotEffect,
    eotFSEffect,
	eotRSEffect,
    eotParticleMesh,
	eotFSParticleMesh,
	eotRSParticleMesh,
	//이펙트 관련 끝

    // 충돌관련(KMF 데이터와도 동일한 값을 사용하므로 상수값 바꾸면 안됨)
    eotCollisionChecker = 6000,
    eotCollisionResourceList = 6001,
    eotCollisionResource = 6002,
    eotFaceCollision = 6003,
    eotSphereCollision = 6004,
    eotOBBCollision = 6005,
    eotRayCollision = 6006,
    eotCollisionRenderRes = 6007,
    eotCollisionObjectRes = 6008,
    eotCollisionRSObject = 6009,
    eotCollisionFSObject = 6010,
    eotCollisionObject = 6011,
    // 충돌관련 끝

    // 카메라
    eotDebugCamera,
    eotStaticCamera,
    eotSpringCamera,
    // 카메라 끝

    // 메쉬
    eotMesh,
    eotAniMesh,
    eotDefaultMesh,
    eotLODMesh,
    eotCharacter,
    eotBoosterMesh,
    eotStaticMesh
    // 메쉬끝
};

enum ERenderFuncType
{
    erftNONE = 0,
    erftGENERAL = 1,
    erftDEPTHBUFFERSHADOW = 2
};

enum EEngineShadowType
{
    eestNONE = 0,
    eestApproximate = 1,
    eestPlaneProjected = 2,
    eestDepthBufferProjected = 3
};

#define MATERIAL_TEXTURE_COUNT 12
enum EMaterialTextureIndex
{
    emtAmbient = 0,
    emtDiffuse = 1,
    emtSpecular = 2,
    emtShine = 3,
    emtShineStrength = 4,
    emtSelfIllum = 5,
    emtOpacity = 6,
    emtFilterColor = 7,
    emtBump = 8,
    emtReflect = 9,
    emtRefract = 10,
    emtGeneral = 11
};

enum EMeshVertexesType
{
    emvtMESH = 1,
    emvtBONE = 2
};

enum ECollisionObjectType
{
    ecotNULL,
    ecotFace,
    ecotOBB,
    ecotSphere,
    ecotRay,
    ecotSphereFace,
    ecotSphereOBB,
    ecotSphereSphere,
    ecotSphereRay,
    ecotSphereSphereFace,
    ecotSphereSphereOBB,
    ecotSphereSphereSphere,
    ecotSphereSphereRay
};

enum ECollisionMeshSourceType
{
    ecmstNULL,
    ecmstGeom,  // 오리진 컬리젼 데이터를 만들시, TKMeshRes 의 GeomList 를 참조한다
    ecmstBone   // 오리진 컬리젼 데이터를 만들시, TKMeshRes 의 BoneList 를 참조한다
};

enum ETerrainPatchDirection
{
    etpdTop     = 1 << 0,
    etpdDown    = 1 << 1,
    etpdLeft    = 1 << 2,
    etpdRight   = 1 << 3,
};

enum ETerrainPatchType
{
// 1.전체	2.상	3.하	4.좌	5.우	6.상하	
// 7.좌우	8.상우	9.우하	10.하좌	11.좌상	12.좌하우	13.상하좌	14.좌상우	15.상우하
    etptAll             = 0,
    etptTop             = etpdTop,
    etptDown            = etpdDown,
    etptLeft            = etpdLeft,
    etptRight           = etpdRight,
    etptTopDown         = etpdTop | etpdDown,
    etptLeftRight       = etpdLeft | etpdRight,
    etptTopRight        = etpdTop | etpdRight,
    etptRightDown       = etpdRight | etpdDown,
    etptDownLeft        = etpdDown | etpdLeft,
    etptLeftTop         = etpdLeft | etpdTop,
    etptLeftDownRight   = etpdLeft | etpdDown | etpdRight,
    etptTopDownRight    = etpdTop | etpdDown | etpdRight,
    etptLeftTopDown     = etpdLeft | etpdTop | etpdDown,
    etptTopRightDown    = etpdTop | etpdRight | etpdDown,
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//INT_VECTOR3
///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class _Unknown_Vector3
{
public:
	T x;
	T y;
	T z;

	_Unknown_Vector3(void)
	{
		:x(0), y(0), z(0)
	}
	_Unknown_Vector3(_Unknown_Vector3<T> & AData)
	{
		(*this) = AData;
	}
	_Unknown_Vector3(T x, T y, T z)
	{
		:x(x), y(y), z(z)
	}
	~_Unknown_Vector3(void) {}

	bool operator == (_Unknown_Vector3<T> & AData)
	{
		return  (this->x == AData.x) && (this->y == AData.y) && (this->z == AData.z);
	}
	bool operator != (_Unknown_Vector3<T> & AData)
	{
		return  !(*this == AData);
	}
};

typedef _Unknown_Vector3<int> INT_VECTOR3;
typedef vector<INT_VECTOR3> INT_VECTOR3S;

typedef _Unknown_Vector3<float> FLOAT_VECTOR3;
typedef vector<FLOAT_VECTOR3> FLOAT_VECTOR3S;

//class INT_VECTOR3
//{
//public:
//	int x;
//	int y;
//	int z;
//
//	INT_VECTOR3(void);
//	INT_VECTOR3(INT_VECTOR3 & AData);
//	INT_VECTOR3(int x, int y, int z);
//	~INT_VECTOR3(void);
//
//	bool operator == (INT_VECTOR3 & AData);
//	bool operator != (INT_VECTOR3 & AData);
//};
//typedef vector<INT_VECTOR3> INT_VECTOR3S;
//
//class FLOAT_VECTOR3
//{
//public:
//	int x;
//	int y;
//	int z;
//
//	FLOAT_VECTOR3(void);
//	FLOAT_VECTOR3(FLOAT_VECTOR3 & AData);
//	FLOAT_VECTOR3(int x, int y, int z);
//	~FLOAT_VECTOR3(void);
//
//	bool operator == (FLOAT_VECTOR3 & AData);
//	bool operator != (FLOAT_VECTOR3 & AData);
//};
//typedef vector<FLOAT_VECTOR3> FLOAT_VECTOR3S;

///////////////////////////////////////////////////////////////////////////////////////////////////
// 객체 인스턴스 관리 템플릿
///////////////////////////////////////////////////////////////////////////////////////////////////
// safe new
template <typename T>
inline void SAFENEW(T*& ptr)
{
	if(!ptr)
	{
		ptr = new T;
	}
}

// cleared safe new
template <typename T>
inline void NEWCLEAR(T*& ptr)
{
	if(!ptr)
	{
		ptr = new T;	
		_ASSERT(ptr);
		::memset(ptr, 0, sizeof(T));
	}
}

// safe delete
template <typename T>
inline void SAFEDELETE(T*& ptr)
{
	if(ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
};

// safe Release And Delete
template <typename T>
inline void SAFERELEASEDELETE(T*& ptr)
{
	if(ptr)
	{
		ptr->Release();
        delete ptr;
		ptr = nullptr;
	}
};

// safe new array
template <typename T>
inline void SAFEARRAYNEW(T*& ptr, INT i)
{
	if(!ptr)
	{
		ptr = new T[i];
	}
}

// cleared safe new array
template <typename T>
inline void NEWARRAYCLEAR(T*& ptr, INT i)
{
	if(!ptr)
	{
		ptr = new T[i];
		_ASSERT(ptr);
		::memset(ptr, 0, sizeof(T) * i);
	}
}

// safe delete array
template <typename T>
inline void SAFEARRAYDELETE(T*& ptr)
{
	if(ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

// safe zero
template <typename T>
inline void SAFEZERO(T& var)
{
	var = 0;
}

template <typename T>
inline void SAFEZERO(T*& ptr)	// 포인터 특화버전
{
	ptr = nullptr;
}

// safe release
template <typename T>
inline void SAFERELEASE(T*& ptr)
{
	if(ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

// 단순 메모리 복사 함수
template <class T> void SAMETYPEMEMCPY(T *pDest, T *pSrc)
{
    memcpy(pDest, pSrc, sizeof(T));
}

// 단순 포인터 SWAP 함수
template <class T> void SAMETYPESWAP(T* &pData0, T* &pData1)
{
    T* pDataTemp;
    pDataTemp = pData0;
    pData0 = pData1;
    pData1 = pDataTemp;
}

// safe handle close
template <typename T>
inline void SAFEHANDLECLOSE(T& var)
{
	if (var)
	{
		CloseHandle(var);
		var = nullptr;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// ID 생성기
///////////////////////////////////////////////////////////////////////////////////////////////////
class TKIDGenerator
{
    int LastID;
    CRITICAL_SECTION    cs;
    TKIDGenerator(void);
    ~TKIDGenerator(void);
public:
    static TKIDGenerator & GetInstance(void);
    int GenID(void);
    string GenIDStr(void);
    string GenIDStr(string PreFix = "", string PostFix = "");
};
#define GENID TKIDGenerator::GetInstance().GenID()
#define GENIDSTR(a) TKIDGenerator::GetInstance().GenIDStr(a)

///////////////////////////////////////////////////////////////////////////////////////////////////
// 실행비용체크
///////////////////////////////////////////////////////////////////////////////////////////////////
class TKCheckExpenses
{
private:
    LARGE_INTEGER tStart, tEnd, tResult;
    CRITICAL_SECTION    cs;
    TKCheckExpenses(void);
    ~TKCheckExpenses(void);
public:
    static TKCheckExpenses & CurrInstance(void);
    void ExpensesCheckStart(void);
    void ExpensesCheckEnd(void);
    void OutputDebugExpenses(char *pString = nullptr);
};
#define EXPENSESCHECKSTART TKCheckExpenses::CurrInstance().ExpensesCheckStart()
#define EXPENSESCHECKEND TKCheckExpenses::CurrInstance().ExpensesCheckEnd()
#define EXPENSESOUTPUT(pString) TKCheckExpenses::CurrInstance().OutputDebugExpenses(pString)

///////////////////////////////////////////////////////////////////////////////////////////////////
//CMinMax
///////////////////////////////////////////////////////////////////////////////////////////////////
//	for Getting Random Value
template <class T> class CMinMax
{
public:
	CMinMax() { m_Min = m_Max = 0; }
	CMinMax(T tMin, T tMax) { m_Min = tMin; m_Max = tMax; }
	~CMinMax() { }

	T m_Min;
	T m_Max;

	T GetRandomNumInRange(void) { return(RandomNumber(m_Min, m_Max)); }
};

/****************************************************************************
RandomNumber: returns a random number between iMin and iMax.
****************************************************************************/
inline int RandomNumber(int iMin, int iMax)
{
	if (iMin == iMax) return(iMin);
	return((rand() % (abs(iMax-iMin)+1))+iMin);
}

inline float RandomNumber(float fMin, float fMax)
{
	if (fMin == fMax) return(fMin);
	float fRandom = (float)rand() / (float)RAND_MAX;
	return((fRandom * (float)fabs(fMax-fMin))+fMin);
}


// float to DWORD
inline DWORD FtoDw(float f) {
	return *((DWORD*)&f);
}

inline float ATOF(char *pStr)
{
    return ((float)(atof(pStr)));
}

//파일이나 폴더 존재 유무 오픈하지 않고 체크하기
inline bool IsFileExist(const char * strFile)
{
    DWORD fileAttr;

    fileAttr = GetFileAttributes(strFile);
    if (INVALID_FILE_ATTRIBUTES == fileAttr) return FALSE;
    return TRUE;
}

// 문자치환 함수
void StringReplace(char *pSrc, const char* pFindStr, const char* pReplaceStr);
// 전체 파일명에서 파일명만 가져오기
void ExtractFileNameWithExt( const char *pSrc, char *pResult );
// 전체 파일명에서 확장자만 가져오기
void ExtractFileExt( const char *pSrc, char *pResult );
// 전체 파일명에서 경로만 가져오기
void ExtractFileDir( const char *pSrc, char *pResult );

