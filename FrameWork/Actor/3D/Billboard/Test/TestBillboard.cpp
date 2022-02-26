//=============================================================================
//
//  �e�X�g�p�r���{�[�h�N���X [TestBillboard.cpp]
//  Date   : 2021/11/3
//
//=============================================================================


#include "TestBillboard.h"
#include "../../../../Manager/Manager_Texture.h"
#include "../../../../Component/Component_Billboard.h"
#include "../../../../Manager/Manager_Drawer/Manager_ImGui.h"

void  TestBillboard::Load()//���\�[�X�ǂݍ���
{

	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//�e�N�X�`���[�}�l�[�W���[�L���b�V��
		manager_Texture->LoadTexture("AnimTest", "ROM/2D/Test/T_AnimTest.png");
	}
}
void  TestBillboard::Unload()//���\�[�X�폜
{
	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//�e�N�X�`���[�}�l�[�W���[�L���b�V��
		manager_Texture->UnloadTexture("AnimTest");
	}
}

void  TestBillboard::Init()//������
{
	Actor3D::Init();//������

	m_Position = Vector3{ 0.0f,0.0f,1.0f };

	//�r���{�[�h�R���|�[�l���g�ǉ�
	m_Component_Billboard = AddComponent<Component_Billboard>(0);//�ǉ�
	m_Component_Billboard->SetTexture("AnimTest");//�e�N�X�`���[�ݒ�
	m_Component_Billboard->CalculationOneFrameSize(3,3);//3*3�̃A�j���[�V����
	m_Component_Billboard->SetMaxAnimCount(6);//6�t���[���ōX�V

}

void TestBillboard::Update()//�X�V
{
	Actor3D::Update();//�X�V
}

void TestBillboard::Uninit()//�I��
{
	Actor3D::Uninit();//�I��
}
