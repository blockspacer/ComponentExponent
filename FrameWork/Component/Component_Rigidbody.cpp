//=============================================================================
//
//  Rigidbody�R���|�[�l���g [Component_Rigidbody.cpp]
//  Date   : 2021/11/22
//
//=============================================================================

#include "Component_Rigidbody.h"
#include "../Manager/Manager_Game.h"
#include "../Actor/3D/Base/Actor3D.h"

constexpr int   JUMP_UP_TIME = 6;//�W�����v�㏸����
constexpr float JUMP_POWER   = 500.0f;//�W�����v��

//������
void Component_Rigidbody::Init()
{
	Component::Init();

	m_RigidBody = nullptr;//nullptr���

	m_JumpTime = 0;//�W�����v���ԏ�����
}
//�I��
void Component_Rigidbody::Uninit()
{
	if (m_RigidBody != nullptr)
	{//�܂����W�b�h�{�f�B�̎��̂����鎞
		if (Manager_Rigidbody* manager_Rigidbody = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Rigidbody())
		{
			//BulletPhysics�}�l�[�W���[�擾�ł�������s
			manager_Rigidbody->RemoveRigidBody(m_RigidBody->body);

			delete m_RigidBody;
			m_RigidBody = nullptr;
		}
	}
}
//�X�V
void Component_Rigidbody::Update()
{
	if (m_RigidBody != nullptr)
	{
		if (Actor3D* actor3D = dynamic_cast<Actor3D*>(m_Owner))
		{
			//���݂̃g�����X�t�H�[���擾
			btTransform transform = m_RigidBody->body->getWorldTransform();

			//���݂̃R���W�����ʒu�擾
			btVector3 btPosition = transform.getOrigin();

			//�I�[�i�[�̉�]�擾
			Vector3 rotation = actor3D->GetRotation();
			//�I�[�i�[�̉�]���R���W�����ɐݒ�(y,x,z�̏��ł炵��)
			btVector3 btrotation = { rotation.y, rotation.x, rotation.z };
			transform.setRotation(btQuaternion{ btrotation.x(), btrotation.y(), btrotation.z() });

			//���݂̃g�����X�t�H�[�����ɐݒ�
			m_RigidBody->body->setWorldTransform(transform);

			//���W���擾
			btPosition = transform.getOrigin();

			//�W�����v�㏸���̏ꍇ
			if (m_JumpTime > 0)
			{
				//�㏸���Ԍ���
				m_JumpTime--;
				//�W�����v�͌v�Z
				Vector3 jumpImpulse = actor3D->GetUp() * JUMP_POWER;
				//�W�����v�͂ɉ������C���p���X��^����
				m_RigidBody->body->applyImpulse(btVector3{ jumpImpulse.x,jumpImpulse.y ,jumpImpulse.z }, m_RigidBody->body->getWorldTransform().getOrigin());
			}

			//�I�[�i�[�ɍ��W��ݒ�
			actor3D->SetPosition(Vector3{ btPosition.x(),btPosition.y(),btPosition.z() });
		}
	}
}

//���W�擾
Vector3 Component_Rigidbody::GetPosition()
{
	Vector3 value = { 0.0f,0.0f,0.0f };

	if (m_RigidBody != nullptr)
	{
		btVector3 btPosition = m_RigidBody->body->getWorldTransform().getOrigin();
		value = Vector3{ btPosition.x(),btPosition.y(),btPosition.z() };
	}
	return value;
}

//�X�P�[���ݒ�
void Component_Rigidbody::SetScale(const Vector3& _scale)
{
	m_Scale = _scale;
	//���݂̃g�����X�t�H�[���擾
	m_RigidBody->body->getCollisionShape()->setLocalScaling(btVector3{ _scale.x,_scale.y,_scale.z});
}

//XZ���ړ��ݒ�
void Component_Rigidbody::SetMoveXZ(const Vector3& _velocity)
{
	if (m_RigidBody != nullptr)
	{
		m_RigidBody->body->applyImpulse(btVector3{_velocity.x,m_RigidBody->body->getLinearVelocity().y(),_velocity.z }, m_RigidBody->body->getWorldTransform().getOrigin());
	}
}
//XYZ���ړ��ݒ�
void Component_Rigidbody::SetMoveXYZ(const Vector3& _velocity)
{
	if (m_RigidBody != nullptr)
	{
		m_RigidBody->body->applyImpulse(btVector3{ _velocity.x,_velocity.y,_velocity.z }, m_RigidBody->body->getWorldTransform().getOrigin());
	}
}

//�d�͐ݒ�
void Component_Rigidbody::SetGravity(const Vector3& _velocity)
{
	if (m_RigidBody != nullptr)
	{
		m_RigidBody->body->setGravity(btVector3{ _velocity.x,_velocity.y,_velocity.z });
	}
}

//�W�����v
void Component_Rigidbody::Jump()
{
	if (m_RigidBody != nullptr)
	{
		//�W�����v�㏸����
		m_JumpTime = JUMP_UP_TIME;
	}
}

//���[�v
void Component_Rigidbody::SetWarp(const Vector3& _position)
{
	if (m_RigidBody != nullptr)
	{
		//���݂̃g�����X�t�H�[���擾
		btTransform transform = m_RigidBody->body->getWorldTransform();

		//�ړ����ʂ��v�Z�����
		transform.setOrigin(btVector3{ _position.x,_position.y, _position.z });

		//�g�����X�t�H�[���̍X�V
		m_RigidBody->body->setWorldTransform(transform);
	}
}

//���x�擾
Vector3 Component_Rigidbody::GetVelociy()
{
	if (m_RigidBody != nullptr)
	{//�x���V�e�B�擾
		btVector3 velocity = m_RigidBody->body->getLinearVelocity();
		return Vector3{ velocity.x(),velocity.y(),velocity.z() };
	}

	return Vector3 { 0.0f,0.0f,0.0f };
}

//�X�t�B�A���W�b�h�{�f�B�ǉ�
void  Component_Rigidbody::AddSphere(const Vector3& _scale, const Vector3& _position, float _mass)
{
	if (m_RigidBody == nullptr)
	{//�܂����W�b�h�{�f�B�̎��̂��Ȃ���
		if (Manager_Rigidbody* manager_Rigidbody = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Rigidbody())
		{//BulletPhysics�}�l�[�W���[�擾�ł�������s
			m_Scale = _scale;
			m_RigidBody = manager_Rigidbody->AddSphere(_scale, _position, _mass);
		}
	}
}
//�{�b�N�X���W�b�h�{�f�B�ǉ�
void  Component_Rigidbody::AddBox(const Vector3& _scale, const Vector3& _position, float _mass)
{
	if (m_RigidBody == nullptr)
	{//�܂����W�b�h�{�f�B�̎��̂��Ȃ���
		if (Manager_Rigidbody* manager_Rigidbody = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Rigidbody())
		{//BulletPhysics�}�l�[�W���[�擾�ł�������s
			m_Scale = _scale;
			m_RigidBody = manager_Rigidbody->AddBox(_scale, _position, _mass);
		}
	}
}
//�J�v�Z�����W�b�h�{�f�B�ǉ�
void  Component_Rigidbody::AddCapsule(float _radius, float _height, const Vector3& _position, const Vector3& _rotation, float _mass)
{
	if (m_RigidBody == nullptr)
	{//�܂����W�b�h�{�f�B�̎��̂��Ȃ���
		if (Manager_Rigidbody* manager_Rigidbody = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Rigidbody())
		{//BulletPhysics�}�l�[�W���[�擾�ł�������s
			m_Scale = Vector3{ _radius,_height,_radius };
			m_RigidBody = manager_Rigidbody->AddCapsule(_radius, _height, _position, _rotation, _mass);
		}
	}
}
//�v���[�����W�b�h�{�f�B�ǉ�
void Component_Rigidbody::AddStaticPlane(const Vector3& _up, const Vector3& _position, float _mass)
{
	if (m_RigidBody == nullptr)
	{//�܂����W�b�h�{�f�B�̎��̂��Ȃ���
		if (Manager_Rigidbody* manager_Rigidbody = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Rigidbody())
		{//BulletPhysics�}�l�[�W���[�擾�ł�������s
			m_RigidBody = manager_Rigidbody->AddStaticPlane(_up, _position, _mass);
		}
	}
}

void Component_Rigidbody::AddCylinder(const Vector3& _pos, const Vector3& _scale, const Vector3& _rot, float _mass)
{
	if (m_RigidBody == nullptr)
	{//�܂����W�b�h�{�f�B�̎��̂��Ȃ���
		if (Manager_Rigidbody* manager_Rigidbody = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Rigidbody())
		{//BulletPhysics�}�l�[�W���[�擾�ł�������s
			m_RigidBody = manager_Rigidbody->AddCylinder(_pos, _scale, _rot, _mass);
		}
	}
}

bool Component_Rigidbody::SaveComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName,  "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Save<Component_Rigidbody>(_fileName, this, sizeof(Component_Rigidbody), tempName))
	{
		return false;
	}
	return true;
}

bool Component_Rigidbody::LoadComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName,  "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Load<Component_Rigidbody>(_fileName, this, tempName))
	{
		return false;
	}
	return true;
}

void Component_Rigidbody::SetFriction(const float & _friction)
{
	if (m_RigidBody != nullptr)
	{
		m_RigidBody->body->setFriction(_friction);
	}
}
