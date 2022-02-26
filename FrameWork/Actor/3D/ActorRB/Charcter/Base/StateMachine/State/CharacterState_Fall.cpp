//=============================================================================
//
//  キャラクター落下状態 [CharacterState_Fall.cpp]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "CharacterState_Fall.h"
#include "CharacterState_Idle.h"
#include "CharacterState_Walk.h"
#include "CharacterState_Sprint.h"
#include "../StateMachine_Character.h"
#include "../../Character.h"
#include "../../../../../../../Component/Component_SkeletalMesh.h"

void CharacterState_Fall::Init(Character*  _owner)
{
	CharacterState::Init(_owner);

	//スプリントフラグ初期化
	m_Sprint = false;

	if (Component_SkeletalMesh* component_SkeletalMesh = m_Owner->GetComponentByClass<Component_SkeletalMesh>())
	{//ブレンド先のアニメ設定
		component_SkeletalMesh->SetNextAnimationName("Fall");
	}

	if (m_Owner != nullptr)
	{//オーナーが存在する場合
		//最低速度補正
		(m_KeepVelocity < MAX_WALK_VELOCITY) ? m_KeepVelocity = MAX_WALK_VELOCITY : m_KeepVelocity = m_Owner->GetPlayerVelocity();
	}

}

void CharacterState_Fall::Update()
{
	if (m_Owner != nullptr)
	{//オーナーが存在する場合

		//接地を判定
		int hit_result = m_Owner->GetComponentByClass<Component_OBB>()->RaycastPM(m_Owner->GetPosition(), m_Owner->GetPosition() - (m_Owner->GetUp() * GROUND_RAY_LENGTH), m_Owner, true,
			std::list<ACTOR_TYPE>(),
			Color(0, 1, 1, 1));//RGBA

		//接地していたら他ステートに遷移
		if (hit_result != INVALID_ID)
		{
			//物理マテリアルに応じた着地音
			m_Owner->PlayFootStep(hit_result);

			if (m_Sprint)
			{//スプリント
				m_Owner->GetStateMachine()->SetState<CharacterState_Sprint>();
			}
			else if (m_KeepVelocity >= STOP_VELOCITY)
			{//ウォーク
				m_Owner->GetStateMachine()->SetState<CharacterState_Walk>();
			}
			else
			{//アイドル
				m_Owner->GetStateMachine()->SetState<CharacterState_Idle>();
			}

		}

	}
}

void CharacterState_Fall::Uninit()
{
	if (Component_SkeletalMesh* component_SkeletalMesh = m_Owner->GetComponentByClass<Component_SkeletalMesh>())
	{//ブレンド元のアニメ設定
		component_SkeletalMesh->SetCurrentAnimationName("Fall");
	}
}

//スプリント
void CharacterState_Fall::Sprint(bool _input)
{
	m_Sprint = _input;
}
