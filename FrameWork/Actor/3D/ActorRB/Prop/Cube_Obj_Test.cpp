

#include "../../../../Component/Component_OBJ.h"
#include "../../../../Component/Component_OBB.h"
#include "Cube_Obj_Test.h"

void Obj_Cube::Init()
{
	ActorRB::Init();//������

    //FBX�R���|�[�l���g�ǉ�
	m_Component_StaticMesh = AddComponent<Component_OBJ>(0);
	m_Component_StaticMesh->Load("ROM/3D/Prop/Cube.obj");

	//Rigidbody�R���|�[�l���g�����݂���ꍇ
	if (m_Component_Rigidbody != nullptr)
	{
		m_Component_Rigidbody->AddBox(m_Scale, m_Position, 0);//�ÓI�{�b�N�X�ǉ�
	}

	m_Component_OBB = AddComponent<Component_OBB>(0);
	m_Component_OBB->SetVisibility(true);
	m_Component_OBB->SetScale({ 1,1,1 });
	m_Component_OBB->SetColor(Color(0, 0, 1, 1));
}

void Obj_Cube::Update()
{
	ActorRB::Update();
}
