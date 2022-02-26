#pragma once


#include "../Base/ActorRB.h"

class Obj_Cube : public ActorRB
{
protected:

	class Component_OBJ* m_Component_StaticMesh;//FBXコンポーネント

	class Component_OBB* m_Component_OBB;

public:

	virtual void Init()override;//初期化
	virtual void Update()override;//更新


};