//=============================================================================
//
//  キャラクターアイドル状態 [CharacterState_Idle.h]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "Base/CharacterState.h"

class CharacterState_Idle : public CharacterState
{
public:

	//初期化・更新・終了
	void Init(Character* _owner)override;
	void Update()override;
	void Uninit()override;

	//軸移動
	void MoveAxis(float _forward_value, float _right_value)override;
	//ジャンプ
	void Jump()override;
};