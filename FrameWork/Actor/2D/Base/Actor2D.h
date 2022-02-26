//=============================================================================
//
//  2Dアクタークラス [Actor2D.h]
//  Date   : 2021/11/1
//
//=============================================================================
#pragma once


#include "../../Base/Actor.h"

class Actor2D : public Actor
{
protected:

	Vector2 m_Position2D;//2次元座標
	Vector2 m_Scale2D;  //2次元スケール

public:

	virtual void Init()override;//初期化
	virtual void Uninit()override;//終了
	virtual void Update()override;//更新

	inline Vector2 GetPosition2D() const { return m_Position2D;}//2次元座標取得
	inline Vector2 GetScale2D()    const { return m_Scale2D; }  //2次元スケール取得

	inline void SetPosition2D(Vector2 _position3D) { m_Position2D  = _position3D; }//2次元座標設定
	inline void SetScale2D   (Vector2 _scale2D)     { m_Scale2D    = _scale2D; }   //2次元スケール設定

};