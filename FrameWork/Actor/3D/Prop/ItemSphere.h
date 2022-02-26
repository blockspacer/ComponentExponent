#pragma once

#include "../Base/Actor3D.h"

class ItemSphere : public Actor3D
{
private:

	class Component_StaticMesh* m_Component_StaticMesh;
	class Component_OBB* m_Component_OBB;

public:

	void Init() override;
	void Update() override;
	void Uninit() override;

};