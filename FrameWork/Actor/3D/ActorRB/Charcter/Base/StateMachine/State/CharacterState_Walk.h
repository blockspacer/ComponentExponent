//=============================================================================
//
//  キャラクター歩き状態 [CharacterState_Walk.h]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "Base/CharacterState.h"

class CharacterState_Walk : public CharacterState
{
private:

	//フレーム
	int m_Frame;

public:

	void Init(Character* _owner)override;
	void Update()override;
	void Uninit()override;

	//軸移動
	void MoveAxis(float _forward_value, float _right_value)override;
	//ジャンプ
	void Jump()override;
	//スプリント
	void Sprint(bool _input)override;

};