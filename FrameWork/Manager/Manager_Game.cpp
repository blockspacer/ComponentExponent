//=============================================================================
//
//  �Q�[���}�l�[�W���[ [Manager_Game.cpp]
//  Date   : 2021/11/02
//
//=============================================================================

#include <time.h>
#include "../Renderer/Renderer_Polygon.h"
#include "../Level/Level_TP.h"
#include "../Level/Level_Title.h"
#include "../Level/Level_Game.h"
#include "../System/Input.h"
#include "../System/Main.h"
#include "Manager_Game.h"
#include "Manager_Texture.h"
#include "Manager_Shader.h"
#include "Manager_Drawer/Manager_ImGui.h"
#include "Manager_Drawer/Manager_Font2D.h"
#include "Manager_Grid.h"
#include "Manager_Font.h"
#include "Manager_Font3D.h"


void Manager_Game::Init()//������
{

	Manager_Texture::Instance()->Init();//�e�N�X�`���}�l�[�W���[������
	Manager_Shader::Instance()->Init(); //�V�F�[�_�[�}�l�[�W���[������
	Renderer_Polygon::Instance()->Init(); //�v���[�������_���[������
	Manager_Font2D::Instance()->Init();//�t�H���g������
	Manager_Grid2D::Instance()->Init();//�O���b�h2D������
	m_PlayerController = nullptr;//�v���C���[�R���g���[���[��nullptr���

	m_Level     = nullptr;//���݂̃��x����nullptr���
	m_NextLevel = nullptr;//  ���̃��x����nullptr���

	//LoadLevelByClass_New<Level_Title>();//�e�X�g�V�[���Z�b�g
	LoadLevelByClass_New<Level_Test>();//�e�X�g�V�[���Z�b�g
	//LoadLevelByClass_New<Level_Game>();//�e�X�g�V�[���Z�b�g	todo:�^�C�g���Ɏ���

}


void Manager_Game::Uninit()//�I��
{
	//�v���C���[�R���g���[���[�폜
	if (m_PlayerController != nullptr)
	{
		delete m_PlayerController;//�폜
		m_PlayerController = nullptr;//nullptr���
	}

	//�V�[���폜
	if (m_Level != nullptr)
	{
		m_Level->Uninit();//�I��
		delete m_Level;//�폜
		m_Level = nullptr;//nullptr���
	}

	//�}�l�[�W���[�I��
	Manager_Texture::Instance()->Uninit();//�e�N�X�`���}�l�[�W���[�I��
	Manager_Shader::Instance()->Uninit(); //�V�F�[�_�[�}�l�[�W���[�I��
	Manager_Font2D::Instance()->Uninit();//�t�H���g������
}

void Manager_Game::Update()//�X�V
{
	//�v���C���[�R���g���[���[�X�V
	if (m_PlayerController != nullptr)
	{
		m_PlayerController->Update();
	}
	//�V�[���X�V
	if (m_Level != nullptr)
	{
		m_Level->Update();
	}
}

void Manager_Game::Draw()//�`��
{
	//�V�[���`��
	if (m_Level != nullptr)
	{
		m_Level->Draw();//�V�[���`��
	}
}
