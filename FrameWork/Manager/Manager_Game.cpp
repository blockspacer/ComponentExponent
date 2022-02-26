//=============================================================================
//
//  ゲームマネージャー [Manager_Game.cpp]
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


void Manager_Game::Init()//初期化
{

	Manager_Texture::Instance()->Init();//テクスチャマネージャー初期化
	Manager_Shader::Instance()->Init(); //シェーダーマネージャー初期化
	Renderer_Polygon::Instance()->Init(); //プレーンレンダラー初期化
	Manager_Font2D::Instance()->Init();//フォント初期化
	Manager_Grid2D::Instance()->Init();//グリッド2D初期化
	m_PlayerController = nullptr;//プレイヤーコントローラーにnullptr代入

	m_Level     = nullptr;//現在のレベルにnullptr代入
	m_NextLevel = nullptr;//  次のレベルにnullptr代入

	//LoadLevelByClass_New<Level_Title>();//テストシーンセット
	LoadLevelByClass_New<Level_Test>();//テストシーンセット
	//LoadLevelByClass_New<Level_Game>();//テストシーンセット	todo:タイトルに治す

}


void Manager_Game::Uninit()//終了
{
	//プレイヤーコントローラー削除
	if (m_PlayerController != nullptr)
	{
		delete m_PlayerController;//削除
		m_PlayerController = nullptr;//nullptr代入
	}

	//シーン削除
	if (m_Level != nullptr)
	{
		m_Level->Uninit();//終了
		delete m_Level;//削除
		m_Level = nullptr;//nullptr代入
	}

	//マネージャー終了
	Manager_Texture::Instance()->Uninit();//テクスチャマネージャー終了
	Manager_Shader::Instance()->Uninit(); //シェーダーマネージャー終了
	Manager_Font2D::Instance()->Uninit();//フォント初期化
}

void Manager_Game::Update()//更新
{
	//プレイヤーコントローラー更新
	if (m_PlayerController != nullptr)
	{
		m_PlayerController->Update();
	}
	//シーン更新
	if (m_Level != nullptr)
	{
		m_Level->Update();
	}
}

void Manager_Game::Draw()//描画
{
	//シーン描画
	if (m_Level != nullptr)
	{
		m_Level->Draw();//シーン描画
	}
}
