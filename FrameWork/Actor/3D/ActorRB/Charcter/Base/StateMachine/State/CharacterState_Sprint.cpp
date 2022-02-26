//=============================================================================
//
//  キャラクター走り状態 [CharacterState_Run.cpp]
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

constexpr int KEY_FRAME   =  5;//キーフレーム
constexpr int FRAME_RESET = 15;//フレームリセット時間

void CharacterState_Sprint::Init(Character* _owner)
{
	CharacterState::Init(_owner);

	//フレーム初期化
	m_Frame = 0;

	if (Component_SkeletalMesh* component_SkeletalMesh = m_Owner->GetComponentByClass<Component_SkeletalMesh>())
	{//ブレンド先のアニメ設定
		component_SkeletalMesh->SetNextAnimationName("Run");
	}
}


void CharacterState_Sprint::Update()
{
	if (m_Owner != nullptr)
	{
		//速度設定(歩き以上走り未満にクランプ)
		m_Owner->SetPlayerVelocity(Math::clamp(m_Owner->GetPlayerVelocity() + ONE_FRAME_UP_VELOCITY, MAX_WALK_VELOCITY, MAX_SPRINT_VELOCITY));

		//接地を判定
		int hit_result = m_Owner->GetComponentByClass<Component_OBB>()->RaycastPM(m_Owner->GetPosition(), m_Owner->GetPosition() - (m_Owner->GetUp() * GROUND_RAY_LENGTH), m_Owner, true);

		//真下にレイを飛ばして接地していなかった場合、ジャンプせずに空中にいるため落下に遷移
		if (hit_result == INVALID_ID)
		{
			m_Owner->GetStateMachine()->SetState<CharacterState_Fall>();

		}

		//フレーム加算
		m_Frame++;

		//キーフレームイベント
		if (m_Frame == KEY_FRAME)
		{
			//物理マテリアルに応じた足音
			m_Owner->PlayFootStep(hit_result);
		}
		else if (m_Frame == FRAME_RESET)
		{
			//フレーム初期化
			m_Frame = 0;
		}

	}
}

void CharacterState_Sprint::Uninit()
{
	if (Component_SkeletalMesh* component_SkeletalMesh = m_Owner->GetComponentByClass<Component_SkeletalMesh>())
	{//ブレンド元のアニメ設定
		component_SkeletalMesh->SetCurrentAnimationName("Run");
	}
}

//イベント駆動軸移動
void CharacterState_Sprint::MoveAxis(float _forward_value, float _right_value)
{
	if (fabsf(_forward_value) + fabsf(_right_value) <= STOP_VELOCITY)
	{//軸入力がない場合、Idleに遷移
		m_Owner->GetStateMachine()->SetState<CharacterState_Idle>();
	}
	else
	{
		//軸移動
		CharacterState::MoveAxis(_forward_value, _right_value);
	}
}

//スプリント
void  CharacterState_Sprint::Sprint(bool _input)
{
	if (!_input)
	{
		m_Owner->GetStateMachine()->SetState<CharacterState_Walk>();
	}
}

//イベント駆動ジャンプ
void CharacterState_Sprint::Jump()
{
	//ジャンプ処理
	CharacterState::Jump();
	//ジャンプ開始状態に遷移
	m_Owner->GetStateMachine()->SetState<CharacterState_Jump_Start>();
}