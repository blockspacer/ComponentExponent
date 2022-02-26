//=============================================================================
//
//  ���C���E�B���h�E�@[MainWindow.cpp]
//  Date   : 2021/12/19
//
//=============================================================================

#include "MainWindow.h"
#include "Exchanger.h"
#include "Input.h"
#include "../Project/ImGui/imgui_impl_dx11.h"
#include "../Project/ImGui/imgui_impl_win32.h"
#include "../Manager/Manager_Drawer/Manager_ImGui.h"
#include "../Manager/Manager_Audio.h"
#include "../Manager/Manager_Game.h"
#include "../Renderer/Renderer_DX11.h"


void MainWindow::CreateMyWindow(HINSTANCE _hInstance, int _nCmdShow, WNDPROC _proc)
{
	windowClass =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		_proc,
		0,
		0,
		_hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		className.c_str(),
		NULL
	};

	RegisterClassEx(&windowClass);

	//�f�X�N�g�b�v�T�C�Y�擾
	Vector2 windowPos = { 0,0 };
	RECT desktopRect;
	if (!GetWindowRect(GetDesktopWindow(), &desktopRect))
	{
		MessageBox(NULL, "�E�B���h�E�T�C�Y�̎擾�Ɏ��s���܂���", "ERROR", MB_OK);
	}
	else
	{
		//�t���X�N
		if (isFullscreen)
		{
			windowSize.x = desktopRect.right;
			windowSize.y = desktopRect.bottom;

		}
		else
		{
			//�t���X�N����Ȃ����̓E�B���h�E����ʒ��S�ɁB
			windowPos.x = (desktopRect.right - windowSize.x) * 0.5f;
			windowPos.y = (desktopRect.bottom - windowSize.y) * 0.5f;
		}
	}

	int what = GetSystemMetrics(SM_CXDLGFRAME);
	int what2 = GetSystemMetrics(SM_CYCAPTION);

	window = CreateWindowEx
	(
		0,
		className.c_str(),
		windowName.c_str(),
		WS_POPUPWINDOW,
		static_cast<int>(windowPos.x),
		static_cast<int>(windowPos.y),
		(windowSize.x + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		(windowSize.y + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL,
		NULL,
		_hInstance,
		NULL
	);

	ShowWindow(window, _nCmdShow);
	UpdateWindow(window);
}

bool MainWindow::CalculationFps()
{
	//���݂̎��Ԃ��擾
	QueryPerformanceCounter(&timeEnd);

	//(���̎��� - �O�̃t���[���̎���) / ���g�� = �o�ߎ��ԁi�b�P�ʁj
	elapsedTime = static_cast<double>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<double>(timeFrequency.QuadPart);

	static int tempUcnt = 0;
	static int tempSlpCnt = 0;

	//�O��̍X�V���Ԃ��A1��̍X�V�Ɏg���Ă������ԓ���������A���̃t���[���܂ł̎���Sleep����B
	if (elapsedTime < GetFrame())
	{
		DWORD sleepTime = static_cast<DWORD>(((GetFrame() - elapsedTime) * 1000.0));
		timeBeginPeriod(1);
		Sleep(sleepTime);

		std::string tempSlp = "SLP : " + Exchanger::INT_TO_STRING((int)sleepTime);
		Manager_ImGui::Instance()->GetMainDebugWindow()->DebugLogDontDestroy(tempSlp, "SLP", Vector4(0, 0.5f, 0.5f, 1));

		tempUcnt++;
		if (tempUcnt >= 60)
		{
			std::string tempSlp = "SLPCNT : " + Exchanger::FLOAT_TO_STRING((tempSlpCnt / (tempUcnt * 1.0f)) * 100.0f);
			Manager_ImGui::Instance()->GetMainDebugWindow()->DebugLogDontDestroy(tempSlp, "SLPCNT", Vector4(1, 0.5f, 0.5f, 1));
			tempUcnt = 0;
			tempSlpCnt = 0;
		}

		tempSlpCnt += sleepTime;
		timeEndPeriod(1);

		return true;
	}
	return false;
}

void MainWindow::Init(HINSTANCE _hInstance, int _nCmdShow, WNDPROC _proc)
{
	//�ϐ�������
	className = "AppClass";
	windowName = "�Q�[���^�C�g��";
	windowSize = Int2(1920, 1080);
	isFullscreen = false;
	isFullscreenChecker = isFullscreen;
	elapsedTime = 0.0;
	timeFrequency = {};
	timeStart = {};
	timeEnd = {};
	optionFps = 60.0f;//�ݒ���FPS
	currentFps = 0.0f;

	hInstance = &_hInstance;
	nCmdShow = &_nCmdShow;
	proc = &_proc;

	//�E�B���h�E�쐬
	CreateMyWindow(_hInstance, _nCmdShow, _proc);

	//Renderer_DX11������
	Renderer_DX11::Instance()->Init();

	// �C���v�b�g������
	Input::Instance()->Init(window);

	//�Q�[���}�l�[�W���[������
	Manager_Game::Instance()->Init();

	// �T�E���h�}�l�[�W���[�̏�����
	Manager_Audio::Instance()->Init();

	//ImGui�}�l�[�W���[������
	Manager_ImGui::Instance()->Init();

	//�f�o�b�O�̂��߂�FPS�ϐ���GUI�ɃZ�b�g
	Manager_ImGui::Instance()->GetMainDebugWindow()->SendFloatData(&optionFps, "SETTING_FPS", 1, 300);

	//FPS
	if (QueryPerformanceFrequency(&timeFrequency) == FALSE)
	{
		MessageBox(NULL, "FPS�擾�̂��߂�freq���擾�o���Ȃ��G���[", "ERROR", MB_OK);
	}

	//����v�Z�p
	QueryPerformanceCounter(&timeStart);

	HDC hdc = GetDC(MainWindow::Instance()->GetWindow());
	int rate = GetDeviceCaps(hdc, VREFRESH);
	float interval = rate / 60.0f;
	Manager_ImGui::Instance()->GetMainDebugWindow()->DebugLogDontDestroy("REF:RATE : " + Exchanger::FLOAT_TO_STRING(interval * 60.0f), "refrate", Vector4(0, 1, 1, 1));
	Manager_ImGui::Instance()->GetMainDebugWindow()->DebugLogDontDestroy("REF60:RATE : " + Exchanger::FLOAT_TO_STRING(interval), "refrate60", Vector4(0, 1, 1, 1));

}

void MainWindow::Uninit()
{
	Manager_Audio::Instance()->UnInit();

	//ImGui�I��
	Manager_ImGui::Instance()->Uninit();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	UnregisterClass(className.c_str(), windowClass.hInstance);

	//�Q�[���}�l�[�W���[�I������
	Manager_Game::Instance()->Uninit();

	//�C���v�b�g�I������
	Input::Instance()->Uninit();

	//Renderer_DX11�I������
	Renderer_DX11::Instance()->Uninit();
}

void MainWindow::DebugFps()
{
	//todo:if_DEBUG
	static int tempTestCnt = 0;
	tempTestCnt++;
	if (tempTestCnt >= 60)
	{
		tempTestCnt = 0;

		//ImGui��FPS�\��
		currentFps = static_cast<float>(1.0 / elapsedTime);
		std::string tempFps = "FPS : " + Exchanger::INT_TO_STRING((int)currentFps);
		Manager_ImGui::Instance()->GetMainDebugWindow()->DebugLogDontDestroy(tempFps, "FPS", Vector4(1, 0, 0, 1));

		std::string tempFreq = "FRAME : " + Exchanger::DOUBLE_TO_STRING((elapsedTime));
		Manager_ImGui::Instance()->GetMainDebugWindow()->DebugLogDontDestroy(tempFreq, "FRAME", Vector4(1, 0, 0, 1));
	}
}

void MainWindow::SwapStartAndEnd()
{
	timeStart = timeEnd;
}

void MainWindow::SetWindowSize(Int2 _set)
{
	//if (_set.x > 0 && _set.y > 0)windowSize = _set;

	//todo:
}
