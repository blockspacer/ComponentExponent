////=============================================================================
//
//  StateMachine_Character [StateMachine_Character.cpp].

//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "StateMachine_Character.h"
#include "State/CharacterState_Idle.h"
#include "State/CharacterState_Jump_Start.h"
#include "State/CharacterState_Jump_Loop.h"
#include "State/CharacterState_Fall.h"
#include "State/CharacterState_Walk.h"
#include "State/CharacterState_Sprint.h"

//������
void StateMachine_Character::Init(Character* _owner)
{
	//�I�[�i�[���
	m_Owner = _owner;

	//�X�e�[�g��z��Ɋi�[
	m_CharacterStates.push_back(new CharacterState_Idle);
	m_CharacterStates.push_back(new CharacterState_Jump_Start);
	m_CharacterStates.push_back(new CharacterState_Jump_Loop);
	m_CharacterStates.push_back(new CharacterState_Fall);
	m_CharacterStates.push_back(new CharacterState_Walk);
	m_CharacterStates.push_back(new CharacterState_Sprint);

	//�����X�e�[�g�̓A�C�h��
	SetState<CharacterState_Idle>();

}

//�I��
void StateMachine_Character::Uninit()
{
	//�S�X�e�[�g�폜
	for (CharacterState* state : m_CharacterStates)
	{
		delete state;
		state = nullptr;
	}

	//�v���C���[�X�e�[�g�̃N���A
	m_CharacterStates.clear();

}

//�X�V
void StateMachine_Character::Update()
{
	if (m_CharacterState != nullptr)
	{
		m_CharacterState->Update();//���݂̃X�e�[�g���X�V
	}
}


//���ړ�
void StateMachine_Character::MoveAxis(float _forward_value, float _right_value)
{
	if (m_CharacterState != nullptr)
	{
		m_CharacterState->MoveAxis(_forward_value,_right_value);//���ړ�
	}
}

//�X�v�����g
void StateMachine_Character::Sprint(bool _input)
{
	if (m_CharacterState != nullptr)
	{
		m_CharacterState->Sprint(_input);//�X�v�����g����
	}
}

//�W�����v
void StateMachine_Character::Jump()
{
	if (m_CharacterState != nullptr)
	{
		m_CharacterState->Jump();//�W�����v����
	}
}
