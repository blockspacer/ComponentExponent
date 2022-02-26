//=============================================================================
//
//  �L�����N�^�[�A�C�h����� [CharacterState_Idle.cpp]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "CharacterState_Idle.h"
#include "CharacterState_Walk.h"
#include "CharacterState_Jump_Start.h"
#include "CharacterState_Jump_Loop.h"
#include "CharacterState_Fall.h"
#include "../StateMachine_Character.h"
#include "../../Character.h"
#include "../../../../../../../Component/Component_SkeletalMesh.h"

void CharacterState_Idle::Init(Character* _owner)
{
	CharacterState::Init(_owner);

	if (Component_SkeletalMesh* component_SkeletalMesh = m_Owner->GetComponentByClass<Component_SkeletalMesh>())
	{//�u�����h��̃A�j���ݒ�
		component_SkeletalMesh->SetNextAnimationName("Idle");
	}
	if (m_Owner != nullptr)
	{//���x������
		m_Owner->SetPlayerVelocity(STOP_VELOCITY);
	}
}

void CharacterState_Idle::Update()
{
	if (m_Owner != nullptr && m_Component_Rigidbody != nullptr)
	{//�I�[�i�[�ƃ��W�b�h�{�f�B���ǂ�������݂���ꍇ

		//todo���u���[�V����

		//�^���Ƀ��C���΂��Đڒn���Ă��Ȃ������ꍇ�A�W�����v�����ɋ󒆂ɂ��邽�ߗ����ɑJ��
		if (!m_Owner->GetComponentByClass<Component_OBB>()->Raycast(m_Owner->GetPosition(), m_Owner->GetPosition() - (m_Owner->GetUp() * GROUND_RAY_LENGTH), m_Owner, true))
		{
			m_Owner->GetStateMachine()->SetState<CharacterState_Fall>();
		}
	}
}

void CharacterState_Idle::Uninit()
{
	if (Component_SkeletalMesh* component_SkeletalMesh = m_Owner->GetComponentByClass<Component_SkeletalMesh>())
	{//�u�����h���̃A�j���ݒ�
		component_SkeletalMesh->SetCurrentAnimationName("Idle");
	}
}

//�C�x���g�쓮���ړ�
void CharacterState_Idle::MoveAxis(float _forward_value, float _right_value)
{
	if (fabsf(_forward_value) + fabsf(_right_value) >= STOP_VELOCITY)
	{//�����͎�Walk�ɑJ��
		m_Owner->GetStateMachine()->SetState<CharacterState_Walk>();
	}
}

//�C�x���g�쓮�W�����v
void CharacterState_Idle::Jump()
{
	//�W�����v����
	CharacterState::Jump();
	//�W�����v�J�n��ԂɑJ��
	m_Owner->GetStateMachine()->SetState<CharacterState_Jump_Start>();
}
