//=============================================================================
//
//  ���W�b�h�{�f�B�A�N�^�[���N���X [ActorRB.h]
//  Date   : 2021/11/26
//
//=============================================================================
#pragma once


#include "../../Base/Actor3D.h"
#include "../../../../Component/Component_Rigidbody.h"
#include "../../../../Component/Component_OBB.h"

class ActorRB : public Actor3D
{
private:

	int mass;

protected:

	class Component_Rigidbody*     m_Component_Rigidbody;    //Rigidbody�R���|�[�l���g
	class Component_OBB*           m_Component_OBB;          //OBB�R���|�[�l���g

public:

	ActorRB():mass(0) {}
	ActorRB(int _mass) :mass(_mass) {}
	virtual ~ActorRB() {}

	virtual void Init()override;//������
	void Uninit() override;

	void SetRigidbodyPostion(Vector3 _position);//Rigidbody���W�ݒ�
	void SetRigidbodyScale  (Vector3 _scale);   //Rigidbody�X�P�[���ݒ�

	//�X�t�B�A���W�b�h�{�f�B�ǉ�
	void AddSphereRB(const Vector3& _scale, const Vector3& _position, float _mass);
	//�{�b�N�X���W�b�h�{�f�B�ǉ�
	void AddBoxRB(const Vector3& _scale, const Vector3& _position, float _mass);
	//�J�v�Z�����W�b�h�{�f�B�ǉ�
	void AddCapsuleRB(float _radius, float _height, const Vector3& _position, const Vector3& _rotation, float _mass);

	void AddCylinderRB(const Vector3& _pos, const Vector3& _scale, const Vector3& _rot, float _mass);
};