

#include "../../../../Component/Component_StaticMesh.h"
#include "../../../../Component/Component_Rigidbody.h"
#include "Cube_NongravityMass.h"

void Cube_NonGravityMass::Init()
{
	ActorRB::Init();

	m_Component_StaticMesh = AddComponent<Component_StaticMesh>(0);
	m_Component_StaticMesh->LoadStaticMesh("ROM\\3D\\Test\\box.fbx", "ROM\\2D\\Material\\T_Hex.png");
	m_Component_StaticMesh->SetScaleRate(0.5f);

	AddBoxRB({1,1,1}, {0,0,0}, 10000);
	m_Component_Rigidbody->SetGravity({ 0,0,0 });
	m_Component_Rigidbody->SetFriction(1.0f);

}

void Cube_NonGravityMass::Update()
{
	ActorRB::Update();
}
