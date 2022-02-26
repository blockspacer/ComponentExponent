#pragma once

#include "../../ActorRB/Base/ActorRB.h"

class Treasure : public ActorRB
{
private:

	class Component_StaticMesh* m_Component_StaticMesh;

public:

	void Init() override;
};