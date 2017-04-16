#include "CollisionCheck.h"

int SphereOBBCollision( ST_Sphere* pSphere, ST_OBB* pOBB )
{
	float fDistanceToBox = 0.f;
	float fDiff;
	float fAxisLen[3];
	fAxisLen[0] = pOBB->m_fAxisLen[0];
	fAxisLen[1] = pOBB->m_fAxisLen[1];
	fAxisLen[2] = pOBB->m_fAxisLen[2];
	D3DXVECTOR3* pSphereCenter = &pSphere->m_vCenter;

	//OBB기준의 기저로 변환
	D3DXVECTOR3 vRelativeVec = (*pSphereCenter) - pOBB->m_vCenterPos;
	D3DXVECTOR3 vRelativeSphereCenter( D3DXVec3Dot( &pOBB->m_vAxisDir[0], &vRelativeVec ), D3DXVec3Dot( &pOBB->m_vAxisDir[1], &vRelativeVec ), D3DXVec3Dot( &pOBB->m_vAxisDir[2], &vRelativeVec ) );

	if( vRelativeSphereCenter.x < -fAxisLen[0])
	{
		fDiff = vRelativeSphereCenter.x + fAxisLen[0];
		fDistanceToBox += fDiff*fDiff;
	}
	else if( vRelativeSphereCenter.x > fAxisLen[0] )
	{
		fDiff = vRelativeSphereCenter.x - fAxisLen[0];
		fDistanceToBox += fDiff*fDiff;
	}
	if( vRelativeSphereCenter.y < -fAxisLen[1])
	{
		fDiff = vRelativeSphereCenter.y + fAxisLen[1];
		fDistanceToBox += fDiff*fDiff;
	}
	else if( vRelativeSphereCenter.y > fAxisLen[1] )
	{
		fDiff = vRelativeSphereCenter.y - fAxisLen[1];
		fDistanceToBox += fDiff*fDiff;
	}
	if( vRelativeSphereCenter.z < -fAxisLen[2])
	{
		fDiff = vRelativeSphereCenter.z + fAxisLen[2];
		fDistanceToBox += fDiff*fDiff;
	}
	else if( vRelativeSphereCenter.z > fAxisLen[2] )
	{
		fDiff = vRelativeSphereCenter.z - fAxisLen[2];
		fDistanceToBox += fDiff*fDiff;
	}

	if( fDistanceToBox > pSphere->m_fRadius * pSphere->m_fRadius )
		return 0;

	return 1;
}