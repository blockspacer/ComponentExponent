//=============================================================================
//
//  メイン　[Main.cpp]
//  Date   : 2021/12/19
//
//=============================================================================

#include "Main.h"
#include "../Renderer/Renderer_DX11.h"
#include "../Manager/Manager_Audio.h"
#include "../Manager/Manager_Game.h"
#include "../Manager/Manager_Drawer/Manager_ImGui.h"
#include "../System/MainWindow.h"
#include "../Manager/Manager_Thread.h"
#include "../Manager/Manager_Timeline.h"
#include "DataSaver.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//メインウィンドウ初期化
	MainWindow* mainWindow = MainWindow::Instance();
	mainWindow->Init(hInstance, nCmdShow, WndProc);
	MSG msg;

	//スレッド初期化
	Manager_Thread::Instance()->Init();

	//タイムライン初期化
	Manager_Timeline::Instance()->Init();

	//毎フレーム使用するシングルトンをキャッシュ
	Input*         input         = Input::Instance();//インプット
	Renderer_DX11* renderer_DX11 = Renderer_DX11::Instance();//DX11描画機構
	Manager_Game*  manager_game  = Manager_Game::Instance();//ゲームマネージャー
	Manager_Audio* manager_audio = Manager_Audio::Instance();//オーディオマネージャー
	Manager_ImGui* manager_imgui = Manager_ImGui::Instance();//ImGuiマネージャー


	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			if (mainWindow->CalculationFps())continue;

			//ゲームの処理
			{
				input->Update();//インプット更新
				manager_game->Update();//ゲームマネージャー更新
				manager_audio->Update();//サウンドマネージャーの更新
				manager_imgui->Update();//ImGuiの更新

				//===========描画開始================
				renderer_DX11->Begin();
				manager_game->Draw(); //ゲームマネージャー描画
				manager_imgui->Draw();//ImGUI描画
				renderer_DX11->End();
				//===========描画終了================

			}
			
			mainWindow->DebugFps();//fps表示
			mainWindow->SwapStartAndEnd();//入れ替え
		}
	}

	Manager_Timeline::Instance()->Uninit();//タイムライン終了
	Manager_Thread::Instance()->Uninit();//スレッド終了処理
	DataSaver::Instance()->CloseAllFiles();//セーブ用に開いていたファイルがまだ開いていたら全て閉じる。
	MainWindow::Instance()->Uninit();//ウィンドウ終了処理

	return (int)msg.wParam;
}

//ImGui用
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//imGUI用
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	// マウスメッセージ処理
	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Input::Instance()->Mouse_ProcessMessage(uMsg, wParam, lParam);
		break;
	}

	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
