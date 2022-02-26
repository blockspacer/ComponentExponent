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
// �N���X��`
//******************************************************************************
class StateMachine_Character
{
private:

	//���L�L�����N�^�[�X�e�[�g
	std::vector<class CharacterState*> m_CharacterStates;
	//���݂̃L�����N�^�[�̏��
	class CharacterState* m_CharacterState = nullptr;
	//�I�[�i�[
	class Character* m_Owner;

public:

	//������
	void Init(Character* _owner);
	//�I��
	void Uninit();
	//�X�V
	void Update();

	//���ړ�
	void MoveAxis(float _forward_value, float _right_value);
	//�X�v�����g
	void Sprint(bool _input);
	//�W�����v
	void Jump();

//******************************************************************************
// ��Ԑݒ�֐�
//******************************************************************************
	template<typename T>
	inline void SetState()//��Ԑݒ�
	{
		if (m_CharacterState != nullptr)
		{
			m_CharacterState->Uninit();//�I��
		}
		for (CharacterState* state : m_CharacterStates)
		{
			if (typeid(*state) == typeid(T))//�^�𒲂ׂ�(RTTI���I�^���)
			{
				m_CharacterState = state;//�J��
				m_CharacterState->Init(m_Owner);//������
			}
		}
	}
	//��Ԏ擾
	inline CharacterState* GetCharacterState() { return m_CharacterState; }
};

