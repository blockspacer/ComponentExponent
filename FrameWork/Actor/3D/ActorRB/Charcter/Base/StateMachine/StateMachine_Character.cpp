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

//初期化
void StateMachine_Character::Init(Character* _owner)
{
	//オーナー代入
	m_Owner = _owner;

	//ステートを配列に格納
	m_CharacterStates.push_back(new CharacterState_Idle);
	m_CharacterStates.push_back(new CharacterState_Jump_Start);
	m_CharacterStates.push_back(new CharacterState_Jump_Loop);
	m_CharacterStates.push_back(new CharacterState_Fall);
	m_CharacterStates.push_back(new CharacterState_Walk);
	m_CharacterStates.push_back(new CharacterState_Sprint);

	//初期ステートはアイドル
	SetState<CharacterState_Idle>();

}

//終了
void StateMachine_Character::Uninit()
{
	//全ステート削除
	for (CharacterState* state : m_CharacterStates)
	{
		delete state;
		state = nullptr;
	}

	//プレイヤーステートのクリア
	m_CharacterStates.clear();

}

//更新
void StateMachine_Character::Update()
{
	if (m_CharacterState != nullptr)
	{
		m_CharacterState->Update();//現在のステートを更新
	}
}


//軸移動
void StateMachine_Character::MoveAxis(float _forward_value, float _right_value)
{
	if (m_CharacterState != nullptr)
	{
		m_CharacterState->MoveAxis(_forward_value,_right_value);//軸移動
	}
}

//スプリント
void StateMachine_Character::Sprint(bool _input)
{
	if (m_CharacterState != nullptr)
	{
		m_CharacterState->Sprint(_input);//スプリント処理
	}
}

//ジャンプ
void StateMachine_Character::Jump()
{
	if (m_CharacterState != nullptr)
	{
		m_CharacterState->Jump();//ジャンプ処理
	}
}
