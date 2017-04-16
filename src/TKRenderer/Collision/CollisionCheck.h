#ifndef __COLLISIONCHECK_H__
#define __COLLISIONCHECK_H__

#include "../TKCommon.h"

//OBB 구조체
struct ST_OBB {
	D3DXVECTOR3	m_vCenterPos;	//OBB의 중심
	D3DXVECTOR3 m_vAxisDir[3];	//OBB의 축방향 3개 (단위벡터)
	float		m_fAxisLen[3];	//OBB의 축길이 3개 (축방향과 일대일 대응)
};

//구 구조체
struct ST_Sphere {
	D3DXVECTOR3	m_vCenter;		//구의 중심점
	float		m_fRadius;		//구의 반지름
};

//삼각형 구조체.. 시계방향 순서여야 한다.
struct ST_Face {
	D3DXVECTOR3	m_vDot1;		//꼭지점1
	D3DXVECTOR3	m_vDot2;		//꼭지점2
	D3DXVECTOR3	m_vDot3;		//꼭지점3
	D3DXVECTOR3 m_vNor;			//삼각형의 노말벡터
};

//레이 구조체.. 방향은 정규화되어있어야 한다.
struct ST_Ray {
	D3DXVECTOR3 m_vStartPos;	//Ray의 시작점
	D3DXVECTOR3 m_vDirection;	//Ray의 방향 (단위벡터)
	float		m_fLength;		//Ray의 길이
};


// 삼각형과 삼각형 충돌검사
int FaceFaceCollision( ST_Face* pFace1, ST_Face* pFace2 );

// 구와 OBB 충돌검사
int SphereOBBCollision( ST_Sphere* pSphere, ST_OBB* pOBB );

// 삼각형과 Box의 충돌 검사
int FaceOBBCollision( ST_Face* pFace, ST_OBB* pOBB );

// ray와 구의 충돌 검사
int RaySphereCollision( ST_Ray* pRay, ST_Sphere* pSphere );

// ray와 OBB 충돌검사
int RayOBBCollision( ST_Ray* pRay, ST_OBB* pOBB );

// ray와 삼각형 충돌검사
int RayFaceCollision( ST_Ray* pRay, ST_Face* pFace );

//구와 구의 충돌검사
int SphereSphereCollision( ST_Sphere* pSphere1, ST_Sphere* pSphere2 );

//OBB와 OBB의 충돌검사
int OBBOBBCollision( ST_OBB* pOBB1, ST_OBB* pOBB2 );

//구와 삼각형의 충돌검사.. 면과 정면에서 부딛히는 경우만 체크하므로 100% 정확하지는 않다.
//(측면까지 측정하려면 연산이 더 드는데, 그러지 않아도 왠만큼 근접하므로 굳이 연산을 할 필요는 없어보임)
int SphereFaceCollision( ST_Sphere* pSphere, ST_Face* pFace );

#endif