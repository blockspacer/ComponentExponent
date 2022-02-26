//=============================================================================
//
//  3Dアクタークラス [Actor3D.cpp]
//  Date   : 2021/11/5
//
//=============================================================================


#include "Actor3D.h"


void  Actor3D::Init()//初期化
{
	m_Position       = Vector3_Zero;//3次元座標初期値
	m_Rotation       = Vector3_Zero;//3次元回転初期値
	m_Scale          = Vector3{ 1.0f,1.0f,1.0f };//3次元サイズ初期値
}

void Actor3D::Update()//更新
{
	//Yaw軸正規化
	if (m_Rotation.y < DegToRad(-360.0f))
	{
		m_Rotation.y += DegToRad(360.0f);

	}
	else if (m_Rotation.y > DegToRad(360.0f))
	{
		m_Rotation.y -= DegToRad(360.0f);
	}

	Actor::Update();//更新

}

void Actor3D::Uninit()//終了
{
	Actor::Uninit();//終了
}
