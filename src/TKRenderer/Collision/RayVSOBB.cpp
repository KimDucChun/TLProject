#include "CollisionCheck.h"


void swapf(float &a, float &b)
{
    float temp;
    temp = a;
    a = b;
    b = temp;
}

bool RayOBBoxIntersect(D3DXVECTOR3 &vecStart, D3DXVECTOR3 &vecDir, D3DXVECTOR3 &vecCenter, D3DXVECTOR3 vecAxisDir[3], float AxisLen[3], float& t)
{
    float tfirst, tlast;
    float fstart;
    float fdir;
    float fmin, fmax;
    int i;
    D3DXVECTOR3 vecP;
    
    tfirst = (float)-0x7FFFFFF;
    tlast = (float)0x7FFFFFF;
    fstart = 0.0f;
    fdir = 0.0f;
    fmin = 0.0f;
    fmax = 0.0f;
    
    vecP = vecCenter - vecStart;
    
    for ( i = 0 ; i < 3 ; ++i )
    {
        fstart = D3DXVec3Dot(&vecAxisDir[i], &vecP);
        fdir = D3DXVec3Dot(&vecAxisDir[i], &vecDir);
        if (fabs(fdir) > 0.0f )
        {
            fmin = (fstart + AxisLen[i]) / fdir;
            fmax = (fstart - AxisLen[i]) / fdir;
            if ( fmin > fmax ) swapf( fmin, fmax );
            if ( fmin > tfirst ) tfirst = fmin;
            if ( fmax < tlast  ) tlast = fmax;
            
            if ( tfirst > tlast ) return false;
            if ( tlast < 0x7FFFFFF * -1.0f ) return false;
        }
        else if ( -fstart-AxisLen[i] > 0.0f || -fstart+AxisLen[i] < 0.0f )
        {
            return false;
        }
    }
    
    if ( tfirst > 0.0f ) t = tfirst;
    else t = tlast;
    
    //static char strTemp[255];
    //sprintf(strTemp, "%f\t", t);
    //OutputDebugString( strTemp );
    
    return true;
}

int RayOBBCollision( ST_Ray* pRay, ST_OBB* pOBB )
{
    float f;
    bool bIntersect;
    
    bIntersect = RayOBBoxIntersect( pRay->m_vStartPos, pRay->m_vDirection, pOBB->m_vCenterPos, pOBB->m_vAxisDir, pOBB->m_fAxisLen, f );
    
    if (bIntersect)
    {
        if (f <= pRay->m_fLength )
        {
            return 1;
        }
        else
        {
            return 0;
        }        
    }
    else
    {
        return 0;
    }
    
	//D3DXVECTOR3* pOBBPos = &pOBB->m_vCenterPos;
	//D3DXVECTOR3* pAxis1 = &pOBB->m_vAxisDir[0];
	//D3DXVECTOR3* pAxis2 = &pOBB->m_vAxisDir[1];
	//D3DXVECTOR3* pAxis3 = &pOBB->m_vAxisDir[2];
	//float fAxisLen1 = pOBB->m_fAxisLen[0];
	//float fAxisLen2 = pOBB->m_fAxisLen[1];
	//float fAxisLen3 = pOBB->m_fAxisLen[2];
	//D3DXVECTOR3 vRelativeVec;

	//vRelativeVec = pRay->m_vStartPos - (*pOBBPos);
	//D3DXVECTOR3 vNewRayPoint( D3DXVec3Dot( pAxis1, &vRelativeVec), D3DXVec3Dot( pAxis2, &vRelativeVec), D3DXVec3Dot( pAxis3, &vRelativeVec) );

	//D3DXVECTOR3 vRayMiddle = pRay->m_vStartPos + ( pRay->m_vDirection * pRay->m_fLength );
	//vRelativeVec = vRayMiddle - (*pOBBPos);
	//D3DXVECTOR3 vNewRayMiddlePoint( D3DXVec3Dot( pAxis1, &vRelativeVec), D3DXVec3Dot( pAxis2, &vRelativeVec), D3DXVec3Dot( pAxis3, &vRelativeVec) );

	//float fWidthX = vNewRayPoint.x - vNewRayMiddlePoint.x;
	//float fWidthY = vNewRayPoint.y - vNewRayMiddlePoint.y;
	//float fWidthZ = vNewRayPoint.z - vNewRayMiddlePoint.z;
	//fWidthX = ( fWidthX < 0.f )? -fWidthX : fWidthX;
	//fWidthY = ( fWidthY < 0.f )? -fWidthY : fWidthY;
	//fWidthZ = ( fWidthZ < 0.f )? -fWidthZ : fWidthZ;

	//float fMiddleX = ( vNewRayMiddlePoint.x < 0.f )? -vNewRayMiddlePoint.x : vNewRayMiddlePoint.x;
	//float fMiddleY = ( vNewRayMiddlePoint.y < 0.f )? -vNewRayMiddlePoint.y : vNewRayMiddlePoint.y;
	//float fMiddleZ = ( vNewRayMiddlePoint.z < 0.f )? -vNewRayMiddlePoint.z : vNewRayMiddlePoint.z;

	//if( fMiddleX > fWidthX + fAxisLen1 ) return 0;
	//if( fMiddleY > fWidthY + fAxisLen2 ) return 0;
	//if( fMiddleZ > fWidthZ + fAxisLen3 ) return 0;

	//float fRaySAT1 = fMiddleY*fWidthZ - fMiddleZ*fWidthY;
	//float fRaySAT2 = fMiddleX*fWidthZ - fMiddleZ*fWidthX;
	//float fRaySAT3 = fMiddleX*fWidthY - fMiddleY*fWidthX;
	//fRaySAT1 = ( fRaySAT1 < 0.f )? -fRaySAT1 : fRaySAT1;
	//fRaySAT2 = ( fRaySAT2 < 0.f )? -fRaySAT2 : fRaySAT2;
	//fRaySAT3 = ( fRaySAT3 < 0.f )? -fRaySAT3 : fRaySAT3;

	//if( fRaySAT1 > fAxisLen2*fWidthZ + fAxisLen3*fWidthY ) return 0;
	//if( fRaySAT2 > fAxisLen1*fWidthZ + fAxisLen3*fWidthX ) return 0;
	//if( fRaySAT3 > fAxisLen1*fWidthY + fAxisLen2*fWidthX ) return 0;

	//return 1;
}
