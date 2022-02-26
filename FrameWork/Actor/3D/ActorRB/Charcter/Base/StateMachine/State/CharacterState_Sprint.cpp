//=============================================================================
//
//  �L�����N�^�[������ [CharacterState_Run.cpp]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "CharacterState_Sprint.h"
#include "CharacterState_Idle.h"
#include "CharacterState_Walk.h"
#include "CharacterState_Fall.h"
#include "CharacterState_Jump_Start.h"
#include "../StateMachine_Character.h"
#include "../../Character.h"
#include "../../../../../../../Component/Component_SkeletalMesh.h"

constexpr int KEY_FRAME   =  5;//�L�[�t���[��
constexpr int FRAME_RESET = 15;//�t���[�����Z�b�g����

void CharacterState_Sprint::Init(Character* _owner)
{
	CharacterState::Init(_owner);

	//�t���[��������
	m_Frame = 0;

	if (Component_SkeletalMesh* component_SkeletalMesh = m_Owner->GetComponentByClass<Component_SkeletalMesh>())
	{//�u�����h��̃A�j���ݒ�
		component_SkeletalMesh->SetNextAnimationName("Run");
	}
}


void CharacterState_Sprint::Update()
{
	if (m_Owner != nullptr)
	{
		//���x�ݒ�(�����ȏ㑖�薢���ɃN�����v)
		m_Owner->SetPlayerVelocity(Math::clamp(m_Owner->GetPlayerVelocity() + ONE_FRAME_UP_VELOCITY, MAX_WALK_VELOCITY, MAX_SPRINT_VELOCITY));

		//�ڒn�𔻒�
		int hit_result = m_Owner->GetComponentByClass<Component_OBB>()->RaycastPM(m_Owner->GetPosition(), m_Owner->GetPosition() - (m_Owner->GetUp() * GROUND_RAY_LENGTH), m_Owner, true);

		//�^���Ƀ��C���΂��Đڒn���Ă��Ȃ������ꍇ�A�W�����v�����ɋ󒆂ɂ��邽�ߗ����ɑJ��
		if (hit_result == INVALID_ID)
		{
			m_Owner->GetStateMachine()->SetState<CharacterState_Fall>();

		}

		//�t���[�����Z
		m_Frame++;

		//�L�[�t���[���C�x���g
		if (m_Frame == KEY_FRAME)
		{
			//�����}�e���A���ɉ���������
			m_Owner->PlayFootStep(hit_result);
		}
		else if (m_Frame == FRAME_RESET)
		{
			//�t���[��������
			m_Frame = 0;
		}

	}
}

void CharacterState_Sprint::Uninit()
{
	if (Component_SkeletalMesh* component_SkeletalMesh = m_Owner->GetComponentByClass<Component_SkeletalMesh>())
	{//�u�����h���̃A�j���ݒ�
		component_SkeletalMesh->SetCurrentAnimationName("Run");
	}
}

//�C�x���g�쓮���ړ�
void CharacterState_Sprint::MoveAxis(float _forward_value, float _right_value)
{
	if (fabsf(_forward_value) + fabsf(_right_value) <= STOP_VELOCITY)
	{//�����͂��Ȃ��ꍇ�AIdle�ɑJ��
		m_Owner->GetStateMachine()->SetState<CharacterState_Idle>();
	}
	else
	{
		//���ړ�
		CharacterState::MoveAxis(_forward_value, _right_value);
	}
}

//�X�v�����g
void  CharacterState_Sprint::Sprint(bool _input)
{
	if (!_input)
	{
		m_Owner->GetStateMachine()->SetState<CharacterState_Walk>();
	}
}

//�C�x���g�쓮�W�����v
void CharacterState_Sprint::Jump()
{
	//�W�����v����
	CharacterState::Jump();
	//�W�����v�J�n��ԂɑJ��
	m_Owner->GetStateMachine()->SetState<CharacterState_Jump_Start>();
}