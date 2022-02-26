//=============================================================================
//
//  ImGUI�}�l�[�W���[ [Manager_ImGui.h]
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

	friend class Singleton<Manager_ImGui>;//�V���O���g��
	std::unordered_map<std::string, class Window_ImGui*>windowList;//GUI�E�B���h�E�z��
	class Window_ImGui* mainDebugWindow;

public:

	Manager_ImGui() {};
	virtual ~Manager_ImGui() {};

	void Init();
	void Update();
	void Draw();
	void Uninit();

	//GUI�E�B���h�E�쐬�B�߂�l��Window_ImGui��SetWindowName()���Ȃ��Ă����̊֐��̈����ŃZ�b�g�����B
	class Window_ImGui* CreateGuiWindow(std::string _windowName);
	class Window_ImGui* GetGuiWindow(std::string _windowName);//GUI�E�B���h�E�擾
	
	void DestroyAllWindow();//�S�E�B���h�E�폜

	class Window_ImGui* GetMainDebugWindow();

};