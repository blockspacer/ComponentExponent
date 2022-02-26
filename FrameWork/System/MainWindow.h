//=============================================================================
//
//  ���C���E�B���h�E�@[MainWindow.h]
//  Date   : 2021/12/19
//
//=============================================================================
#pragma once

#include "../System/Main.h"
#include "../System/Singleton.h"

class MainWindow : public Singleton<MainWindow>
{
private:

	friend class Singleton<MainWindow>;
	HWND window;
	Int2 windowSize;
	float currentFps;//���ۂ̌��݂�FPS
	float optionFps;//�ݒ���FPS
	std::string className;
	std::string windowName;
	bool isFullscreen;
	bool isFullscreenChecker;
	WNDCLASSEX windowClass;

	double elapsedTime;//���̍X�V�o�ߎ���
	LARGE_INTEGER timeFrequency;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;

	HINSTANCE* hInstance;
	int* nCmdShow;
	WNDPROC* proc;

	void CreateMyWindow(HINSTANCE _hInstance, int _nCmdShow, WNDPROC _proc);

public:

	void Init(HINSTANCE _hInstance, int _nCmdShow, WNDPROC _proc);
	void Uninit();

	bool CalculationFps();
	void DebugFps();
	void SwapStartAndEnd();//�X�V�O���ԂƍX�V�㎞�Ԃ����ւ�

	bool GetIsFullscreen()const { return isFullscreen; }
	HWND GetWindow()const { return window; }
	Int2 GetWindowSize()const { return windowSize; }
	float GetCurrentFPS() { return currentFps; }
	float GetOptionFPS() { return optionFps; }
	float GetFrame() { return (1.0f / optionFps); }

	inline void SetFps(double _set) { optionFps = static_cast<float>(_set); }//FPS�ݒ�
	inline void SetisFullscreen(bool _set) { isFullscreen = _set; };//�t���X�N���[�����[�h�؂�ւ�
	inline void SetWindowName(const char* _name) { windowName = _name; }//�E�B���h�E�̖��O�ݒ�
	void SetWindowSize(Int2 _set);//�E�B���h�E�T�C�Y�ݒ�

};