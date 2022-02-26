//=============================================================================
//
//  ���C���@[Main.cpp]
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
	//���C���E�B���h�E������
	MainWindow* mainWindow = MainWindow::Instance();
	mainWindow->Init(hInstance, nCmdShow, WndProc);
	MSG msg;

	//�X���b�h������
	Manager_Thread::Instance()->Init();

	//�^�C�����C��������
	Manager_Timeline::Instance()->Init();

	//���t���[���g�p����V���O���g�����L���b�V��
	Input*         input         = Input::Instance();//�C���v�b�g
	Renderer_DX11* renderer_DX11 = Renderer_DX11::Instance();//DX11�`��@�\
	Manager_Game*  manager_game  = Manager_Game::Instance();//�Q�[���}�l�[�W���[
	Manager_Audio* manager_audio = Manager_Audio::Instance();//�I�[�f�B�I�}�l�[�W���[
	Manager_ImGui* manager_imgui = Manager_ImGui::Instance();//ImGui�}�l�[�W���[


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

			//�Q�[���̏���
			{
				input->Update();//�C���v�b�g�X�V
				manager_game->Update();//�Q�[���}�l�[�W���[�X�V
				manager_audio->Update();//�T�E���h�}�l�[�W���[�̍X�V
				manager_imgui->Update();//ImGui�̍X�V

				//===========�`��J�n================
				renderer_DX11->Begin();
				manager_game->Draw(); //�Q�[���}�l�[�W���[�`��
				manager_imgui->Draw();//ImGUI�`��
				renderer_DX11->End();
				//===========�`��I��================

			}
			
			mainWindow->DebugFps();//fps�\��
			mainWindow->SwapStartAndEnd();//����ւ�
		}
	}

	Manager_Timeline::Instance()->Uninit();//�^�C�����C���I��
	Manager_Thread::Instance()->Uninit();//�X���b�h�I������
	DataSaver::Instance()->CloseAllFiles();//�Z�[�u�p�ɊJ���Ă����t�@�C�����܂��J���Ă�����S�ĕ���B
	MainWindow::Instance()->Uninit();//�E�B���h�E�I������

	return (int)msg.wParam;
}

//ImGui�p
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//imGUI�p
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	// �}�E�X���b�Z�[�W����
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
