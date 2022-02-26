//=============================================================================
//
//  �L���[�u�N���X [Cube.cpp]
//  Date   : 2021/11/4
//
//=============================================================================

#include "Cube.h"
#include "../../../../Component/Component_StaticMesh.h"
#include "../../../../Component/Component_OBB.h"


void  Cube::Init()//������
{
	ActorRB::Init();//������

	//�ÓI���b�V���R���|�[�l���g�ǉ�
	m_Component_StaticMesh = AddComponent<Component_StaticMesh>(0);
	m_Component_StaticMesh->LoadStaticMesh("ROM\\3D\\Test\\box.fbx", texName ? texName : "ROM\\2D\\Material\\T_Sand.png");
	m_Component_StaticMesh->SetScaleRate(0.5f);

	//�ÓI�{�b�N�X���W�b�h�{�f�B�ǉ�
    AddBoxRB(m_Scale, m_Position, 10000);
}

void Cube::Uninit()
{
	ActorRB::Uninit();
}


