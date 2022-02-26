
#include "../../../../Component/Component_StaticMesh.h"
#include "Treasure.h"

void Treasure::Init()
{
	ActorRB::Init();

	m_Component_StaticMesh = AddComponent<Component_StaticMesh>(0);
	m_Component_StaticMesh->LoadStaticMesh("ROM/3D/Character/Jean/SK_Jean.fbx");

	//AddCapsuleRB(0.5f, 1.0f, { 0,0,0 }, { 0,0,0 }, 30);

	AddBoxRB({ 1,1,1 }, { 0,0,0 }, 100);
	m_Component_Rigidbody->SetFriction(1.0f);
}
