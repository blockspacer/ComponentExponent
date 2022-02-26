//=============================================================================
//
//  ���W�b�h�{�f�B�A�N�^�[���N���X [ActorRB.cpp]
//  Date   : 2021/11/26
//
//=============================================================================

#include "ActorRB.h"


//������
void  ActorRB::Init()
{
	//������
	Actor3D::Init();

	//Rigidbody�R���|�[�l���g�ǉ�
	m_Component_Rigidbody = AddComponent<Component_Rigidbody>(0);

	//OBB�R���|�[�l���g�ǉ�
	m_Component_OBB = AddComponent<Component_OBB>(0);
	m_Component_OBB->SetVisibility(true);//����
	m_Component_OBB->SetScale(Vector3{ 1,1,1 });//�X�P�[���ݒ�
}

void ActorRB::Uninit()
{
	Actor3D::Uninit();

	//�|�C���^�[��Y���
	m_Component_OBB = nullptr;
	m_Component_Rigidbody = nullptr;
}

//�X�t�B�A���W�b�h�{�f�B�ǉ�
void ActorRB::AddSphereRB(const Vector3& _scale, const Vector3& _position, float _mass)
{
	if (m_Component_Rigidbody != nullptr)
	{//���W�b�h�{�f�B�̎��Ԃ�����ꍇ
		m_Component_Rigidbody->AddSphere(HalfSize(_scale), _position, _mass);
	}
	if (m_Component_OBB != nullptr)
	{//OBB�R���|�[�l���g�̎��Ԃ�����Ȃ�
		m_Component_OBB->SetScale(_scale);//�X�P�[���ݒ�
	}
}
//�{�b�N�X���W�b�h�{�f�B�ǉ�
void ActorRB::AddBoxRB(const Vector3& _scale, const Vector3& _position, float _mass)
{
	if (m_Component_Rigidbody != nullptr)
	{//���W�b�h�{�f�B�̎��Ԃ�����ꍇ
		m_Component_Rigidbody->AddBox(HalfSize(_scale), _position, _mass);
	}
	if (m_Component_OBB != nullptr)
	{//OBB�R���|�[�l���g�̎��Ԃ�����Ȃ�
		m_Component_OBB->SetScale(_scale);//�X�P�[���ݒ�
	}
}
//�J�v�Z�����W�b�h�{�f�B�ǉ�
void ActorRB::AddCapsuleRB(float _radius, float _height, const Vector3& _position, const Vector3& _rotation, float _mass)
{
	if (m_Component_Rigidbody != nullptr)
	{//���W�b�h�{�f�B�̎��Ԃ�����ꍇ
		m_Component_Rigidbody->AddCapsule(HalfSize(_radius), HalfSize(_height),_position,_rotation,_mass);
	}
	if (m_Component_OBB != nullptr)
	{//OBB�R���|�[�l���g�̎��Ԃ�����Ȃ�
		m_Component_OBB->SetScale(Vector3{ _radius,_height,_radius });//�X�P�[���ݒ�
	}
}

void ActorRB::AddCylinderRB(const Vector3& _pos, const Vector3& _scale, const Vector3& _rot, float _mass)
{
	if (m_Component_Rigidbody != nullptr)
	{//���W�b�h�{�f�B�̎��Ԃ�����ꍇ
		m_Component_Rigidbody->AddCylinder(_pos, _scale, _rot, _mass);
	}
}

//Rigidbody���W�ݒ�
void ActorRB::SetRigidbodyPostion(Vector3 _position)
{
	if (m_Component_Rigidbody != nullptr)
	{//���W�b�h�{�f�B�̎��Ԃ�����ꍇ
		m_Component_Rigidbody->SetWarp(_position);
	}
}

//Rigidbody�X�P�[���ݒ�
void ActorRB::SetRigidbodyScale(Vector3 _scale)
{
	//��b�X�P�[����ݒ�
	m_Scale = _scale;

	if (m_Component_Rigidbody != nullptr)
	{//���W�b�h�{�f�B�̎��Ԃ�����ꍇ
		m_Component_Rigidbody->SetScale(_scale);
	}
	if (m_Component_OBB != nullptr)
	{//OBB�R���|�[�l���g�̎��Ԃ�����Ȃ�
		m_Component_OBB->SetScale(_scale);
	}
}



