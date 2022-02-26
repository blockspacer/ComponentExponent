//=============================================================================
//
//  メインウィンドウ　[MainWindow.h]
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
	float currentFps;//実際の現在のFPS
	float optionFps;//設定上のFPS
	std::string className;
	std::string windowName;
	bool isFullscreen;
	bool isFullscreenChecker;
	WNDCLASSEX windowClass;

	double elapsedTime;//一回の更新経過時間
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
	void SwapStartAndEnd();//更新前時間と更新後時間を入れ替え

	bool GetIsFullscreen()const { return isFullscreen; }
	HWND GetWindow()const { return window; }
	Int2 GetWindowSize()const { return windowSize; }
	float GetCurrentFPS() { return currentFps; }
	float GetOptionFPS() { return optionFps; }
	float GetFrame() { return (1.0f / optionFps); }

	inline void SetFps(double _set) { optionFps = static_cast<float>(_set); }//FPS設定
	inline void SetisFullscreen(bool _set) { isFullscreen = _set; };//フルスクリーンモード切り替え
	inline void SetWindowName(const char* _name) { windowName = _name; }//ウィンドウの名前設定
	void SetWindowSize(Int2 _set);//ウィンドウサイズ設定

};