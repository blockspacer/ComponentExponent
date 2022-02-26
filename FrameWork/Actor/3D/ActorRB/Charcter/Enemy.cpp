//=============================================================================
//
//  エネミークラス [Enemy.cpp]
//  Date   : 2021/11/4
//
//=============================================================================


#include "Enemy.h"
#include "../../../../Component/Component_SkeletalMesh.h"
#include "../../../../Component/Component_OBB.h"
#include "../../../../Component/Component_Rigidbody.h"

static bool a = false;
static int b = 0;


void  Enemy::Init()//初期化
{
	Character::Init();//初期化

	m_Position = Vector3{ 20.0f,1.0f,1.0f };//3次元座標初期値
	m_Rotation = Vector3{ DegToRad(0.0f),DegToRad(0.0f),DegToRad(0.0f) };//3次元回転初期値
	m_Scale    = Vector3{ 1.0f,1.0f,1.0f };//3次元サイズ初期値

	m_Move = false;
}
void Enemy::Update()//更新
{
	Vector3 Forward = GetForward();//フォワード取得
	Vector3 Right = GetRight(); //ライト取得

	b++;
	if (b > 360)
	{
		b = 0;
		a = !a;

	}
	if (m_Move)
	{
		if (a)//前方向移動
		{
			m_Component_Rigidbody->SetMoveXZ(-GetForward() * m_PlayerVelocity);
		}
		else
		{
			m_Component_Rigidbody->SetMoveXZ(GetForward() * m_PlayerVelocity);
		}
	}

	Character::Update();
}

