//=============================================================================
//
//  Rigidbodyコンポーネント [Component_Rigidbody.h]
//  Date   : 2021/11/22
//
//=============================================================================
#pragma once
#include "Base/Component.h"
#include "../Manager/Manager_Drawer/Manager_Rigidbody.h"

const float ON_GROUND_VELOCITY = 0.1f;//接地判定とする速度

class Component_Rigidbody : public Component
{
private:

	//リジッドボディ
	Manager_Rigidbody::s_RigidBody* m_RigidBody;

	//サイズ
	Vector3 m_Scale;

	//ジャンプ上昇時間
	int m_JumpTime;

public:

	Component_Rigidbody(class Actor* _owner, int _updatePriority) : Component(_owner, _updatePriority) {}

	virtual void Init() override; //初期化
	virtual void Uninit()override;//終了
	virtual void Update()override;//更新

	//座標取得
	Vector3 GetPosition();

	//スケール設定
	void SetScale(const Vector3& _scale);

	//XZ軸移動設定
	void SetMoveXZ(const Vector3& _velocity);

	//XYZ軸移動設定
	void SetMoveXYZ(const Vector3& _velocity);

	//重力設定
	void SetGravity(const Vector3& _velocity);

	//ジャンプ
	void Jump();

	//ワープ
	void SetWarp(const Vector3& _position);

	//速度取得
	Vector3 GetVelociy();

	//スフィアリジッドボディ追加
	void AddSphere(const Vector3& _scale, const Vector3& _position, float _mass);
	//ボックスリジッドボディ追加
	void AddBox(const Vector3& _scale, const Vector3& _position, float _mass);
	//カプセルリジッドボディ追加
	void AddCapsule(float radius, float _height, const Vector3& _position, const Vector3& _rotation, float _mass);
	//プレーンリジッドボディ追加
	void AddStaticPlane(const Vector3& _up, const Vector3& _position, float _mass);

	void AddCylinder(const Vector3& _pos, const Vector3& _scale, const Vector3& _rot, float _mass);

	bool SaveComponentData(const char* _fileName) override;
	bool LoadComponentData(const char* _fileName) override;

	//摩擦設定
	void SetFriction(const float& _friction);
};

