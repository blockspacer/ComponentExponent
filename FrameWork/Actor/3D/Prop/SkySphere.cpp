//=============================================================================
//
//  SkySphere�N���X [SkySphere.cpp]
//  Date   : 2021/1/16
//
//=============================================================================

#include "SkySphere.h"
#include "../../../Component/Component_SkeletalMesh.h"
#include "../../../Component/Component_OBB.h"


constexpr float PLAYER_VELOCITY = 0.15f;


void  SkySphere::Init()//������
{
	Actor3D::Init();//������

	//�ÓI���b�V���R���|�[�l���g�ǉ�
	m_Component_StaticMesh = AddComponent<Component_StaticMesh>(0);
	m_Component_StaticMesh->LoadStaticMesh("ROM\\3D\\Prop\\night_test_03.fbx", "ROM\\3D\\Prop\\T_Stars4.png");
	m_Component_StaticMesh->SetScaleRate(250.0f);

}

void SkySphere::Update()//�X�V
{
	Actor3D::Update();
}

