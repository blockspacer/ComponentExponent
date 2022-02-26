//=============================================================================
//
//  Rigidbody�}�l�[�W���[ [Manager_Rigidbody.cpp]
//  Date   : 2021/11/22
//
//=============================================================================

//******************************************************************************
// �C���N���[�h
//******************************************************************************

#include "../../Actor/Base/Actor.h"
#include "Manager_Rigidbody.h"

constexpr float DAMPING     = 0.99f;//��R�l  (�l���傫���قǒ�R������)
constexpr float FRICTION    = 0.2f; //���C�W��(�l���傫���قǖ��C������)
constexpr float RESTITUTION = 0.0f; //������  (�l���傫���قǒ��˂�)

const double GRAVITY_SCALE = -128.8;//�d�͌W��

//������
void Manager_Rigidbody::Init()
{

	// �Փˌ��o���@�̑I��(�f�t�H���g��I��)
	btDefaultCollisionConfiguration *config = new btDefaultCollisionConfiguration();
	btCollisionDispatcher *dispatcher       = new btCollisionDispatcher(config);

	// �u���[�h�t�F�[�Y�@�̐ݒ�(Dynamic AABB tree method)
	btDbvtBroadphase *broadphase = new btDbvtBroadphase();

	// �S��)�̃\���o�ݒ�(���̊ԃ����N
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	// Bullet�̃��[���h�쐬
	m_DynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
	m_DynamicsWorld->setGravity(btVector3(0, static_cast<btScalar>(GRAVITY_SCALE), 0));

	
	// �f�o�b�O�\��
	m_MyDebugDraw = new MyDebugDraw();

	m_MyDebugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe
		| btIDebugDraw::DBG_DrawContactPoints
		| btIDebugDraw::DBG_DrawConstraints);

	m_DynamicsWorld->setDebugDrawer(m_MyDebugDraw);

}
//�X�V
void Manager_Rigidbody::Update()
{
	//���Ԃ�i�߂�
     	m_DynamicsWorld->stepSimulation(1.0f / 60.0f, 10);
}
//�`��
void Manager_Rigidbody::Draw()
{
	// �f�o�b�O�\��
	m_DynamicsWorld->debugDrawWorld();
}

//�I��
void Manager_Rigidbody::Uninit()
{
	// ���[���h�j��
	delete m_DynamicsWorld;
	m_DynamicsWorld = nullptr;

	delete m_MyDebugDraw;
	m_MyDebugDraw = nullptr;
}

//���W�b�h�{�f�B�ǉ�
Manager_Rigidbody::s_RigidBody* Manager_Rigidbody::AddRigidBody(btCollisionShape * _shape, btScalar _mass, const btTransform & _transform)
{
	bool isDynamic = _mass > 0.0f;

	btVector3 localInertia(0, 0, 0);

	if (isDynamic)
	{//���I�I�u�W�F�N�g�̏ꍇ
		_shape->calculateLocalInertia(_mass, localInertia);
	}

	auto* state = new btDefaultMotionState(_transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(_mass, state, _shape, localInertia);

	//��R
	rbInfo.m_linearDamping  = DAMPING;
	rbInfo.m_angularDamping = DAMPING;

	//���C�W��
	rbInfo.m_friction         = FRICTION;
	rbInfo.m_rollingFriction  = FRICTION;
	rbInfo.m_spinningFriction = FRICTION;

	// ������
	rbInfo.m_restitution = RESTITUTION;

	// RigidBody���u�~�܂����v�Ɣ��肳���臒l
	rbInfo.m_linearSleepingThreshold  = 0.0;
	rbInfo.m_angularSleepingThreshold = 0.0;

	//���ԍ쐬
	auto* body = new btRigidBody(rbInfo);
	m_DynamicsWorld->addRigidBody(body);

	//�߂�l�쐬
	s_RigidBody* rigidBody = new s_RigidBody;

	rigidBody->state = state;
	rigidBody->body  = body;

	//�Œ莲�ݒ�
	body->setLinearFactor (btVector3(1, 1, 1));
	body->setAngularFactor(btVector3(0, 0, 0));

	return  rigidBody;
}

//�X�t�B�A���W�b�h�{�f�B�ǉ�
Manager_Rigidbody::s_RigidBody* Manager_Rigidbody::AddSphere(const Vector3& _scale, const Vector3& _position, float _mass)
{
	btVector3 btPostion(_position.x, _position.y, _position.z);
	btVector3 btScale(_scale.x, _scale.y, _scale.z);

	auto* shape = new btSphereShape(btScale.x());

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btPostion);

	return AddRigidBody(shape, _mass, transform);
}
//�{�b�N�X���W�b�h�{�f�B�ǉ�
Manager_Rigidbody::s_RigidBody* Manager_Rigidbody::AddBox(const Vector3& _scale, const Vector3& _position, float _mass)
{
	btVector3 btPostion(_position.x, _position.y, _position.z);
	btVector3 btScale(_scale.x, _scale.y, _scale.z);

	auto* shape = new btBoxShape(btScale);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btPostion);

	return AddRigidBody(shape, _mass, transform);
}
//�J�v�Z�����W�b�h�{�f�B�ǉ�
Manager_Rigidbody::s_RigidBody* Manager_Rigidbody::AddCapsule(float _radius, float _height, const Vector3& _position, const Vector3& _rotation, float _mass)
{
	btVector3 btPostion(_position.x, _position.y, _position.z);
	btVector3 btrotation(_rotation.x, _rotation.y, _rotation.z);

	auto* shape = new btCapsuleShape(_radius, _height);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btPostion);
	transform.setRotation(btQuaternion(btrotation.x(), btrotation.y(), btrotation.z()));

	return AddRigidBody(shape, _mass, transform);
}
//�v���[�����W�b�h�{�f�B�ǉ�
Manager_Rigidbody::s_RigidBody* Manager_Rigidbody::AddStaticPlane(const Vector3& _up, const Vector3& _position, float _mass)
{
	btVector3 btPostion(_position.x, _position.y, _position.z);
	btVector3 btUp(_up.x, _up.y, _up.z);

	auto* shape = new btStaticPlaneShape(btUp, 0);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btPostion);

	return AddRigidBody(shape, _mass, transform);
}

Manager_Rigidbody::s_RigidBody * Manager_Rigidbody::AddCylinder(const Vector3& _pos, const Vector3& _scale, const Vector3& _rot, float _mass)
{
	btVector3 btPostion(_pos.x, _pos.y, _pos.z);
	btVector3 btrotation(_rot.x, _rot.y, _rot.z);
	btVector3 btScale(_scale.x, _scale.y, _scale.z);

	auto* shape = new btCylinderShape(btScale);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btPostion);
	transform.setRotation(btQuaternion(btrotation.x(), btrotation.y(), btrotation.z()));

	return AddRigidBody(shape, _mass, transform);
}

//���W�b�h�{�f�B����������
void Manager_Rigidbody::RemoveRigidBody(btRigidBody* _rigidBody)
{
	if (m_DynamicsWorld != nullptr)
	{
		m_DynamicsWorld->removeRigidBody(_rigidBody);
	}
}

//�f�o�b�O�`��p
void MyDebugDraw::drawLine(const btVector3& _from, const btVector3& _to,
	const btVector3& _color)
{
	Renderer_Polygon::Instance()->DrawLine(Vector3{ _from.x(),_from.y(),_from.z() }, Vector3{ _to.x(),_to.y(),_to.z() }, Color{ _color.x(),_color.y(),_color.z(),1.0f });
}

void MyDebugDraw::drawLine(const btVector3& _from, const btVector3& _to,
	const btVector3& _fromColor, const btVector3& _toColor)
{
	Renderer_Polygon::Instance()->DrawLine(Vector3{ _from.x(),_from.y(),_from.z() }, Vector3{ _to.x(),_to.y(),_to.z() }, Color{ _fromColor.x(),_fromColor.y(),_fromColor.z(),1.0f });
}