//=============================================================================
//
//  2Dアクタークラス [Actor2D.cpp]
//  Date   : 2021/11/1
//
//=============================================================================


#include "Actor2D.h"


void  Actor2D::Init()//初期化
{
	m_Position2D       = Vector2_Zero;//2次元座標初期値
	m_Scale2D          = Vector2_Zero;//2次元サイズ初期値
}

void Actor2D::Update()//更新
{
	Actor::Update();//更新
}

void Actor2D::Uninit()//終了
{
	Actor::Uninit();//終了
}
