//=============================================================================
//
//  テスト用アニメーション2Dアクタークラス [TestActor2D_Animation.cpp]
//  Date   : 2021/11/3
//
//=============================================================================


#include "TestActor2D_Animation.h"
#include "../../../../Manager/Manager_Texture.h"
#include "../../../../Component/Component_SpriteAnimation.h"

void  TestActor2D_Animation::Load()//リソース読み込み
{

	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//テクスチャーマネージャーキャッシュ
		manager_Texture->LoadTexture("AnimTest", "ROM/2D/Title/logo_03.png");
	}
		
}
void  TestActor2D_Animation::Unload()//リソース削除
{

	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//テクスチャーマネージャーキャッシュ
		manager_Texture->UnloadTexture("AnimTest");
	}

}

void  TestActor2D_Animation::Init()//初期化
{
	Actor2D::Init();//初期化

	m_Position2D = Vector2{0.0f,0.0f};//2次元座標初期値
	m_Scale2D    = Vector2{1920.0f,1080.0f};//2次元サイズ初期値
	//todo:スケール

	//アニメーションスプライトコンポーネント設定
	m_Component_SpriteAnimation = AddComponent<Component_SpriteAnimation>(0);//追加
	m_Component_SpriteAnimation->SetTexture("AnimTest");//テクスチャー設定
	m_Component_SpriteAnimation->CalculationOneFrameSize(3,1);//3*3のアニメーション
	m_Component_SpriteAnimation->SetMaxAnimCount(6);//6フレームで更新

}

void TestActor2D_Animation::Update()//更新
{
	Actor2D::Update();//更新
}

void TestActor2D_Animation::Uninit()//終了
{
	Actor2D::Uninit();//終了
}
