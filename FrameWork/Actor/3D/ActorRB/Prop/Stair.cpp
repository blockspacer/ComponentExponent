//=============================================================================
//
//  �K�i�N���X [Stair.h]
//  Date   : 2021/12/4
//
//=============================================================================
#include "../../../../Component/Component_StaticMesh.h"
#include "../../../../Component/Component_OBB.h"
#include "Stair.h"


void  Stair::Init()//������
{
	ActorRB::Init();//������

	//�ÓI���b�V���R���|�[�l���g�ǉ�
	m_Component_StaticMesh = AddComponent<Component_StaticMesh>(0);
	m_Component_StaticMesh->LoadStaticMesh("ROM\\3D\\Prop\\SM_Stair_new.fbx", "ROM\\2D\\Test\\T_Sand.png");


	//�ÓI�{�b�N�X���W�b�h�{�f�B�ǉ�
	AddBoxRB(m_Scale, m_Position, 0);

}

void Stair::Update()//�X�V
{
	ActorRB::Update();
}
