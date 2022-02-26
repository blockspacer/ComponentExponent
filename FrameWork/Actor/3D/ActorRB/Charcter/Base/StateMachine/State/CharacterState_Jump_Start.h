//=============================================================================
//
//  キャラクタージャンプ開始状態 [CharacterState_Jump_Start.h]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "Base/CharacterState.h"

class CharacterState_Jump_Start : public CharacterState
{
public:

	void Init(Character* _owner)override;
	void Update()override;
	void Uninit()override;

	//ジャンプはできない
	void Jump()override {};

};