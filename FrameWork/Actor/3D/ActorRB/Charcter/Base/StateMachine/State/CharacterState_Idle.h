//=============================================================================
//
//  �L�����N�^�[�A�C�h����� [CharacterState_Idle.h]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "Base/CharacterState.h"

class CharacterState_Idle : public CharacterState
{
public:

	//�������E�X�V�E�I��
	void Init(Character* _owner)override;
	void Update()override;
	void Uninit()override;

	//���ړ�
	void MoveAxis(float _forward_value, float _right_value)override;
	//�W�����v
	void Jump()override;
};