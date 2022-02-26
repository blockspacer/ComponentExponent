
#include "../../../Component/Component_StaticMesh.h"
#include "../../../Component/Component_OBB.h"
#include "ItemSphere.h"

void ItemSphere::Init()
{
	m_Component_StaticMesh = AddComponent<Component_StaticMesh>(0);
	m_Component_StaticMesh->LoadStaticMesh("ROM/3D/Prop/box_01.fbx", "ROM/2D/Material/T_Gold.png");

	m_Position = Vector3(0, 0, 0);
	m_Scale = Vector3(0.25f, 0.25f, 0.25f);
	m_Rotation = Vector3(0, 0, 0);

	SetTag("item");

	m_Component_OBB = AddComponent<Component_OBB>(0);
	m_Component_OBB->SetVisibility(true);
	m_Component_OBB->SetScale(m_Scale);
	m_Component_OBB->SetIsReactRay(true);
}

void ItemSphere::Update()
{
	Actor3D::Update();
}

void ItemSphere::Uninit()
{
	Actor3D::Uninit();
}
