//=============================================================================
//
//  ImGUIマネージャー [Manager_ImGui.cpp]
//  Date   : 2021/11/16
//
//=============================================================================
#include "../../System/Main.h"
#include "../../System/MainWindow.h"
#include "../../Renderer/Renderer_DX11.h"

#include "../../Project/ImGui/imgui.h"
#include "../../Project/ImGui/imgui_impl_dx11.h"
#include "../../Project/ImGui/imgui_impl_win32.h"
#include "../../Project/ImGui/Window_ImGui.h"
#include "Manager_ImGui.h"

void Manager_ImGui::Init()
{
	mainDebugWindow = nullptr;
	GetMainDebugWindow();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	Renderer_DX11::Instance()->GetDevice();

	ID3D11Device* pDev = Renderer_DX11::Instance()->GetDevice();
	ID3D11DeviceContext* pDevContext = Renderer_DX11::Instance()->GetDeviceContext();
	//ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplWin32_Init(MainWindow::Instance()->GetWindow());
	ImGui_ImplDX11_Init(pDev, pDevContext);
}

void Manager_ImGui::Update()
{
	//GUIまとめて更新
	auto itr = windowList.begin();
	for (; itr != windowList.end(); itr++)
	{
		if (Window_ImGui* temp = itr->second)
		{
			temp->Update();
		}
	}
}

void Manager_ImGui::Draw()
{
	//GUIフレーム更新
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//GUIまとめて更新
	auto itr = windowList.begin();
	for (; itr != windowList.end(); itr++)
	{
		if (Window_ImGui* temp = itr->second)
		{
			temp->SetDrawData();//描画するデータをセット
		}
	}

	//GUI描画
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Manager_ImGui::Uninit()
{
	DestroyAllWindow();
}

Window_ImGui* Manager_ImGui::CreateGuiWindow(std::string _windowName)
{
	if (windowList.count(_windowName) == 0)
	{
		Window_ImGui* temp = new Window_ImGui();
		windowList[_windowName] = temp;
		temp->Init();
		temp->SetWindowName(_windowName);
		temp->SetWindowSize(Vector2(100, 100));
	}
	return windowList[_windowName];
}

Window_ImGui* Manager_ImGui::GetGuiWindow(std::string _windowName)
{
	if (windowList.count(_windowName) > 0)
	{
		return windowList[_windowName];
	}
	return nullptr;
}

void Manager_ImGui::DestroyAllWindow()
{
	//todo:delete

	windowList.erase(windowList.begin(), windowList.end());
}

Window_ImGui * Manager_ImGui::GetMainDebugWindow()
{
	if (mainDebugWindow == nullptr)
	{
		mainDebugWindow = CreateGuiWindow("mainDebugWindow");
	}

	return mainDebugWindow;
}
