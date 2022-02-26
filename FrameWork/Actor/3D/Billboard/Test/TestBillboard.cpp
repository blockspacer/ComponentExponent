//=============================================================================
//
//  テスト用ビルボードクラス [TestBillboard.cpp]
//  Date   : 2021/11/3
//
//=============================================================================


#include "TestBillboard.h"
#include "../../../../Manager/Manager_Texture.h"
#include "../../../../Component/Component_Billboard.h"
#include "../../../../Manager/Manager_Drawer/Manager_ImGui.h"

void  TestBillboard::Load()//リソース読み込み
{

	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//テクスチャーマネージャーキャッシュ
		manager_Texture->LoadTexture("AnimTest", "ROM/2D/Test/T_AnimTest.png");
	}
}
void  TestBillboard::Unload()//リソース削除
{
	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//テクスチャーマネージャーキャッシュ
		manager_Texture->UnloadTexture("AnimTest");
	}
}

void  TestBillboard::Init()//初期化
{
	Actor3D::Init();//初期化

	m_Position = Vector3{ 0.0f,0.0f,1.0f };

	//ビルボードコンポーネント追加
	m_Component_Billboard = AddComponent<Component_Billboard>(0);//追加
	m_Component_Billboard->SetTexture("AnimTest");//テクスチャー設定
	m_Component_Billboard->CalculationOneFrameSize(3,3);//3*3のアニメーション
	m_Component_Billboard->SetMaxAnimCount(6);//6フレームで更新

}

void TestBillboard::Update()//更新
{
	Actor3D::Update();//更新
}

void TestBillboard::Uninit()//終了
{
	Actor3D::Uninit();//終了
}
