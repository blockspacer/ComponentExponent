//=============================================================================
//
//  �L�����N�^�[�W�����v�J��Ԃ���� [CharacterState_Jump_Loop.cpp]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "CharacterState_Jump_Loop.h"
#include "CharacterState_Idle.h"
#include "CharacterState_Walk.h"
#include "CharacterState_Sprint.h"
#include "../StateMachine_Character.h"
#include "../../Character.h"
#include "../../../../../../../Component/Component_SkeletalMesh.h"

void CharacterState_Jump_Loop::Init(Character* _owner)
{
	CharacterState::Init(_owner);

	//�X�v�����g�t���O������
	m_Sprint = false;

	if (Component_SkeletalMesh* component_SkeletalMesh = m_Owner->GetComponentByClass<Component_SkeletalMesh>())
	{//�u�����h��̃A�j���ݒ�
		component_SkeletalMesh->SetNextAnimationName("Jump_Loop");
	}

	if (m_Owner != nullptr)
	{//�I�[�i�[�����݂���ꍇ
		//�Œᑬ�x�␳
		(m_KeepVelocity < MAX_WALK_VELOCITY)? m_KeepVelocity = MAX_WALK_VELOCITY : m_KeepVelocity = m_Owner->GetPlayerVelocity();
	}

}

void CharacterState_Jump_Loop::Update()
{
	if (m_Owner != nullptr)
	{//�I�[�i�[�����݂���ꍇ

		//���x�ݒ�(�W�����v�J�n��������)
		m_Owner->SetPlayerVelocity(m_KeepVelocity);

		//�ڒn�𔻒�
		int hit_result = m_Owner->GetComponentByClass<Component_OBB>()->RaycastPM(m_Owner->GetPosition(), m_Owner->GetPosition() - (m_Owner->GetUp() * GROUND_RAY_LENGTH), m_Owner, true,
			std::list<ACTOR_TYPE>(),
			Color(0, 1, 1, 1));//RGBA

		//�ڒn���Ă����瑼�X�e�[�g�ɑJ��
		if (hit_result != INVALID_ID)
		{
			//�����}�e���A���ɉ��������n��
			m_Owner->PlayFootStep(hit_result);

			if (m_Sprint)
			{//�X�v�����g
				m_Owner->GetStateMachine()->SetState<CharacterState_Sprint>();
			}
			else if (m_KeepVelocity >= STOP_VELOCITY)
			{//�E�H�[�N
				m_Owner->GetStateMachine()->SetState<CharacterState_Walk>();
			}
			else
			{//�A�C�h��
				m_Owner->GetStateMachine()->SetState<CharacterState_Idle>();
			}

		}
	}
}

//�X�v�����g
void CharacterState_Jump_Loop::Sprint(bool _input)
{
	m_Sprint = _input;
}

void CharacterState_Jump_Loop::Uninit()
{
	if (Component_SkeletalMesh* component_SkeletalMesh = m_Owner->GetComponentByClass<Component_SkeletalMesh>())
	{//�u�����h���̃A�j���ݒ�
		component_SkeletalMesh->SetCurrentAnimationName("Jump_Loop");
	}
}
