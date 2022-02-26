//=============================================================================
//
//  描画マネージャー [Manager_Drawer.cpp]
//  Date   : 2021/11/16
//
//=============================================================================

//******************************************************************************
// インクルード
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

//初期化関数
void Manager_Drawer::Init()
{
	m_Manager_Landscape = nullptr;//ランドスケープマネージャーnullptr化
	m_Manager_Model     = nullptr;//モデルマネージャーnullptr化
	m_Manager_Billboard = nullptr;//ビルボードマネージャーnullptr化
	m_Manager_Sprite    = nullptr;//スプライトマネージャーnullptr化
	m_Manager_OBB       = nullptr;//OBBマネージャーnullptr化
	m_Manager_Rigidbody = nullptr;//Rigidbodyマネージャーnullptr化
	m_HUD               = nullptr;//HUDをnullptr化
	m_Manager_Font2D    = Manager_Font2D::Instance();//キャッシュ
	m_Manager_Font3D    = nullptr;//Font3Dマネージャーnullptr化
	m_Manager_Cube		= nullptr;

	m_Manager_Fade      = new Manager_Fade;//フェードマネージャー生成
	m_Manager_Fade->Init();//フェードマネージャー初期化
	m_Manager_Fade->SetFade(Manager_Fade::FADE_IN, 3.0f);

	m_DebugDraw = false;//デバッグ表示なし
	m_DebugDrawGrid = false;

	Window_ImGui* debugWindow = Manager_ImGui::Instance()->CreateGuiWindow("Manager_Drawer");
	Vector4 tempColor = ColorSpace::Yellow;
	std::string tempColorString = Exchanger::VEC4_TO_STRING(tempColor);
	debugWindow->DebugLog(tempColorString, "none", tempColor);
	debugWindow->SendBoolData(&m_DebugDraw, "DebugDraw");
	debugWindow->SendBoolData(&m_DebugDrawGrid, "DebugDrawGrid");
}


//アクター更新前の更新
void Manager_Drawer::UpdateBefore()
{
	//Rigidbody更新
	if (m_Manager_Rigidbody != nullptr)
	{
		m_Manager_Rigidbody->Update();
	}
}
//アクター更新後の更新
void Manager_Drawer::UpdateAfter()
{
	//HUD更新
	if (m_HUD != nullptr)
	{
		m_HUD->Update();
	}
	//OBBマネージャー更新
	if (m_Manager_OBB != nullptr)
	{
		m_Manager_OBB->Update();
	}
	//フェード更新
	if (m_Manager_Fade != nullptr)
	{
		m_Manager_Fade->Update();
	}
}

//描画関数
void Manager_Drawer::Draw()
{
	//ランドスケープ描画
	if (m_Manager_Landscape != nullptr)
	{
		m_Manager_Landscape->Draw();
	}
	//キューブ描画
	if (m_Manager_Cube != nullptr)
	{
		m_Manager_Cube->Draw();
	}
	//モデル描画
	if (m_Manager_Model != nullptr)
	{
		m_Manager_Model->Draw();
	}
	//ビルボード描画
	if (m_Manager_Billboard != nullptr)
	{
		m_Manager_Billboard->Draw();
	}
	//デバッグ表示オンならコリジョン描画
	if (m_DebugDraw)
	{
		//Rigidbody描画
		if (m_Manager_Rigidbody != nullptr)
		{
			m_Manager_Rigidbody->Draw();
		}

		//OBB/レイ描画
		if (m_Manager_OBB != nullptr)
		{
			m_Manager_OBB->Draw();
		}
	}

	//フォント3D描画
	if (m_Manager_Font3D != nullptr)
	{
		m_Manager_Font3D->Draw();
	}
	//スプライト描画
	if (m_Manager_Sprite != nullptr)
	{
		m_Manager_Sprite->Draw();
	}
	//フォント2D
	{
		m_Manager_Font2D->Draw();
	}
	//HUD描画
	if (m_HUD != nullptr)
	{
		m_HUD->Draw();
	}
	//デバッグ表示オンなら2Dグリッド表示
	if (m_DebugDrawGrid)
	{
		Manager_Grid2D::Instance()->Draw();
	}
	//フェード描画
	if (m_Manager_Fade != nullptr)
	{
		m_Manager_Fade->Draw();
	}
}


//終了関数
void Manager_Drawer::Uninit()
{
	//ランドスケープマネージャー終了
	if (m_Manager_Landscape != nullptr)
	{
		m_Manager_Landscape->Uninit();//終了
		delete m_Manager_Landscape;//削除
		m_Manager_Landscape = nullptr;//nullptr代入
	}
	//キューブマネージャー終了
	if (m_Manager_Cube != nullptr)
	{
		m_Manager_Cube->Uninit();//終了
		delete m_Manager_Cube;//削除
		m_Manager_Cube = nullptr;//nullptr代入
	}
	//モデルマネージャー終了
	if (m_Manager_Model != nullptr)
	{
		m_Manager_Model->Uninit();//終了
		delete m_Manager_Model;//削除
		m_Manager_Model = nullptr;//nullptr代入
	}
	//ビルボードマネージャー終了
	if (m_Manager_Billboard != nullptr)
	{
		m_Manager_Billboard->Uninit();//終了
		delete m_Manager_Billboard;//削除
		m_Manager_Billboard = nullptr;//nullptr代入
	}
	//OBBマネージャー終了
	if (m_Manager_OBB != nullptr)
	{
		m_Manager_OBB->Uninit();//終了
		delete m_Manager_OBB;//削除
		m_Manager_OBB = nullptr;//nullptr代入
	}
	//BulletPhysicsマネージャー終了
	if (m_Manager_Rigidbody != nullptr)
	{
		m_Manager_Rigidbody->Uninit();//終了
		delete m_Manager_Rigidbody;//削除
		m_Manager_Rigidbody = nullptr;//nullptr代入
	}
	//スプライトマネージャー終了
	if (m_Manager_Sprite != nullptr)
	{
		m_Manager_Sprite->Uninit();//終了
		delete m_Manager_Sprite;//削除
		m_Manager_Sprite = nullptr;//nullptr代入
	}
	//HUD終了
	if (m_HUD != nullptr)
	{
		m_HUD->Uninit();//終了
		delete m_HUD;//削除
		m_HUD = nullptr;//nullptr代入
	}
	//フォント3Dマネージャー終了
	if (m_Manager_Font3D != nullptr)
	{
		delete m_Manager_Font3D;//削除
		m_Manager_Font3D = nullptr;//nullptr代入
	}
	//フェードマネージャー終了
	if (m_Manager_Fade != nullptr)
	{
		delete m_Manager_Fade;//削除
		m_Manager_Fade = nullptr;//nullptr代入
	}
}

//マネージャー追加関数

//ランドスケープマネージャー追加
void Manager_Drawer::AddManager_Landscape()
{
	m_Manager_Landscape = new Manager_Landscape;
}
//キューブマネージャー追加
void Manager_Drawer::AddManager_Cube()
{
	m_Manager_Cube = new Manager_Cube;
}
//モデルマネージャー追加
void Manager_Drawer::AddManager_Model()
{
	m_Manager_Model = new Manager_Model;
}
//ビルボードマネージャー追加
void Manager_Drawer::AddManager_Billboard()
{
	m_Manager_Billboard = new Manager_Billboard;
}
//スプライトマネージャー追加
void Manager_Drawer::AddManager_Sprite()
{
	m_Manager_Sprite = new Manager_Sprite;
}
//OBBマネージャー追加
void Manager_Drawer::AddManager_OBB()
{
	m_Manager_OBB = new Manager_OBB;
}

//Rigidbodyマネージャー追加
void Manager_Drawer::AddManager_Rigidbody()
{
	m_Manager_Rigidbody = new Manager_Rigidbody;
	m_Manager_Rigidbody->Init();//初期化
}

void Manager_Drawer::AddManager_Font3D()
{
	m_Manager_Font3D = new Manager_Font3D;
	m_Manager_Font3D->Init();
}
