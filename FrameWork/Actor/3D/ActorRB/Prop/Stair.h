//=============================================================================
//
//  �K�i�N���X [Stair.h]
//  Date   : 2021/12/4
//
//=============================================================================
#pragma once


#include "../Base/ActorRB.h"

class Stair : public ActorRB
{
protected:

	class Component_StaticMesh*           m_Component_StaticMesh;          //�ÓI���b�V���R���|�[�l���g

	class Component_OBB* m_Component_OBB;

public:

	virtual void Init()override;//������
	virtual void Update()override;//�X�V


};