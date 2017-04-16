#include "CollisionCheck.h"

int SphereSphereCollision( ST_Sphere* pSphere1, ST_Sphere* pSphere2 )
{
	D3DXVECTOR3 vDiff = pSphere1->m_vCenter - pSphere2->m_vCenter;
	float fDistance = D3DXVec3Length( &vDiff );

	if( fDistance > pSphere1->m_fRadius + pSphere2->m_fRadius )
		return 0;

	return 1;
}