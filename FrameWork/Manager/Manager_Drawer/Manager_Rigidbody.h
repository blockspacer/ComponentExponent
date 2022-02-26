//=============================================================================
//
//  Rigidbody�}�l�[�W���[ [Manager_Rigidbody.h]
//  Date   : 2021/11/22
//
//=============================================================================
#pragma once

#include <unordered_map>
#include "../../Project/Bullet/src/btBulletDynamicsCommon.h"
#include "../../Renderer/Renderer_Polygon.h"
#include "../../System/Main.h"

#if defined(_DEBUG)
#pragma comment(lib, "Bullet/bin/BulletCollision_Debug.lib")
#pragma comment(lib, "Bullet/bin/BulletDynamics_Debug.lib")
#pragma comment(lib, "Bullet/bin/LinearMath_Debug.lib")
#else
#pragma comment(lib, "Bullet/bin/BulletCollision.lib")
#pragma comment(lib, "Bullet/bin/BulletDynamics.lib")
#pragma comment(lib, "Bullet/bin/LinearMath.lib")
#endif

//******************************************************************************
// �N���X��`
//******************************************************************************
class Manager_Rigidbody
{

public:

	//���W�b�h�{�f�B�\����
	typedef struct 
	{

		btMotionState* state; //���
		btRigidBody*   body;//����

	} s_RigidBody;

	void Init();//������
	void Update();//�X�V
	void Draw();//�`��
	void Uninit();//�I��

	//�X�t�B�A���W�b�h�{�f�B�ǉ�
	s_RigidBody* AddSphere(const Vector3& _scale, const Vector3& _position, float _mass);
	//�{�b�N�X���W�b�h�{�f�B�ǉ�
	s_RigidBody* AddBox(const Vector3& _scale, const Vector3& _position, float _mass);
	//�J�v�Z�����W�b�h�{�f�B�ǉ�
	s_RigidBody* AddCapsule(float _radius, float _height, const Vector3& _position, const Vector3& _rotation, float _mass);
	//�v���[�����W�b�h�{�f�B�ǉ�
	s_RigidBody* AddStaticPlane(const Vector3& _up, const Vector3& _position, float _mass);
	//�V�����_�[�ǉ�
	s_RigidBody* AddCylinder(const Vector3& _pos, const Vector3& _scale, const Vector3& _rot, float _mass);

	//Bullet���[���h�擾
	inline btDynamicsWorld* GebtDynamicsWorld() { return m_DynamicsWorld; }

	//���W�b�h�{�f�B����������
	void RemoveRigidBody(btRigidBody* _rigidBody);

private:

	btDynamicsWorld*   m_DynamicsWorld = 0;//Bullet���[���h
	class MyDebugDraw* m_MyDebugDraw;      //�f�o�b�O�`��N���X

	//���W�b�h�{�f�B�ǉ�
	s_RigidBody* AddRigidBody(btCollisionShape* _shape, btScalar _mass, const btTransform& _transform);

};

// �f�o�b�O�`��N���X
class MyDebugDraw : public btIDebugDraw
{
	int Debug_mode;//�f�o�b�O���[�h

	void drawLine(const btVector3& _from, const btVector3& _to,
		const btVector3& _color);

	void drawLine(const btVector3& _from, const btVector3& _to,
		const btVector3& _fromColor, const btVector3& _toColor);


	/* PointOnB�ɁA�Փ˓_��`�� */
	void drawContactPoint(const btVector3& _PointOnB, const btVector3& _normalOnB,
		btScalar _distance, int _lifeTime,
		const btVector3& _color){}


	/* �x���\�� */
	void reportErrorWarning(const char* _warningString){}

	/* �w���ԍ��W�ɕ�����\�� */
	void draw3dText(const btVector3& _location, const char* _textString){}

public:

	/* �f�o�b�O���[�h�w�� */
	inline void setDebugMode(int _debugMode){Debug_mode = _debugMode;}

	/* ���݂̃f�o�b�O���[�h��ԋp */
	inline int getDebugMode() const override{return Debug_mode;}


};