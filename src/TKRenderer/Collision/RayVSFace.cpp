#include "CollisionCheck.h"


//http://jgt.akpeters.com/papers/MollerTrumbore97/

#define EPSILON 0.000001
#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 

int RayFaceCollision( ST_Ray* pRay, ST_Face* pFace )
{
   double orig[3] = { pRay->m_vStartPos.x, pRay->m_vStartPos.y, pRay->m_vStartPos.z };
   double dir[3] = { pRay->m_vDirection.x, pRay->m_vDirection.y, pRay->m_vDirection.z };
   double vert0[3] = { pFace->m_vDot1.x, pFace->m_vDot1.y, pFace->m_vDot1.z };
   double vert1[3] = { pFace->m_vDot2.x, pFace->m_vDot2.y, pFace->m_vDot2.z };
   double vert2[3] = { pFace->m_vDot3.x, pFace->m_vDot3.y, pFace->m_vDot3.z };

   BOOL bIsInRange = FALSE;
   double RangeSquare = (pRay->m_fLength)*(pRay->m_fLength);

   // 근데 거리비교할때 거리가 1보다 작은 거리이면 제곱비교를 해도 괜찮을까..?
   if( RangeSquare > (orig[0]-vert0[0])*(orig[0]-vert0[0]) + (orig[1]-vert0[1])*(orig[1]-vert0[1]) + (orig[2]-vert0[2])*(orig[2]-vert0[2]) )
	   bIsInRange = TRUE;
   if( RangeSquare > (orig[0]-vert1[0])*(orig[0]-vert1[0]) + (orig[1]-vert1[1])*(orig[1]-vert1[1]) + (orig[2]-vert1[2])*(orig[2]-vert1[2]) )
	   bIsInRange = TRUE;
   if( RangeSquare > (orig[0]-vert2[0])*(orig[0]-vert2[0]) + (orig[1]-vert2[1])*(orig[1]-vert2[1]) + (orig[2]-vert2[2])*(orig[2]-vert2[2]) )
	   bIsInRange = TRUE;

   if( bIsInRange == FALSE )
	   return 0;

   double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
   double det,inv_det;
   double t, u, v;

   /* find vectors for two edges sharing vert0 */
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = DOT(edge1, pvec);

#ifdef TEST_CULL           /* define TEST_CULL if culling is desired */
   if (det < EPSILON)
      return 0;

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, orig, vert0);

   /* calculate U parameter and test bounds */
   u = DOT(tvec, pvec);
   if (u < 0.0 || u > det)
      return 0;

   /* prepare to test V parameter */
   CROSS(qvec, tvec, edge1);

    /* calculate V parameter and test bounds */
   v = DOT(dir, qvec);
   if (v < 0.0 || u + v > det)
      return 0;

   /* calculate t, scale parameters, ray intersects triangle */
   t = DOT(edge2, qvec);
   inv_det = 1.0 / det;
   t *= inv_det;
   u *= inv_det;
   v *= inv_det;
#else                    /* the non-culling branch */
   if (det > -EPSILON && det < EPSILON)
     return 0;
   inv_det = 1.0 / det;

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, orig, vert0);

   /* calculate U parameter and test bounds */
   u = DOT(tvec, pvec) * inv_det;
   if (u < 0.0 || u > 1.0)
     return 0;

   /* prepare to test V parameter */
   CROSS(qvec, tvec, edge1);

   /* calculate V parameter and test bounds */
   v = DOT(dir, qvec) * inv_det;
   if (v < 0.0 || u + v > 1.0)
     return 0;

   /* calculate t, ray intersects triangle */
   t = DOT(edge2, qvec) * inv_det;
#endif
   return 1;
}