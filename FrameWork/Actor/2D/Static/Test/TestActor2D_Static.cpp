//=============================================================================
//
//  �e�X�g�p�Œ�2D�A�N�^�[�N���X [TestActor2D_Static.cpp]
//  Date   : 2021/11/3
//
//=============================================================================


#include "TestActor2D_Static.h"
#include "../../../../Component/Component_Sprite.h"
#include "../../../../Manager/Manager_Texture.h"

void  TestActor2D_Static::Load()//���\�[�X�ǂݍ���
{

	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//�e�N�X�`���[�}�l�[�W���[�L���b�V��
		manager_Texture->LoadTexture("Test", "ROM/2D/Test/T_test.png");
	}
		
}
void  TestActor2D_Static::Unload()//���\�[�X�폜
{

	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//�e�N�X�`���[�}�l�[�W���[�L���b�V��
		manager_Texture->UnloadTexture("Test");
	}

}

void  TestActor2D_Static::Init()//������
{
	Actor2D::Init();//������

	m_Position2D = Vector2{-500.0f,-300.0f};//2�������W�����l
	m_Scale2D    = Vector2{ 100.0f, 100.0f };//2�����T�C�Y�����l

	//�X�v���C�g�R���|�[�l���g�ݒ�
	m_Component_Sprite = AddComponent<Component_Sprite>(0);//�ǉ�
	m_Component_Sprite->SetTexture("Test");	//�e�N�X�`���[�ݒ�
}

void TestActor2D_Static::Update()//�X�V
{
	Actor2D::Update();//�X�V
}

void TestActor2D_Static::Uninit()//�I��
{
	Actor2D::Uninit();//�I��
}
