//=============================================================================
//
//  �`��}�l�[�W���[ [Manager_Drawer.cpp]
//  Date   : 2021/11/16
//
//=============================================================================

//******************************************************************************
// �C���N���[�h
//******************************************************************************

#include "Manager_Drawer.h"
#include "../Manager_Landscape.h"
#include "../Manager_Cube.h"
#include "../Manager_Model.h"
#include "../Manager_Billboard.h"
#include "../Manager_Sprite.h"
#include "../Manager_Fade.h"
#include "../Manager_OBB.h"
#include "../Manager_Rigidbody.h"
#include "../../Manager_Grid.h"
#include "../../../System/Color.h"
#include "../../../System/Exchanger.h"
#include "../../../Manager/Manager_Drawer/Manager_ImGui.h"
#include "../../../Actor/HUD/Base/HUD.h"
#include "../../Manager_Font3D.h"
#include "../../Manager_Drawer/Manager_Font2D.h"

//�������֐�
void Manager_Drawer::Init()
{
	m_Manager_Landscape = nullptr;//�����h�X�P�[�v�}�l�[�W���[nullptr��
	m_Manager_Model     = nullptr;//���f���}�l�[�W���[nullptr��
	m_Manager_Billboard = nullptr;//�r���{�[�h�}�l�[�W���[nullptr��
	m_Manager_Sprite    = nullptr;//�X�v���C�g�}�l�[�W���[nullptr��
	m_Manager_OBB       = nullptr;//OBB�}�l�[�W���[nullptr��
	m_Manager_Rigidbody = nullptr;//Rigidbody�}�l�[�W���[nullptr��
	m_HUD               = nullptr;//HUD��nullptr��
	m_Manager_Font2D    = Manager_Font2D::Instance();//�L���b�V��
	m_Manager_Font3D    = nullptr;//Font3D�}�l�[�W���[nullptr��
	m_Manager_Cube		= nullptr;

	m_Manager_Fade      = new Manager_Fade;//�t�F�[�h�}�l�[�W���[����
	m_Manager_Fade->Init();//�t�F�[�h�}�l�[�W���[������
	m_Manager_Fade->SetFade(Manager_Fade::FADE_IN, 3.0f);

	m_DebugDraw = false;//�f�o�b�O�\���Ȃ�
	m_DebugDrawGrid = false;

	Window_ImGui* debugWindow = Manager_ImGui::Instance()->CreateGuiWindow("Manager_Drawer");
	Vector4 tempColor = ColorSpace::Yellow;
	std::string tempColorString = Exchanger::VEC4_TO_STRING(tempColor);
	debugWindow->DebugLog(tempColorString, "none", tempColor);
	debugWindow->SendBoolData(&m_DebugDraw, "DebugDraw");
	debugWindow->SendBoolData(&m_DebugDrawGrid, "DebugDrawGrid");
}


//�A�N�^�[�X�V�O�̍X�V
void Manager_Drawer::UpdateBefore()
{
	//Rigidbody�X�V
	if (m_Manager_Rigidbody != nullptr)
	{
		m_Manager_Rigidbody->Update();
	}
}
//�A�N�^�[�X�V��̍X�V
void Manager_Drawer::UpdateAfter()
{
	//HUD�X�V
	if (m_HUD != nullptr)
	{
		m_HUD->Update();
	}
	//OBB�}�l�[�W���[�X�V
	if (m_Manager_OBB != nullptr)
	{
		m_Manager_OBB->Update();
	}
	//�t�F�[�h�X�V
	if (m_Manager_Fade != nullptr)
	{
		m_Manager_Fade->Update();
	}
}

//�`��֐�
void Manager_Drawer::Draw()
{
	//�����h�X�P�[�v�`��
	if (m_Manager_Landscape != nullptr)
	{
		m_Manager_Landscape->Draw();
	}
	//�L���[�u�`��
	if (m_Manager_Cube != nullptr)
	{
		m_Manager_Cube->Draw();
	}
	//���f���`��
	if (m_Manager_Model != nullptr)
	{
		m_Manager_Model->Draw();
	}
	//�r���{�[�h�`��
	if (m_Manager_Billboard != nullptr)
	{
		m_Manager_Billboard->Draw();
	}
	//�f�o�b�O�\���I���Ȃ�R���W�����`��
	if (m_DebugDraw)
	{
		//Rigidbody�`��
		if (m_Manager_Rigidbody != nullptr)
		{
			m_Manager_Rigidbody->Draw();
		}

		//OBB/���C�`��
		if (m_Manager_OBB != nullptr)
		{
			m_Manager_OBB->Draw();
		}
	}

	//�t�H���g3D�`��
	if (m_Manager_Font3D != nullptr)
	{
		m_Manager_Font3D->Draw();
	}
	//�X�v���C�g�`��
	if (m_Manager_Sprite != nullptr)
	{
		m_Manager_Sprite->Draw();
	}
	//�t�H���g2D
	{
		m_Manager_Font2D->Draw();
	}
	//HUD�`��
	if (m_HUD != nullptr)
	{
		m_HUD->Draw();
	}
	//�f�o�b�O�\���I���Ȃ�2D�O���b�h�\��
	if (m_DebugDrawGrid)
	{
		Manager_Grid2D::Instance()->Draw();
	}
	//�t�F�[�h�`��
	if (m_Manager_Fade != nullptr)
	{
		m_Manager_Fade->Draw();
	}
}


//�I���֐�
void Manager_Drawer::Uninit()
{
	//�����h�X�P�[�v�}�l�[�W���[�I��
	if (m_Manager_Landscape != nullptr)
	{
		m_Manager_Landscape->Uninit();//�I��
		delete m_Manager_Landscape;//�폜
		m_Manager_Landscape = nullptr;//nullptr���
	}
	//�L���[�u�}�l�[�W���[�I��
	if (m_Manager_Cube != nullptr)
	{
		m_Manager_Cube->Uninit();//�I��
		delete m_Manager_Cube;//�폜
		m_Manager_Cube = nullptr;//nullptr���
	}
	//���f���}�l�[�W���[�I��
	if (m_Manager_Model != nullptr)
	{
		m_Manager_Model->Uninit();//�I��
		delete m_Manager_Model;//�폜
		m_Manager_Model = nullptr;//nullptr���
	}
	//�r���{�[�h�}�l�[�W���[�I��
	if (m_Manager_Billboard != nullptr)
	{
		m_Manager_Billboard->Uninit();//�I��
		delete m_Manager_Billboard;//�폜
		m_Manager_Billboard = nullptr;//nullptr���
	}
	//OBB�}�l�[�W���[�I��
	if (m_Manager_OBB != nullptr)
	{
		m_Manager_OBB->Uninit();//�I��
		delete m_Manager_OBB;//�폜
		m_Manager_OBB = nullptr;//nullptr���
	}
	//BulletPhysics�}�l�[�W���[�I��
	if (m_Manager_Rigidbody != nullptr)
	{
		m_Manager_Rigidbody->Uninit();//�I��
		delete m_Manager_Rigidbody;//�폜
		m_Manager_Rigidbody = nullptr;//nullptr���
	}
	//�X�v���C�g�}�l�[�W���[�I��
	if (m_Manager_Sprite != nullptr)
	{
		m_Manager_Sprite->Uninit();//�I��
		delete m_Manager_Sprite;//�폜
		m_Manager_Sprite = nullptr;//nullptr���
	}
	//HUD�I��
	if (m_HUD != nullptr)
	{
		m_HUD->Uninit();//�I��
		delete m_HUD;//�폜
		m_HUD = nullptr;//nullptr���
	}
	//�t�H���g3D�}�l�[�W���[�I��
	if (m_Manager_Font3D != nullptr)
	{
		delete m_Manager_Font3D;//�폜
		m_Manager_Font3D = nullptr;//nullptr���
	}
	//�t�F�[�h�}�l�[�W���[�I��
	if (m_Manager_Fade != nullptr)
	{
		delete m_Manager_Fade;//�폜
		m_Manager_Fade = nullptr;//nullptr���
	}
}

//�}�l�[�W���[�ǉ��֐�

//�����h�X�P�[�v�}�l�[�W���[�ǉ�
void Manager_Drawer::AddManager_Landscape()
{
	m_Manager_Landscape = new Manager_Landscape;
}
//�L���[�u�}�l�[�W���[�ǉ�
void Manager_Drawer::AddManager_Cube()
{
	m_Manager_Cube = new Manager_Cube;
}
//���f���}�l�[�W���[�ǉ�
void Manager_Drawer::AddManager_Model()
{
	m_Manager_Model = new Manager_Model;
}
//�r���{�[�h�}�l�[�W���[�ǉ�
void Manager_Drawer::AddManager_Billboard()
{
	m_Manager_Billboard = new Manager_Billboard;
}
//�X�v���C�g�}�l�[�W���[�ǉ�
void Manager_Drawer::AddManager_Sprite()
{
	m_Manager_Sprite = new Manager_Sprite;
}
//OBB�}�l�[�W���[�ǉ�
void Manager_Drawer::AddManager_OBB()
{
	m_Manager_OBB = new Manager_OBB;
}

//Rigidbody�}�l�[�W���[�ǉ�
void Manager_Drawer::AddManager_Rigidbody()
{
	m_Manager_Rigidbody = new Manager_Rigidbody;
	m_Manager_Rigidbody->Init();//������
}

void Manager_Drawer::AddManager_Font3D()
{
	m_Manager_Font3D = new Manager_Font3D;
	m_Manager_Font3D->Init();
}
