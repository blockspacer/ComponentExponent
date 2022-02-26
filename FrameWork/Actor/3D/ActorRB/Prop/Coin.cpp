


#include "../../../../Component/Component_OBJ.h"
#include "../../../../Component/Component_StaticMesh.h"
#include "Coin.h"

void Coin::Init()
{
	ActorRB::Init();

	//m_Component_Obj = AddComponent<Component_OBJ>(0);
	//m_Component_Obj->Load("ROM\\3D\\Prop\\untitled.obj");

	m_Component_StaticMesh = AddComponent<Component_StaticMesh>(0);
	//m_Component_StaticMesh->LoadStaticMesh("ROM\\3D\\Prop\\coin_01.fbx","ROM\\2D\\Test\\T_Sand.png");
	m_Component_StaticMesh->LoadStaticMesh("ROM\\3D\\Prop\\ccc.fbx","ROM/2D/Material/T_Gold.png");

	m_Position = { 0,0,0 };
	m_Scale = { 1,0.15f,1 };
	m_Rotation = { 0,0,0 };

	m_Component_OBB = nullptr;

	m_Component_Rigidbody->AddCylinder(m_Position, m_Scale, m_Rotation, 10);
	m_Component_Rigidbody->SetFriction(1.0f);
	//m_Component_Rigidbody->SetGravity({ 0,-100,0 });
}

void Coin::Update()
{
	ActorRB::Update();
}
