//=============================================================================
//
//  �e�N���X [Bullet.h]
//  Date   : 2021/11/4
//
//=============================================================================
#pragma once


#include "../Base/Actor3D.h"


class Bullet : public Actor3D
{
protected:

	class Component_OBB*     m_Component_OBB;    //OBB�R���|�[�l���g

public:

	virtual void Init()override;//������
	virtual void Update()override;//�X�V

};