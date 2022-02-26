//=============================================================================
//
//  Test���x���pHUD�N���X [HUD_Test.cpp]
//  Date   : 2021/12/3
//
//=============================================================================


#include "HUD_Test.h"
#include "../../Component/Component_Sprite.h"
#include "../../Manager/Manager_Texture.h"
#include "../../Manager/Manager_Drawer/Manager_Font2D.h"
#include "../../Manager/Manager_Font3D.h"
#include "../../Renderer/Renderer_Polygon.h"
#include "../../System/Input.h"

void HUD_Test::Init()//������
{

	//�g�p�e�N�X�`���ǂݍ���
	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//�e�N�X�`���[�}�l�[�W���[�L���b�V��
		manager_Texture->LoadTexture("Test", "ROM/2D/Test/T_test.png");
	}

	Manager_Font2D* mf2d = Manager_Font2D::Instance();

	mf2d->SetAilgningMode(Manager_Font::e_Aligning::center);
	mf2d->SetFont(L"02UtsukushiMincho", L"02������������");
	mf2d->SetCharLength(1.0f);
	mf2d->SetFontSize(32);
	mf2d->SetIsUseItalic(false);
	mf2d->SetFontWeight(FW_DONTCARE);
	mf2d->SetFontColor(Color(1, 1, 1, 1));
}

void HUD_Test::Update()//�X�V
{
	HUD::Update();
}
	
void HUD_Test::Uninit()//�I��
{
	HUD::Uninit();
}

void HUD_Test::Draw()//�`��
{
	//Manager_Font2D::Instance()->Printf2D({ 0.0f,240.0f }, "HP");

#pragma region FONT_TEST

	//�����w��q����̃f�t�H���gprintf
	//Manager_Font2D::Instance()->SetFontSize(32.0f);
	//Manager_Font2D::Instance()->SetFontColor(Color(1, 1, 1, 1));
	//Manager_Font2D::Instance()->Printf({ 0.0f,0.0f }, "abcdefghijklmnopqrstuvwxyz");
	//Manager_Font2D::Instance()->Printf({ 0.0f, 50 }, "1234567890");
	//Manager_Font2D::Instance()->Printf({ 0.0f, 100 }, "ABCDEFGHIJKLMN");
	//Manager_Font2D::Instance()->SetFontColor(Color(1, 0, 0, 0.75f));
	//Manager_Font2D::Instance()->Printf({ 0.0f, 150 }, "������������������������������");
	//Manager_Font2D::Instance()->Printf({ 0.0f, 200 }, "�A�C�E�G�I�J�L�N�P�R�T�V�X�Z�\");
	//Manager_Font2D::Instance()->Printf({ 0.0f, 250 }, "����������");
	//Manager_Font2D::Instance()->Printf2D({ 0.0f, 300 },
	//	"int:%d�@float:%f�@double:%lf�@char:%c�@string:%s",5,3.14f,128.9182,'A',"�����͊��������Ō�^^");

	//�e�X�g����
	Manager_Font2D* mf2d = Manager_Font2D::Instance();
	mf2d->SetAilgningMode(Manager_Font::e_Aligning::center);
	mf2d->SetFontColor(Color(1, 0, 0, 1));
	mf2d->SetFontSize(64.0f);
	mf2d->SetFontWeight(FW_NORMAL);

	mf2d->SetFontColor(Color(1, 1, 1, 1));
	mf2d->SetFont(L"keifont", L"�����ӂ����");
	mf2d->Printf2D({ 0,400 }, "�R�C����3�W�߂ĉ�����");

	//Manager_Font2D::Instance()->SetFontSize(48.0f);
	//Manager_Font2D::Instance()->SetFontColor(Color(0, 0, 0, 1));
	//Manager_Font2D::Instance()->PrintfStatic({ 0, 50 }, "abcdefghijklmnopqrstuvwxyz");
	//Manager_Font2D::Instance()->PrintfStatic({ 0.0f, 50 }, "1234567890");
	//Manager_Font2D::Instance()->PrintfStatic({ 0.0f, 100 }, "ABCDEFGHIJKLMN");
	//Manager_Font2D::Instance()->PrintfStatic({ 0.0f, 150 }, "������������������������������");
	//Manager_Font2D::Instance()->PrintfStatic({ 0.0f, 200 }, "�A�C�E�G�I�J�L�N�P�R�T�V�X�Z�\");
	//Manager_Font2D::Instance()->PrintfStatic({ 0.0f, 250 }, "����������");
	//Manager_Font2D::Instance()->PrintfStatic({ 0.0f, 300 }, "A &�@B");//�X�y�[�X�e�X�g

#pragma endregion

	
}
