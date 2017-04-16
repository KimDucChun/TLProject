#include "CollisionCheck.h"

int RaySphereCollision( ST_Ray* pRay, ST_Sphere* pSphere )
{
	D3DXVECTOR3 vDiff = pSphere->m_vCenter - pRay->m_vStartPos;
	float fLen = D3DXVec3Length( &vDiff );
	float fLen2 = pow(fLen, 2);
	
	float fRadius2 = pow(pSphere->m_fRadius, 2);
	
	float fS = D3DXVec3Dot( &pRay->m_vDirection, &vDiff );
	float fS2 = pow(fS, 2);
	
	if ( fS < 0.0f && fLen2 > fRadius2 ) 
	    return 0;
	    
    //float fQ = sqrt( fLen2 + fS2 );
    //if  ( fQ > pSphere->m_fRadius ) return 0;
	
	float fM2 = fLen2 - fS2;
	if ( fM2 > fRadius2 ) return 0;
    
    return 1;	
	
	//float fDiffLengthToDirection = D3DXVec3Dot( &vDiff, &pRay->m_vDirection );
	//float fDiffLengthSquare = D3DXVec3Dot( &vDiff, &vDiff );  // 동일벡터의 내적이므로 무조건 해당 벡터의 길이가 나온다 ?
	//float fSphereRadiusSquare = pSphere->m_fRadius * pSphere->m_fRadius;

	//if( fDiffLengthToDirection < 0 && fDiffLengthSquare > fSphereRadiusSquare )
	//	return 0;

	//float fPerpendicularLengthSquare = fDiffLengthSquare - fDiffLengthToDirection*fDiffLengthToDirection;

	//if( fPerpendicularLengthSquare > fSphereRadiusSquare )
	//	return 0;

	//return 1;
}