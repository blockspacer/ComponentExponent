//=============================================================================
//
//  テスト用固定2Dアクタークラス [TestActor2D_Static.cpp]
//  Date   : 2021/11/3
//
//=============================================================================


#include "TestActor2D_Static.h"
#include "../../../../Component/Component_Sprite.h"
#include "../../../../Manager/Manager_Texture.h"

void  TestActor2D_Static::Load()//リソース読み込み
{

	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//テクスチャーマネージャーキャッシュ
		manager_Texture->LoadTexture("Test", "ROM/2D/Test/T_test.png");
	}
		
}
void  TestActor2D_Static::Unload()//リソース削除
{

	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//テクスチャーマネージャーキャッシュ
		manager_Texture->UnloadTexture("Test");
	}

}

void  TestActor2D_Static::Init()//初期化
{
	Actor2D::Init();//初期化

	m_Position2D = Vector2{-500.0f,-300.0f};//2次元座標初期値
	m_Scale2D    = Vector2{ 100.0f, 100.0f };//2次元サイズ初期値

	//スプライトコンポーネント設定
	m_Component_Sprite = AddComponent<Component_Sprite>(0);//追加
	m_Component_Sprite->SetTexture("Test");	//テクスチャー設定
}

void TestActor2D_Static::Update()//更新
{
	Actor2D::Update();//更新
}

void TestActor2D_Static::Uninit()//終了
{
	Actor2D::Uninit();//終了
}
