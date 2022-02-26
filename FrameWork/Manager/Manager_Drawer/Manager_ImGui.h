//=============================================================================
//
//  ImGUIマネージャー [Manager_ImGui.h]
//  Date   : 2021/11/16
//
//=============================================================================
#pragma once

#include "../../System/Singleton.h"
#include "../../Project/ImGui/Window_ImGui.h"
#include <unordered_map>
#include <string>

class Manager_ImGui : public Singleton<Manager_ImGui>
{
private:

	friend class Singleton<Manager_ImGui>;//シングルトン
	std::unordered_map<std::string, class Window_ImGui*>windowList;//GUIウィンドウ配列
	class Window_ImGui* mainDebugWindow;

public:

	Manager_ImGui() {};
	virtual ~Manager_ImGui() {};

	void Init();
	void Update();
	void Draw();
	void Uninit();

	//GUIウィンドウ作成。戻り値のWindow_ImGuiにSetWindowName()しなくてもこの関数の引数でセットされる。
	class Window_ImGui* CreateGuiWindow(std::string _windowName);
	class Window_ImGui* GetGuiWindow(std::string _windowName);//GUIウィンドウ取得
	
	void DestroyAllWindow();//全ウィンドウ削除

	class Window_ImGui* GetMainDebugWindow();

};