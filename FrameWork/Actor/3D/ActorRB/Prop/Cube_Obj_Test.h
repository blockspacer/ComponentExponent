#pragma once


#include "../Base/ActorRB.h"

class Obj_Cube : public ActorRB
{
protected:

	class Component_OBJ* m_Component_StaticMesh;//FBX�R���|�[�l���g

	class Component_OBB* m_Component_OBB;

public:

	virtual void Init()override;//������
	virtual void Update()override;//�X�V


};