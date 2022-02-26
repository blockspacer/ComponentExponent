
#include "../../../../Component/Component_StaticMesh.h"
#include "Move_Cube.h"

void Move_Cube::Init()
{
	Cube::Init();

	speed = 0.0f;
	moveSize = 10000;

	m_Component_Rigidbody->SetFriction(1.0f);
}

void Move_Cube::Update()
{
	speed += 0.01f;
	Vector3 temp = {0,0,10000};
	m_Component_Rigidbody->SetMoveXZ({0,0,cos(speed) * moveSize});

	Cube::Update();
}
