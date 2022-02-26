#pragma once


#include "../Base/ActorRB.h"

class Coin : public ActorRB 
{
private:

	//class Component_OBJ* m_Component_Obj;
	class Component_StaticMesh* m_Component_StaticMesh;

public:

	void Init() override;
	void Update() override;

};