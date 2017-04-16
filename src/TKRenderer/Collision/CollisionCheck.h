#ifndef __COLLISIONCHECK_H__
#define __COLLISIONCHECK_H__

#include "../TKCommon.h"

//OBB ����ü
struct ST_OBB {
	D3DXVECTOR3	m_vCenterPos;	//OBB�� �߽�
	D3DXVECTOR3 m_vAxisDir[3];	//OBB�� ����� 3�� (��������)
	float		m_fAxisLen[3];	//OBB�� ����� 3�� (������ �ϴ��� ����)
};

//�� ����ü
struct ST_Sphere {
	D3DXVECTOR3	m_vCenter;		//���� �߽���
	float		m_fRadius;		//���� ������
};

//�ﰢ�� ����ü.. �ð���� �������� �Ѵ�.
struct ST_Face {
	D3DXVECTOR3	m_vDot1;		//������1
	D3DXVECTOR3	m_vDot2;		//������2
	D3DXVECTOR3	m_vDot3;		//������3
	D3DXVECTOR3 m_vNor;			//�ﰢ���� �븻����
};

//���� ����ü.. ������ ����ȭ�Ǿ��־�� �Ѵ�.
struct ST_Ray {
	D3DXVECTOR3 m_vStartPos;	//Ray�� ������
	D3DXVECTOR3 m_vDirection;	//Ray�� ���� (��������)
	float		m_fLength;		//Ray�� ����
};


// �ﰢ���� �ﰢ�� �浹�˻�
int FaceFaceCollision( ST_Face* pFace1, ST_Face* pFace2 );

// ���� OBB �浹�˻�
int SphereOBBCollision( ST_Sphere* pSphere, ST_OBB* pOBB );

// �ﰢ���� Box�� �浹 �˻�
int FaceOBBCollision( ST_Face* pFace, ST_OBB* pOBB );

// ray�� ���� �浹 �˻�
int RaySphereCollision( ST_Ray* pRay, ST_Sphere* pSphere );

// ray�� OBB �浹�˻�
int RayOBBCollision( ST_Ray* pRay, ST_OBB* pOBB );

// ray�� �ﰢ�� �浹�˻�
int RayFaceCollision( ST_Ray* pRay, ST_Face* pFace );

//���� ���� �浹�˻�
int SphereSphereCollision( ST_Sphere* pSphere1, ST_Sphere* pSphere2 );

//OBB�� OBB�� �浹�˻�
int OBBOBBCollision( ST_OBB* pOBB1, ST_OBB* pOBB2 );

//���� �ﰢ���� �浹�˻�.. ��� ���鿡�� �ε����� ��츸 üũ�ϹǷ� 100% ��Ȯ������ �ʴ�.
//(������� �����Ϸ��� ������ �� ��µ�, �׷��� �ʾƵ� �ظ�ŭ �����ϹǷ� ���� ������ �� �ʿ�� �����)
int SphereFaceCollision( ST_Sphere* pSphere, ST_Face* pFace );

#endif