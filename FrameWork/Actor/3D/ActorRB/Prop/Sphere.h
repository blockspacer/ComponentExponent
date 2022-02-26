//=============================================================================
//
//  ���̃N���X [Sphere.h]
//  Date   : 2021/1/10
//
//=============================================================================
#pragma once


#include "../Base/ActorRB.h"

class Sphere : public ActorRB
{
protected:

	class Component_StaticMesh*           m_Component_StaticMesh;          //�ÓI���b�V���R���|�[�l���g

	class Component_OBB* m_Component_OBB;

	int m_LifeTime = 0;

public:

	virtual void Init()override;//������
	virtual void Update()override;//�X�V


};