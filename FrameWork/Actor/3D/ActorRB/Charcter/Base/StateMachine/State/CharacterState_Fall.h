//=============================================================================
//
//  �L�����N�^�[������� [CharacterState_Fall.h]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "Base/CharacterState.h"

class CharacterState_Fall : public CharacterState
{
private:

	float m_KeepVelocity;//�W�����v���̈ړ����x��ۑ�
	bool  m_Sprint;      //�X�v�����g�t���O

public:

	void Init(Character* _owner)override;
	void Update()override;
	void Uninit()override;

	//�X�v�����g
	void Sprint(bool _input)override;
	//�W�����v�͂ł��Ȃ�
	void Jump()override {};

};