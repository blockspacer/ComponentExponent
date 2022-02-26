//=============================================================================
//
//  �e�N���X [Bullet.cpp]
//  Date   : 2021/11/4
//
//=============================================================================

#include "Bullet.h"
#include "../../../Component/Component_SkeletalMesh.h"
#include "../../../Component/Component_OBB.h"


constexpr float PLAYER_VELOCITY = 0.15f;


void  Bullet::Init()//������
{
	Actor3D::Init();//������

	m_Rotation = Vector3{ DegToRad(0.0f),DegToRad(0.0f),DegToRad(0.0f)};//3������]�����l
	m_Scale    = Vector3{ 1.0f,1.0f,1.0f };//3�����T�C�Y�����l

    //OBB�R���|�[�l���g�ǉ�
	m_Component_OBB = AddComponent<Component_OBB>(0);
	m_Component_OBB->SetVisibility(true);
	m_Component_OBB->SetScale(Vector3{ 2.0f,2.0f,2.0f });
	m_Component_OBB->SetIsReactRay(false);

}

void Bullet::Update()//�X�V
{
	Vector3 forward = GetForward();

	m_Position += forward * 0.1f;

	Actor3D::Update();
}

