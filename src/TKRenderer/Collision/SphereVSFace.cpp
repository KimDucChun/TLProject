#include "CollisionCheck.h"

int SphereFaceCollision( ST_Sphere* pSphere, ST_Face* pFace )
{
	D3DXVECTOR3* pvNor = &pFace->m_vNor;
	D3DXVECTOR3 vDiff = pSphere->m_vCenter - pFace->m_vDot1;
	float fRadius = pSphere->m_fRadius;

	float fDistance = D3DXVec3Dot( &vDiff, pvNor );

	if( fDistance > fRadius || fDistance < -fRadius ) return 0;

    D3DXVECTOR3 vEdge1 = pFace->m_vDot1 - pFace->m_vDot3;
	D3DXVECTOR3 vEdge2 = pFace->m_vDot2 - pFace->m_vDot1;
	D3DXVECTOR3 vEdge3 = pFace->m_vDot3 - pFace->m_vDot2;

	D3DXVECTOR3 vEdgePlane1;
	D3DXVec3Cross( &vEdgePlane1, pvNor, &vEdge1 );
	D3DXVec3Normalize( &vEdgePlane1, &vEdgePlane1 );

	fDistance = D3DXVec3Dot( &vEdgePlane1, &vDiff );
	if( fDistance < 0 ) return 0;

	D3DXVECTOR3 vEdgePlane2;
	D3DXVec3Cross( &vEdgePlane2, pvNor, &vEdge2 );
	D3DXVec3Normalize( &vEdgePlane2, &vEdgePlane2 );

	vDiff = pSphere->m_vCenter - pFace->m_vDot2;
	fDistance = D3DXVec3Dot( &vEdgePlane2, &vDiff );
	if( fDistance < 0 ) return 0;

	D3DXVECTOR3 vEdgePlane3;
	D3DXVec3Cross( &vEdgePlane3, pvNor, &vEdge3 );
	D3DXVec3Normalize( &vEdgePlane3, &vEdgePlane3 );

	vDiff = pSphere->m_vCenter - pFace->m_vDot3;
	fDistance = D3DXVec3Dot( &vEdgePlane3, &vDiff );
	if( fDistance < 0 ) return 0;

	return 1;
}