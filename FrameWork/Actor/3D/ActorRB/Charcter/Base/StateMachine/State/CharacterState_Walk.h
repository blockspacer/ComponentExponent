//=============================================================================
//
//  �L�����N�^�[������� [CharacterState_Walk.h]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "Base/CharacterState.h"

class CharacterState_Walk : public CharacterState
{
private:

	//�t���[��
	int m_Frame;

public:

	void Init(Character* _owner)override;
	void Update()override;
	void Uninit()override;

	//���ړ�
	void MoveAxis(float _forward_value, float _right_value)override;
	//�W�����v
	void Jump()override;
	//�X�v�����g
	void Sprint(bool _input)override;

};