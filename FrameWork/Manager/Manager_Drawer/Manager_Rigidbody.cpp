//=============================================================================
//
//  Rigidbodyマネージャー [Manager_Rigidbody.cpp]
//  Date   : 2021/11/22
//
//=============================================================================

//******************************************************************************
// インクルード
//******************************************************************************

#include "../../Actor/Base/Actor.h"
#include "Manager_Rigidbody.h"

constexpr float DAMPING     = 0.99f;//抵抗値  (値が大きいほど抵抗が強い)
constexpr float FRICTION    = 0.2f; //摩擦係数(値が大きいほど摩擦が強い)
constexpr float RESTITUTION = 0.0f; //反発力  (値が大きいほど跳ねる)

const double GRAVITY_SCALE = -128.8;//重力係数

//初期化
void Manager_Rigidbody::Init()
{

	// 衝突検出方法の選択(デフォルトを選択)
	btDefaultCollisionConfiguration *config = new btDefaultCollisionConfiguration();
	btCollisionDispatcher *dispatcher       = new btCollisionDispatcher(config);

	// ブロードフェーズ法の設定(Dynamic AABB tree method)
	btDbvtBroadphase *broadphase = new btDbvtBroadphase();

	// 拘束)のソルバ設定(剛体間リンク
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	// Bulletのワールド作成
	m_DynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
	m_DynamicsWorld->setGravity(btVector3(0, static_cast<btScalar>(GRAVITY_SCALE), 0));

	
	// デバッグ表示
	m_MyDebugDraw = new MyDebugDraw();

	m_MyDebugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe
		| btIDebugDraw::DBG_DrawContactPoints
		| btIDebugDraw::DBG_DrawConstraints);

	m_DynamicsWorld->setDebugDrawer(m_MyDebugDraw);

}
//更新
void Manager_Rigidbody::Update()
{
	//時間を進める
     	m_DynamicsWorld->stepSimulation(1.0f / 60.0f, 10);
}
//描画
void Manager_Rigidbody::Draw()
{
	// デバッグ表示
	m_DynamicsWorld->debugDrawWorld();
}

//終了
void Manager_Rigidbody::Uninit()
{
	// ワールド破棄
	delete m_DynamicsWorld;
	m_DynamicsWorld = nullptr;

	delete m_MyDebugDraw;
	m_MyDebugDraw = nullptr;
}

//リジッドボディ追加
Manager_Rigidbody::s_RigidBody* Manager_Rigidbody::AddRigidBody(btCollisionShape * _shape, btScalar _mass, const btTransform & _transform)
{
	bool isDynamic = _mass > 0.0f;

	btVector3 localInertia(0, 0, 0);

	if (isDynamic)
	{//動的オブジェクトの場合
		_shape->calculateLocalInertia(_mass, localInertia);
	}

	auto* state = new btDefaultMotionState(_transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(_mass, state, _shape, localInertia);

	//抵抗
	rbInfo.m_linearDamping  = DAMPING;
	rbInfo.m_angularDamping = DAMPING;

	//摩擦係数
	rbInfo.m_friction         = FRICTION;
	rbInfo.m_rollingFriction  = FRICTION;
	rbInfo.m_spinningFriction = FRICTION;

	// 反発力
	rbInfo.m_restitution = RESTITUTION;

	// RigidBodyが「止まった」と判定される閾値
	rbInfo.m_linearSleepingThreshold  = 0.0;
	rbInfo.m_angularSleepingThreshold = 0.0;

	//実態作成
	auto* body = new btRigidBody(rbInfo);
	m_DynamicsWorld->addRigidBody(body);

	//戻り値作成
	s_RigidBody* rigidBody = new s_RigidBody;

	rigidBody->state = state;
	rigidBody->body  = body;

	//固定軸設定
	body->setLinearFactor (btVector3(1, 1, 1));
	body->setAngularFactor(btVector3(0, 0, 0));

	return  rigidBody;
}

//スフィアリジッドボディ追加
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
//ボックスリジッドボディ追加
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
//カプセルリジッドボディ追加
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
//プレーンリジッドボディ追加
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

//リジッドボディを除去する
void Manager_Rigidbody::RemoveRigidBody(btRigidBody* _rigidBody)
{
	if (m_DynamicsWorld != nullptr)
	{
		m_DynamicsWorld->removeRigidBody(_rigidBody);
	}
}

//デバッグ描画用
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