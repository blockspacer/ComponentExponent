//=============================================================================
//
//  Rigidbodyコンポーネント [Component_Rigidbody.cpp]
//  Date   : 2021/11/22
//
//=============================================================================

#include "Component_Rigidbody.h"
#include "../Manager/Manager_Game.h"
#include "../Actor/3D/Base/Actor3D.h"

constexpr int   JUMP_UP_TIME = 6;//ジャンプ上昇時間
constexpr float JUMP_POWER   = 500.0f;//ジャンプ力

//初期化
void Component_Rigidbody::Init()
{
	Component::Init();

	m_RigidBody = nullptr;//nullptr代入

	m_JumpTime = 0;//ジャンプ時間初期化
}
//終了
void Component_Rigidbody::Uninit()
{
	if (m_RigidBody != nullptr)
	{//まだリジッドボディの実体がある時
		if (Manager_Rigidbody* manager_Rigidbody = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Rigidbody())
		{
			//BulletPhysicsマネージャー取得できたら実行
			manager_Rigidbody->RemoveRigidBody(m_RigidBody->body);

			delete m_RigidBody;
			m_RigidBody = nullptr;
		}
	}
}
//更新
void Component_Rigidbody::Update()
{
	if (m_RigidBody != nullptr)
	{
		if (Actor3D* actor3D = dynamic_cast<Actor3D*>(m_Owner))
		{
			//現在のトランスフォーム取得
			btTransform transform = m_RigidBody->body->getWorldTransform();

			//現在のコリジョン位置取得
			btVector3 btPosition = transform.getOrigin();

			//オーナーの回転取得
			Vector3 rotation = actor3D->GetRotation();
			//オーナーの回転をコリジョンに設定(y,x,zの順版らしい)
			btVector3 btrotation = { rotation.y, rotation.x, rotation.z };
			transform.setRotation(btQuaternion{ btrotation.x(), btrotation.y(), btrotation.z() });

			//現在のトランスフォームをに設定
			m_RigidBody->body->setWorldTransform(transform);

			//座標を取得
			btPosition = transform.getOrigin();

			//ジャンプ上昇中の場合
			if (m_JumpTime > 0)
			{
				//上昇時間減少
				m_JumpTime--;
				//ジャンプ力計算
				Vector3 jumpImpulse = actor3D->GetUp() * JUMP_POWER;
				//ジャンプ力に応じたインパルスを与える
				m_RigidBody->body->applyImpulse(btVector3{ jumpImpulse.x,jumpImpulse.y ,jumpImpulse.z }, m_RigidBody->body->getWorldTransform().getOrigin());
			}

			//オーナーに座標を設定
			actor3D->SetPosition(Vector3{ btPosition.x(),btPosition.y(),btPosition.z() });
		}
	}
}

//座標取得
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

//スケール設定
void Component_Rigidbody::SetScale(const Vector3& _scale)
{
	m_Scale = _scale;
	//現在のトランスフォーム取得
	m_RigidBody->body->getCollisionShape()->setLocalScaling(btVector3{ _scale.x,_scale.y,_scale.z});
}

//XZ軸移動設定
void Component_Rigidbody::SetMoveXZ(const Vector3& _velocity)
{
	if (m_RigidBody != nullptr)
	{
		m_RigidBody->body->applyImpulse(btVector3{_velocity.x,m_RigidBody->body->getLinearVelocity().y(),_velocity.z }, m_RigidBody->body->getWorldTransform().getOrigin());
	}
}
//XYZ軸移動設定
void Component_Rigidbody::SetMoveXYZ(const Vector3& _velocity)
{
	if (m_RigidBody != nullptr)
	{
		m_RigidBody->body->applyImpulse(btVector3{ _velocity.x,_velocity.y,_velocity.z }, m_RigidBody->body->getWorldTransform().getOrigin());
	}
}

//重力設定
void Component_Rigidbody::SetGravity(const Vector3& _velocity)
{
	if (m_RigidBody != nullptr)
	{
		m_RigidBody->body->setGravity(btVector3{ _velocity.x,_velocity.y,_velocity.z });
	}
}

//ジャンプ
void Component_Rigidbody::Jump()
{
	if (m_RigidBody != nullptr)
	{
		//ジャンプ上昇時間
		m_JumpTime = JUMP_UP_TIME;
	}
}

//ワープ
void Component_Rigidbody::SetWarp(const Vector3& _position)
{
	if (m_RigidBody != nullptr)
	{
		//現在のトランスフォーム取得
		btTransform transform = m_RigidBody->body->getWorldTransform();

		//移動結果を計算し代入
		transform.setOrigin(btVector3{ _position.x,_position.y, _position.z });

		//トランスフォームの更新
		m_RigidBody->body->setWorldTransform(transform);
	}
}

//速度取得
Vector3 Component_Rigidbody::GetVelociy()
{
	if (m_RigidBody != nullptr)
	{//ベロシティ取得
		btVector3 velocity = m_RigidBody->body->getLinearVelocity();
		return Vector3{ velocity.x(),velocity.y(),velocity.z() };
	}

	return Vector3 { 0.0f,0.0f,0.0f };
}

//スフィアリジッドボディ追加
void  Component_Rigidbody::AddSphere(const Vector3& _scale, const Vector3& _position, float _mass)
{
	if (m_RigidBody == nullptr)
	{//まだリジッドボディの実体がない時
		if (Manager_Rigidbody* manager_Rigidbody = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Rigidbody())
		{//BulletPhysicsマネージャー取得できたら実行
			m_Scale = _scale;
			m_RigidBody = manager_Rigidbody->AddSphere(_scale, _position, _mass);
		}
	}
}
//ボックスリジッドボディ追加
void  Component_Rigidbody::AddBox(const Vector3& _scale, const Vector3& _position, float _mass)
{
	if (m_RigidBody == nullptr)
	{//まだリジッドボディの実体がない時
		if (Manager_Rigidbody* manager_Rigidbody = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Rigidbody())
		{//BulletPhysicsマネージャー取得できたら実行
			m_Scale = _scale;
			m_RigidBody = manager_Rigidbody->AddBox(_scale, _position, _mass);
		}
	}
}
//カプセルリジッドボディ追加
void  Component_Rigidbody::AddCapsule(float _radius, float _height, const Vector3& _position, const Vector3& _rotation, float _mass)
{
	if (m_RigidBody == nullptr)
	{//まだリジッドボディの実体がない時
		if (Manager_Rigidbody* manager_Rigidbody = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Rigidbody())
		{//BulletPhysicsマネージャー取得できたら実行
			m_Scale = Vector3{ _radius,_height,_radius };
			m_RigidBody = manager_Rigidbody->AddCapsule(_radius, _height, _position, _rotation, _mass);
		}
	}
}
//プレーンリジッドボディ追加
void Component_Rigidbody::AddStaticPlane(const Vector3& _up, const Vector3& _position, float _mass)
{
	if (m_RigidBody == nullptr)
	{//まだリジッドボディの実体がない時
		if (Manager_Rigidbody* manager_Rigidbody = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Rigidbody())
		{//BulletPhysicsマネージャー取得できたら実行
			m_RigidBody = manager_Rigidbody->AddStaticPlane(_up, _position, _mass);
		}
	}
}

void Component_Rigidbody::AddCylinder(const Vector3& _pos, const Vector3& _scale, const Vector3& _rot, float _mass)
{
	if (m_RigidBody == nullptr)
	{//まだリジッドボディの実体がない時
		if (Manager_Rigidbody* manager_Rigidbody = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Rigidbody())
		{//BulletPhysicsマネージャー取得できたら実行
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
