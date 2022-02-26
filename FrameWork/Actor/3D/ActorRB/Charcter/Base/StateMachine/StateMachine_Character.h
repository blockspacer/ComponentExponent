//=============================================================================
//
//  StateMachine_Character [StateMachine_Character.h]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include <vector>
#include <typeinfo>
#include "State/Base/CharacterState.h"


//******************************************************************************
// クラス定義
//******************************************************************************
class StateMachine_Character
{
private:

	//所有キャラクターステート
	std::vector<class CharacterState*> m_CharacterStates;
	//現在のキャラクターの状態
	class CharacterState* m_CharacterState = nullptr;
	//オーナー
	class Character* m_Owner;

public:

	//初期化
	void Init(Character* _owner);
	//終了
	void Uninit();
	//更新
	void Update();

	//軸移動
	void MoveAxis(float _forward_value, float _right_value);
	//スプリント
	void Sprint(bool _input);
	//ジャンプ
	void Jump();

//******************************************************************************
// 状態設定関数
//******************************************************************************
	template<typename T>
	inline void SetState()//状態設定
	{
		if (m_CharacterState != nullptr)
		{
			m_CharacterState->Uninit();//終了
		}
		for (CharacterState* state : m_CharacterStates)
		{
			if (typeid(*state) == typeid(T))//型を調べる(RTTI動的型情報)
			{
				m_CharacterState = state;//遷移
				m_CharacterState->Init(m_Owner);//初期化
			}
		}
	}
	//状態取得
	inline CharacterState* GetCharacterState() { return m_CharacterState; }
};

