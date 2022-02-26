//=============================================================================
//
//  �L�����N�^�[�W�����v�J�n��� [CharacterState_Jump_Start.cpp]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "CharacterState_Jump_Loop.h"
#include "CharacterState_Jump_Start.h"
#include "../StateMachine_Character.h"
#include "../../Character.h"
#include "../../../../../../../Component/Component_SkeletalMesh.h"

void CharacterState_Jump_Start::Init(Character* _owner)
{
	CharacterState::Init(_owner);

	if (Component_SkeletalMesh* component_SkeletalMesh = m_Owner->GetComponentByClass<Component_SkeletalMesh>())
	{//�u�����h��̃A�j���ݒ�
		component_SkeletalMesh->SetNextAnimationName("Jump_Start");
		component_SkeletalMesh->SetNextAnimationFrame(0);//0�t���[���ڂ���u�����h
	}

}

void CharacterState_Jump_Start::Update()
{
	if (m_Owner != nullptr && m_Component_Rigidbody != nullptr)
	{//�I�[�i�[�ƃ��W�b�h�{�f�B���ǂ�������݂���ꍇ

		if (Component_SkeletalMesh* component_SkeletalMesh = m_Owner->GetComponentByClass<Component_SkeletalMesh>())
		{//�u�����h���̃A�j���ݒ�

			if (component_SkeletalMesh->GetBlendLate() > 0.1f)
			{
				m_Owner->GetStateMachine()->SetState<CharacterState_Jump_Loop>();
			}
		}

	}
}

void CharacterState_Jump_Start::Uninit()
{
	if (Component_SkeletalMesh* component_SkeletalMesh = m_Owner->GetComponentByClass<Component_SkeletalMesh>())
	{//�u�����h���̃A�j���ݒ�
		component_SkeletalMesh->SetCurrentAnimationName("Jump_Start");
	}
}