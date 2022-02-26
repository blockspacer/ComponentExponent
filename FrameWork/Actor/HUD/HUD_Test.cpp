//=============================================================================
//
//  Testレベル用HUDクラス [HUD_Test.cpp]
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

void HUD_Test::Init()//初期化
{

	//使用テクスチャ読み込み
	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//テクスチャーマネージャーキャッシュ
		manager_Texture->LoadTexture("Test", "ROM/2D/Test/T_test.png");
	}

	Manager_Font2D* mf2d = Manager_Font2D::Instance();

	mf2d->SetAilgningMode(Manager_Font::e_Aligning::center);
	mf2d->SetFont(L"02UtsukushiMincho", L"02うつくし明朝体");
	mf2d->SetCharLength(1.0f);
	mf2d->SetFontSize(32);
	mf2d->SetIsUseItalic(false);
	mf2d->SetFontWeight(FW_DONTCARE);
	mf2d->SetFontColor(Color(1, 1, 1, 1));
}

void HUD_Test::Update()//更新
{
	HUD::Update();
}
	
void HUD_Test::Uninit()//終了
{
	HUD::Uninit();
}

void HUD_Test::Draw()//描画
{
	//Manager_Font2D::Instance()->Printf2D({ 0.0f,240.0f }, "HP");

#pragma region FONT_TEST

	//書式指定子ありのデフォルトprintf
	//Manager_Font2D::Instance()->SetFontSize(32.0f);
	//Manager_Font2D::Instance()->SetFontColor(Color(1, 1, 1, 1));
	//Manager_Font2D::Instance()->Printf({ 0.0f,0.0f }, "abcdefghijklmnopqrstuvwxyz");
	//Manager_Font2D::Instance()->Printf({ 0.0f, 50 }, "1234567890");
	//Manager_Font2D::Instance()->Printf({ 0.0f, 100 }, "ABCDEFGHIJKLMN");
	//Manager_Font2D::Instance()->SetFontColor(Color(1, 0, 0, 0.75f));
	//Manager_Font2D::Instance()->Printf({ 0.0f, 150 }, "あいうえおかきくけこさしすせそ");
	//Manager_Font2D::Instance()->Printf({ 0.0f, 200 }, "アイウエオカキクケコサシスセソ");
	//Manager_Font2D::Instance()->Printf({ 0.0f, 250 }, "ｱｲｳｴｵｶｷｸｹｺ");
	//Manager_Font2D::Instance()->Printf2D({ 0.0f, 300 },
	//	"int:%d　float:%f　double:%lf　char:%c　string:%s",5,3.14f,128.9182,'A',"今朝は寒かったで候^^");

	//テスト文字
	Manager_Font2D* mf2d = Manager_Font2D::Instance();
	mf2d->SetAilgningMode(Manager_Font::e_Aligning::center);
	mf2d->SetFontColor(Color(1, 0, 0, 1));
	mf2d->SetFontSize(64.0f);
	mf2d->SetFontWeight(FW_NORMAL);

	mf2d->SetFontColor(Color(1, 1, 1, 1));
	mf2d->SetFont(L"keifont", L"けいふぉんと");
	mf2d->Printf2D({ 0,400 }, "コインを3個集めて下さい");

	//Manager_Font2D::Instance()->SetFontSize(48.0f);
	//Manager_Font2D::Instance()->SetFontColor(Color(0, 0, 0, 1));
	//Manager_Font2D::Instance()->PrintfStatic({ 0, 50 }, "abcdefghijklmnopqrstuvwxyz");
	//Manager_Font2D::Instance()->PrintfStatic({ 0.0f, 50 }, "1234567890");
	//Manager_Font2D::Instance()->PrintfStatic({ 0.0f, 100 }, "ABCDEFGHIJKLMN");
	//Manager_Font2D::Instance()->PrintfStatic({ 0.0f, 150 }, "あいうえおかきくけこさしすせそ");
	//Manager_Font2D::Instance()->PrintfStatic({ 0.0f, 200 }, "アイウエオカキクケコサシスセソ");
	//Manager_Font2D::Instance()->PrintfStatic({ 0.0f, 250 }, "ｱｲｳｴｵｶｷｸｹｺ");
	//Manager_Font2D::Instance()->PrintfStatic({ 0.0f, 300 }, "A &　B");//スペーステスト

#pragma endregion

	
}
