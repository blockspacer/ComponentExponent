//=============================================================================
//
//  キャラクタージャンプ繰り返し状態 [CharacterState_Jump_Loop.h]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "Base/CharacterState.h"

class CharacterState_Jump_Loop : public CharacterState
{

private:

	float m_KeepVelocity;//ジャンプ中の移動速度を保存
	bool  m_Sprint;      //スプリントフラグ

public:

	void Init(Character* _owner)override;
	void Update()override;
	void Uninit()override;

	//スプリント
	void Sprint(bool _input)override;
	//ジャンプはできない
	void Jump()override {};
};