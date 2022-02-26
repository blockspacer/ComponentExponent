//=============================================================================
//
//  リジッドボディアクター基底クラス [ActorRB.h]
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

	class Component_Rigidbody*     m_Component_Rigidbody;    //Rigidbodyコンポーネント
	class Component_OBB*           m_Component_OBB;          //OBBコンポーネント

public:

	ActorRB():mass(0) {}
	ActorRB(int _mass) :mass(_mass) {}
	virtual ~ActorRB() {}

	virtual void Init()override;//初期化
	void Uninit() override;

	void SetRigidbodyPostion(Vector3 _position);//Rigidbody座標設定
	void SetRigidbodyScale  (Vector3 _scale);   //Rigidbodyスケール設定

	//スフィアリジッドボディ追加
	void AddSphereRB(const Vector3& _scale, const Vector3& _position, float _mass);
	//ボックスリジッドボディ追加
	void AddBoxRB(const Vector3& _scale, const Vector3& _position, float _mass);
	//カプセルリジッドボディ追加
	void AddCapsuleRB(float _radius, float _height, const Vector3& _position, const Vector3& _rotation, float _mass);

	void AddCylinderRB(const Vector3& _pos, const Vector3& _scale, const Vector3& _rot, float _mass);
};