//=============================================================================
//
//  �G�l�~�[�N���X [Enemy.cpp]
//  Date   : 2021/11/4
//
//=============================================================================


#include "Enemy.h"
#include "../../../../Component/Component_SkeletalMesh.h"
#include "../../../../Component/Component_OBB.h"
#include "../../../../Component/Component_Rigidbody.h"

static bool a = false;
static int b = 0;


void  Enemy::Init()//������
{
	Character::Init();//������

	m_Position = Vector3{ 20.0f,1.0f,1.0f };//3�������W�����l
	m_Rotation = Vector3{ DegToRad(0.0f),DegToRad(0.0f),DegToRad(0.0f) };//3������]�����l
	m_Scale    = Vector3{ 1.0f,1.0f,1.0f };//3�����T�C�Y�����l

	m_Move = false;
}
void Enemy::Update()//�X�V
{
	Vector3 Forward = GetForward();//�t�H���[�h�擾
	Vector3 Right = GetRight(); //���C�g�擾

	b++;
	if (b > 360)
	{
		b = 0;
		a = !a;

	}
	if (m_Move)
	{
		if (a)//�O�����ړ�
		{
			m_Component_Rigidbody->SetMoveXZ(-GetForward() * m_PlayerVelocity);
		}
		else
		{
			m_Component_Rigidbody->SetMoveXZ(GetForward() * m_PlayerVelocity);
		}
	}

	Character::Update();
}

