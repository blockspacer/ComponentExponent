//=============================================================================
//
//  �L�����N�^�[�W�����v�J�n��� [CharacterState_Jump_Start.h]
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

	//�W�����v�͂ł��Ȃ�
	void Jump()override {};

};