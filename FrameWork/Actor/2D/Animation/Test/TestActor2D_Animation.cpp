//=============================================================================
//
//  �e�X�g�p�A�j���[�V����2D�A�N�^�[�N���X [TestActor2D_Animation.cpp]
//  Date   : 2021/11/3
//
//=============================================================================


#include "TestActor2D_Animation.h"
#include "../../../../Manager/Manager_Texture.h"
#include "../../../../Component/Component_SpriteAnimation.h"

void  TestActor2D_Animation::Load()//���\�[�X�ǂݍ���
{

	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//�e�N�X�`���[�}�l�[�W���[�L���b�V��
		manager_Texture->LoadTexture("AnimTest", "ROM/2D/Title/logo_03.png");
	}
		
}
void  TestActor2D_Animation::Unload()//���\�[�X�폜
{

	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//�e�N�X�`���[�}�l�[�W���[�L���b�V��
		manager_Texture->UnloadTexture("AnimTest");
	}

}

void  TestActor2D_Animation::Init()//������
{
	Actor2D::Init();//������

	m_Position2D = Vector2{0.0f,0.0f};//2�������W�����l
	m_Scale2D    = Vector2{1920.0f,1080.0f};//2�����T�C�Y�����l
	//todo:�X�P�[��

	//�A�j���[�V�����X�v���C�g�R���|�[�l���g�ݒ�
	m_Component_SpriteAnimation = AddComponent<Component_SpriteAnimation>(0);//�ǉ�
	m_Component_SpriteAnimation->SetTexture("AnimTest");//�e�N�X�`���[�ݒ�
	m_Component_SpriteAnimation->CalculationOneFrameSize(3,1);//3*3�̃A�j���[�V����
	m_Component_SpriteAnimation->SetMaxAnimCount(6);//6�t���[���ōX�V

}

void TestActor2D_Animation::Update()//�X�V
{
	Actor2D::Update();//�X�V
}

void TestActor2D_Animation::Uninit()//�I��
{
	Actor2D::Uninit();//�I��
}
