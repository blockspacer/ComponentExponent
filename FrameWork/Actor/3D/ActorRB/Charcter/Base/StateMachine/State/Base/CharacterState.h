//=============================================================================
//
//  キャラクターステート基底 [CharacterState.h]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "../../../Character.h"
#include "../../../../../../../../Manager/Manager_Drawer/Manager_Font2D.h"

constexpr float STOP_VELOCITY = 0.25f;//停止判定
constexpr float ONE_FRAME_UP_VELOCITY = 5.0f;//1フレームに加算される加速度
constexpr float GROUND_RAY_LENGTH = 0.45f;//接地判定に用いるレイの長さ

class CharacterState
{
public:

	//初期化・更新・終了
	virtual void Init(Character* _owner);
	virtual void Update() = 0;
	virtual void Uninit() = 0;

	//軸移動
	virtual void MoveAxis(float _forward_value, float _right_value);

	//ジャンプ
	virtual void Jump();

	//スプリント
	virtual void Sprint(bool _input) {};

protected:

	//このステートを所持しているキャラクター
	class Character*           m_Owner;
	//オーナーのリジッドボディキャッシュ
	class Component_Rigidbody* m_Component_Rigidbody;

};