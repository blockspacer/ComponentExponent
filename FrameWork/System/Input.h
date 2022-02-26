//=============================================================================
//
//  キーボード&マウス&ゲームパッド [Input.cpp]
//  Date   : 2021/11/2
//
//=============================================================================
#pragma once
#include <windowsx.h>
#include <Windows.h>
#include <Xinput.h>
#include <memory>
#include <vector>
#include "Singleton.h"
#include "Main.h"


class Input : public Singleton<Input>
{
public:
	// マウス系
	enum EMouseButtonKind
	{
		BUTTON_NONE = 0,
		BUTTON_LEFT,
		BUTTON_RIGHT,
		BUTTON_MIDDLE,
		BUTTON_MAX
	};

	// マウス座標モード
	typedef enum EMouse_PositionMode_tag
	{
		MOUSE_POSITION_MODE_ABSOLUTE, // 絶対座標モード
		MOUSE_POSITION_MODE_RELATIVE, // 相対座標モード
	} s_Mouse_PositionMode;


	// マウス状態構造体
	typedef struct 
	{
		bool leftButton;
		bool middleButton;
		bool rightButton;
		bool xButton1;
		bool xButton2;
		int x;
		int y;
		int scrollWheelValue;
		s_Mouse_PositionMode positionMode;
	} s_Mouse_State;

	//ゲームパッド系
	typedef struct 
	{
		XINPUT_STATE state;
		int portNum;//ポード番号

		int LEFT_STICK_X  = STICK_NONE;
		int LEFT_STICK_Y  = STICK_NONE;
		int RIGHT_STICK_X = STICK_NONE;
		int RIGHT_STICK_Y = STICK_NONE;
		bool X_BUTTON     = false;
		bool Y_BUTTON     = false;
		bool B_BUTTON     = false;
		bool A_BUTTON     = false;
		bool START_BUTTON = false;
		bool BACK_BUTTON  = false;
		bool RB_BUTTON    = false;
		bool LB_BUTTON    = false;
		bool RT_BUTTON    = false;
		bool LT_BUTTON    = false;
	} s_Gamepad_State;

	enum EGamepadButtonKind
	{
		STICK_NONE = 0,
		STICK_LEFT, //左倒し
		STICK_RIGHT,//右倒し
		STICK_UP,   //上倒し
		STICK_DOWN  //下倒し
	};

	enum EGamepadNum
	{
		GAMEPAD_1 = 0,//ゲームパッド1
		GAMEPAD_2,    //ゲームパッド2
		MAX_CONTROLLERS//ゲームパッド最大接続数
	};
	

private:
	friend class Singleton<Input>;//シングルトン

	//キーボード
	BYTE m_OldKeyState[256];
	BYTE m_KeyState[256];
	s_Mouse_State m_MouseState;

	//マウス
	void Mouse_GetState();
	void clipToWindow(void);

	HWND               m_Window = NULL;
	s_Mouse_PositionMode m_Mode = MOUSE_POSITION_MODE_ABSOLUTE;//相対座標モードor絶対座標モード（デフォルトは絶対座標モード）
	HANDLE             m_ScrollWheelValue = NULL;
	HANDLE             m_RelativeRead = NULL;
	HANDLE             m_AbsoluteMode = NULL;
	HANDLE             m_RelativeMode = NULL;
	int                m_LastX = 0;
	int                m_LastY = 0;
	int                m_RelativeX = INT32_MAX;
	int                m_RelativeY = INT32_MAX;
	bool               m_InFocus = true;

	//ゲームパッド
	std::vector<s_Gamepad_State> m_Gamepads;//所有ゲームパッド

public:

	void Init(HWND _window);//初期化
	void Uninit();//終了
	void Update();//更新

	//キーボード
	inline bool GetKeyPress(BYTE _KeyCode)     const { return (m_KeyState[_KeyCode] & 0x80); };
	inline bool GetKeyTrigger( BYTE _KeyCode ) const { return ((m_KeyState[_KeyCode] & 0x80) && !(m_OldKeyState[_KeyCode] & 0x80));}

	//マウス
	inline s_Mouse_State GetMouse_State() const { return m_MouseState; }//取得
	void Mouse_ProcessMessage(UINT _message, WPARAM _wParam, LPARAM _lParam);
	inline bool GetButtonPress(EMouseButtonKind _kind);
	//マウスコンフィグ系
	void Mouse_ResetScrollWheelValue(void);//累積したマウススクロールホイール値をリセットする
	void Mouse_SetMode(s_Mouse_PositionMode _mode);//マウスのポジションモードを設定する
	void Mouse_SetVisible(bool _visible);//マウスカーソル表示を設定する
	inline s_Mouse_PositionMode GetMouse_PositionMode() const { return m_Mode; }//現在の座標モード取得

	//ゲームパッド
	inline s_Gamepad_State GetGamepad_State(int _num) const { return m_Gamepads[_num]; }//状態取得関数。引数はゲームパッドenum

};
