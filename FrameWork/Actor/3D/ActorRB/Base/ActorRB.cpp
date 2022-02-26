//=============================================================================
//
//  リジッドボディアクター基底クラス [ActorRB.cpp]
//  Date   : 2021/11/26
//
//=============================================================================

#include "ActorRB.h"


//初期化
void  ActorRB::Init()
{
	//初期化
	Actor3D::Init();

	//Rigidbodyコンポーネント追加
	m_Component_Rigidbody = AddComponent<Component_Rigidbody>(0);

	//OBBコンポーネント追加
	m_Component_OBB = AddComponent<Component_OBB>(0);
	m_Component_OBB->SetVisibility(true);//可視性
	m_Component_OBB->SetScale(Vector3{ 1,1,1 });//スケール設定
}

void ActorRB::Uninit()
{
	Actor3D::Uninit();

	//ポインターを忘れる
	m_Component_OBB = nullptr;
	m_Component_Rigidbody = nullptr;
}

//スフィアリジッドボディ追加
void ActorRB::AddSphereRB(const Vector3& _scale, const Vector3& _position, float _mass)
{
	if (m_Component_Rigidbody != nullptr)
	{//リジッドボディの実態がある場合
		m_Component_Rigidbody->AddSphere(HalfSize(_scale), _position, _mass);
	}
	if (m_Component_OBB != nullptr)
	{//OBBコンポーネントの実態があるなら
		m_Component_OBB->SetScale(_scale);//スケール設定
	}
}
//ボックスリジッドボディ追加
void ActorRB::AddBoxRB(const Vector3& _scale, const Vector3& _position, float _mass)
{
	if (m_Component_Rigidbody != nullptr)
	{//リジッドボディの実態がある場合
		m_Component_Rigidbody->AddBox(HalfSize(_scale), _position, _mass);
	}
	if (m_Component_OBB != nullptr)
	{//OBBコンポーネントの実態があるなら
		m_Component_OBB->SetScale(_scale);//スケール設定
	}
}
//カプセルリジッドボディ追加
void ActorRB::AddCapsuleRB(float _radius, float _height, const Vector3& _position, const Vector3& _rotation, float _mass)
{
	if (m_Component_Rigidbody != nullptr)
	{//リジッドボディの実態がある場合
		m_Component_Rigidbody->AddCapsule(HalfSize(_radius), HalfSize(_height),_position,_rotation,_mass);
	}
	if (m_Component_OBB != nullptr)
	{//OBBコンポーネントの実態があるなら
		m_Component_OBB->SetScale(Vector3{ _radius,_height,_radius });//スケール設定
	}
}

void ActorRB::AddCylinderRB(const Vector3& _pos, const Vector3& _scale, const Vector3& _rot, float _mass)
{
	if (m_Component_Rigidbody != nullptr)
	{//リジッドボディの実態がある場合
		m_Component_Rigidbody->AddCylinder(_pos, _scale, _rot, _mass);
	}
}

//Rigidbody座標設定
void ActorRB::SetRigidbodyPostion(Vector3 _position)
{
	if (m_Component_Rigidbody != nullptr)
	{//リジッドボディの実態がある場合
		m_Component_Rigidbody->SetWarp(_position);
	}
}

//Rigidbodyスケール設定
void ActorRB::SetRigidbodyScale(Vector3 _scale)
{
	//基礎スケールを設定
	m_Scale = _scale;

	if (m_Component_Rigidbody != nullptr)
	{//リジッドボディの実態がある場合
		m_Component_Rigidbody->SetScale(_scale);
	}
	if (m_Component_OBB != nullptr)
	{//OBBコンポーネントの実態があるなら
		m_Component_OBB->SetScale(_scale);
	}
}



