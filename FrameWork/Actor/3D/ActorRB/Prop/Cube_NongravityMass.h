#pragma once

#include "../../ActorRB/Base/ActorRB.h"

class Cube_NonGravityMass : public ActorRB
{
private:

	class Component_StaticMesh* m_Component_StaticMesh;//静的メッシュコンポーネント

public:

	void Init() override;
	void Update() override;
};