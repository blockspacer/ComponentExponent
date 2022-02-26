//=============================================================================
//
//  キャラクターアイドル状態 [CharacterState_Idle.cpp]
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
	{//ブレンド先のアニメ設定
		component_SkeletalMesh->SetNextAnimationName("Idle");
	}
	if (m_Owner != nullptr)
	{//速度初期化
		m_Owner->SetPlayerVelocity(STOP_VELOCITY);
	}
}

void CharacterState_Idle::Update()
{
	if (m_Owner != nullptr && m_Component_Rigidbody != nullptr)
	{//オーナーとリジッドボディがどちらも存在する場合

		//todo放置モーション

		//真下にレイを飛ばして接地していなかった場合、ジャンプせずに空中にいるため落下に遷移
		if (!m_Owner->GetComponentByClass<Component_OBB>()->Raycast(m_Owner->GetPosition(), m_Owner->GetPosition() - (m_Owner->GetUp() * GROUND_RAY_LENGTH), m_Owner, true))
		{
			m_Owner->GetStateMachine()->SetState<CharacterState_Fall>();
		}
	}
}

void CharacterState_Idle::Uninit()
{
	if (Component_SkeletalMesh* component_SkeletalMesh = m_Owner->GetComponentByClass<Component_SkeletalMesh>())
	{//ブレンド元のアニメ設定
		component_SkeletalMesh->SetCurrentAnimationName("Idle");
	}
}

//イベント駆動軸移動
void CharacterState_Idle::MoveAxis(float _forward_value, float _right_value)
{
	if (fabsf(_forward_value) + fabsf(_right_value) >= STOP_VELOCITY)
	{//軸入力時Walkに遷移
		m_Owner->GetStateMachine()->SetState<CharacterState_Walk>();
	}
}

//イベント駆動ジャンプ
void CharacterState_Idle::Jump()
{
	//ジャンプ処理
	CharacterState::Jump();
	//ジャンプ開始状態に遷移
	m_Owner->GetStateMachine()->SetState<CharacterState_Jump_Start>();
}
