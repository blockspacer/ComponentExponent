//=============================================================================
//
//  Rigidbody�R���|�[�l���g [Component_Rigidbody.h]
//  Date   : 2021/11/22
//
//=============================================================================
#pragma once
#include "Base/Component.h"
#include "../Manager/Manager_Drawer/Manager_Rigidbody.h"

const float ON_GROUND_VELOCITY = 0.1f;//�ڒn����Ƃ��鑬�x

class Component_Rigidbody : public Component
{
private:

	//���W�b�h�{�f�B
	Manager_Rigidbody::s_RigidBody* m_RigidBody;

	//�T�C�Y
	Vector3 m_Scale;

	//�W�����v�㏸����
	int m_JumpTime;

public:

	Component_Rigidbody(class Actor* _owner, int _updatePriority) : Component(_owner, _updatePriority) {}

	virtual void Init() override; //������
	virtual void Uninit()override;//�I��
	virtual void Update()override;//�X�V

	//���W�擾
	Vector3 GetPosition();

	//�X�P�[���ݒ�
	void SetScale(const Vector3& _scale);

	//XZ���ړ��ݒ�
	void SetMoveXZ(const Vector3& _velocity);

	//XYZ���ړ��ݒ�
	void SetMoveXYZ(const Vector3& _velocity);

	//�d�͐ݒ�
	void SetGravity(const Vector3& _velocity);

	//�W�����v
	void Jump();

	//���[�v
	void SetWarp(const Vector3& _position);

	//���x�擾
	Vector3 GetVelociy();

	//�X�t�B�A���W�b�h�{�f�B�ǉ�
	void AddSphere(const Vector3& _scale, const Vector3& _position, float _mass);
	//�{�b�N�X���W�b�h�{�f�B�ǉ�
	void AddBox(const Vector3& _scale, const Vector3& _position, float _mass);
	//�J�v�Z�����W�b�h�{�f�B�ǉ�
	void AddCapsule(float radius, float _height, const Vector3& _position, const Vector3& _rotation, float _mass);
	//�v���[�����W�b�h�{�f�B�ǉ�
	void AddStaticPlane(const Vector3& _up, const Vector3& _position, float _mass);

	void AddCylinder(const Vector3& _pos, const Vector3& _scale, const Vector3& _rot, float _mass);

	bool SaveComponentData(const char* _fileName) override;
	bool LoadComponentData(const char* _fileName) override;

	//���C�ݒ�
	void SetFriction(const float& _friction);
};

