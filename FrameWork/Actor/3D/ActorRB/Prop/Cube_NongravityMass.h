#pragma once

#include "../../ActorRB/Base/ActorRB.h"

class Cube_NonGravityMass : public ActorRB
{
private:

	class Component_StaticMesh* m_Component_StaticMesh;//�ÓI���b�V���R���|�[�l���g

public:

	void Init() override;
	void Update() override;
};