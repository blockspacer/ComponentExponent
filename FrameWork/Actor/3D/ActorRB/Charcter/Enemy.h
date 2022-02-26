//=============================================================================
//
//  �G�l�~�[�N���X [Enemy.h]
//  Date   : 2021/11/4
//
//=============================================================================
#pragma once


#include "Base/Character.h"


class Enemy : public Character
{
private:

	bool m_Move;

public:

	virtual void Init()override;//������
	virtual void Update()override;//�X�V

	inline void SetMove(bool _move) { m_Move = _move; }
};