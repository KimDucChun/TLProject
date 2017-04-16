#include "CollisionCheck.h"

int OBBOBBCollision( ST_OBB* pOBB1, ST_OBB* pOBB2 )
{
	double c[3][3];
	double absC[3][3];
	double d[3];

	double r0, r1, r;
	int i;

	const double cutoff = 0.999999;
	int existsParallelPair = 0;

	D3DXVECTOR3 diff = pOBB1->m_vCenterPos - pOBB2->m_vCenterPos;



	for( i = 0 ; i < 3 ; ++i )
	{
		c[0][i] = D3DXVec3Dot( &pOBB1->m_vAxisDir[0], &pOBB2->m_vAxisDir[i] );
		absC[0][i] = abs( c[0][i] );
		if( absC[0][i] > cutoff )
			existsParallelPair = 1;
	}
	d[0] = D3DXVec3Dot( &diff, &pOBB1->m_vAxisDir[0] );
	r = abs( d[0] );
	r0 = pOBB1->m_fAxisLen[0];
	r1 = pOBB2->m_fAxisLen[0] * absC[0][0] + pOBB2->m_fAxisLen[1] * absC[0][1] + pOBB2->m_fAxisLen[2] * absC[0][2];

	if( r > r0 + r1 )
		return 0;



	for( i = 0 ; i < 3 ; ++i )
	{
		c[1][i] = D3DXVec3Dot( &pOBB1->m_vAxisDir[1], &pOBB2->m_vAxisDir[i] );
		absC[1][i] = abs( c[1][i] );
		if( absC[1][i] > cutoff )
			existsParallelPair = 1;
	}
	d[1] = D3DXVec3Dot( &diff, &pOBB1->m_vAxisDir[1] );
	r = abs( d[1] );
	r0 = pOBB1->m_fAxisLen[1];
	r1 = pOBB2->m_fAxisLen[0] * absC[1][0] + pOBB2->m_fAxisLen[1] * absC[1][1] + pOBB2->m_fAxisLen[2] * absC[1][2];

	if( r > r0 + r1 )
		return 0;



	for( i = 0 ; i < 3 ; ++i )
	{
		c[2][i] = D3DXVec3Dot( &pOBB1->m_vAxisDir[2], &pOBB2->m_vAxisDir[i] );
		absC[2][i] = abs( c[2][i] );
		if( absC[2][i] > cutoff )
			existsParallelPair = 1;
	}
	d[2] = D3DXVec3Dot( &diff, &pOBB1->m_vAxisDir[2] );
	r = abs( d[2] );
	r0 = pOBB1->m_fAxisLen[2];
	r1 = pOBB2->m_fAxisLen[0] * absC[2][0] + pOBB2->m_fAxisLen[1] * absC[2][1] + pOBB2->m_fAxisLen[2] * absC[2][2];

	if( r > r0 + r1 )
		return 0;



	r = abs( D3DXVec3Dot( &diff, &pOBB2->m_vAxisDir[0] ) );
	r0 = pOBB1->m_fAxisLen[0] * absC[0][0] + pOBB1->m_fAxisLen[1] * absC[1][0] + pOBB1->m_fAxisLen[2] * absC[2][0];
	r1 = pOBB2->m_fAxisLen[0];

	if( r > r0 + r1 )
		return 0;



	r = abs( D3DXVec3Dot( &diff, &pOBB2->m_vAxisDir[1] ) );
	r0 = pOBB1->m_fAxisLen[0] * absC[0][1] + pOBB1->m_fAxisLen[1] * absC[1][1] + pOBB1->m_fAxisLen[2] * absC[2][1];
	r1 = pOBB2->m_fAxisLen[1];

	if( r > r0 + r1 )
		return 0;



	r = abs( D3DXVec3Dot( &diff, &pOBB2->m_vAxisDir[2] ) );
	r0 = pOBB1->m_fAxisLen[0] * absC[0][2] + pOBB1->m_fAxisLen[1] * absC[1][2] + pOBB1->m_fAxisLen[2] * absC[2][2];
	r1 = pOBB2->m_fAxisLen[2];

	if( r > r0 + r1 )
		return 0;

	if( existsParallelPair == 1 )
		return 1;

	r = abs( d[2]*c[1][0] - d[1]*c[2][0] );
	r0 = pOBB1->m_fAxisLen[1] * absC[2][0] + pOBB1->m_fAxisLen[2] * absC[1][0];
	r1 = pOBB2->m_fAxisLen[1] * absC[0][2] + pOBB2->m_fAxisLen[2] * absC[0][1];
	if( r > r0 + r1 )
		return 0;

	r = abs( d[2]*c[1][1] - d[1]*c[2][1] );
	r0 = pOBB1->m_fAxisLen[1] * absC[2][1] + pOBB1->m_fAxisLen[2] * absC[1][1];
	r1 = pOBB2->m_fAxisLen[0] * absC[0][2] + pOBB2->m_fAxisLen[2] * absC[0][0];
	if( r > r0 + r1 )
		return 0;

	r = abs( d[2]*c[1][2] - d[1]*c[2][2] );
	r0 = pOBB1->m_fAxisLen[1] * absC[2][2] + pOBB1->m_fAxisLen[2] * absC[1][2];
	r1 = pOBB2->m_fAxisLen[0] * absC[0][1] + pOBB2->m_fAxisLen[1] * absC[0][0];
	if( r > r0 + r1 )
		return 0;

	r = abs( d[0]*c[2][0] - d[2]*c[0][0] );
	r0 = pOBB1->m_fAxisLen[0] * absC[2][0] + pOBB1->m_fAxisLen[2] * absC[0][0];
	r1 = pOBB2->m_fAxisLen[1] * absC[1][2] + pOBB2->m_fAxisLen[2] * absC[1][1];
	if( r > r0 + r1 )
		return 0;

	r = abs( d[0]*c[2][1] - d[2]*c[0][1] );
	r0 = pOBB1->m_fAxisLen[0] * absC[2][1] + pOBB1->m_fAxisLen[2] * absC[0][1];
	r1 = pOBB2->m_fAxisLen[0] * absC[1][2] + pOBB2->m_fAxisLen[2] * absC[1][0];
	if( r > r0 + r1 )
		return 0;

	r = abs( d[0]*c[2][2] - d[2]*c[0][2] );
	r0 = pOBB1->m_fAxisLen[0] * absC[2][2] + pOBB1->m_fAxisLen[2] * absC[0][2];
	r1 = pOBB2->m_fAxisLen[0] * absC[1][1] + pOBB2->m_fAxisLen[1] * absC[1][0];
	if( r > r0 + r1 )
		return 0;

	r = abs( d[1]*c[0][0] - d[0]*c[1][0] );
	r0 = pOBB1->m_fAxisLen[0] * absC[1][0] + pOBB1->m_fAxisLen[1] * absC[0][0];
	r1 = pOBB2->m_fAxisLen[1] * absC[2][2] + pOBB2->m_fAxisLen[2] * absC[2][1];
	if( r > r0 + r1 )
		return 0;

	r = abs( d[1]*c[0][1] - d[0]*c[1][1] );
	r0 = pOBB1->m_fAxisLen[0] * absC[1][1] + pOBB1->m_fAxisLen[1] * absC[0][1];
	r1 = pOBB2->m_fAxisLen[0] * absC[2][2] + pOBB2->m_fAxisLen[2] * absC[2][0];
	if( r > r0 + r1 )
		return 0;

	r = abs( d[1]*c[0][2] - d[0]*c[1][2] );
	r0 = pOBB1->m_fAxisLen[0] * absC[1][2] + pOBB1->m_fAxisLen[1] * absC[0][2];
	r1 = pOBB2->m_fAxisLen[0] * absC[2][1] + pOBB2->m_fAxisLen[1] * absC[2][0];
	if( r > r0 + r1 )
		return 0;

	return 1;
}