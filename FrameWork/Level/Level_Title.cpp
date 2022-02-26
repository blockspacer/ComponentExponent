//=============================================================================
//
//  タイトル [Level_Title.h]
//  Date   : 2021/1/09
//
//=============================================================================

#include "../System/MainWindow.h"
#include "Level_Title.h"
#include "../System/Input.h"
#include "../Manager/Manager_Game.h"
#include "Level_TP.h"
#include "../Renderer/Renderer_DX11.h"
#include "../Actor/2D/Animation/Test/TestActor2D_Animation.h"

void Level_Title::Init()
{
	Level::Init();

	TestActor2D_Animation::Load();

	//描画マネージャー追加
	if (Manager_Drawer* manager_Drawer = GetManager_Drawer())
	{//描画マネージャーをキャッシュ

		//各種エラー番号追加
		manager_Drawer->AddManager_Sprite();       //スプライトマネージャー追加

		TestActor2D_Animation* titleLogo = AddActor<TestActor2D_Animation>("titleLogo", "none");
		titleLogo->SetScale2D({ static_cast<float>(MainWindow::Instance()->GetWindowSize().x),static_cast<float>(MainWindow::Instance()->GetWindowSize().y) });
		titleLogo->SetPosition2D({ 0,0 });
	}

	Renderer_DX11::Instance()->SetClearColor({ 0,1,0.8f,1 });

	moveSceneCount = 0;
}
void Level_Title::Update()
{
	Level::Update();

	constexpr int countMax = 60 * 3;
	moveSceneCount++;
	if (moveSceneCount >= countMax)
	{
		Manager_Game::Instance()->LoadLevelByClass_New<Level_Test>();
	}
}
void Level_Title::Uninit()
{
	//TestActor2D_Animation::Unload();

	Level::Uninit();
};