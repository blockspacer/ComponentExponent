//=============================================================================
//
//  キーボード&マウス&ゲームパッド [Input.cpp]
//  Date   : 2021/11/2
//
//=============================================================================

#include "Input.h"

#define SAFE_CLOSEHANDLE(h) if(h){CloseHandle(h); h = NULL;}

void Input::Init(HWND _window)
{
	//Window取得
	m_Window = _window;

	//キーボード初期化
	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );

	// マウスの初期化処理
	RtlZeroMemory(&m_MouseState, sizeof(m_MouseState));

	assert(_window != NULL);

	RAWINPUTDEVICE Rid;
	Rid.usUsagePage = 0x01 /* HID_USAGE_PAGE_GENERIC */;
	Rid.usUsage = 0x02     /* HID_USAGE_GENERIC_MOUSE */;
	Rid.dwFlags = RIDEV_INPUTSINK;
	Rid.hwndTarget = _window;
	RegisterRawInputDevices(&Rid, 1, sizeof(RAWINPUTDEVICE));

	//絶対座標モードに設定
	m_Mode = MOUSE_POSITION_MODE_ABSOLUTE;

	if (!m_ScrollWheelValue) 
	{ 
		m_ScrollWheelValue = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_MODIFY_STATE | SYNCHRONIZE); 
	}
	if (!m_RelativeRead) 
	{ 
		m_RelativeRead = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_MODIFY_STATE | SYNCHRONIZE); 
	}
	if (!m_AbsoluteMode)
	{ 
		m_AbsoluteMode = CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE); 
	}
	if (!m_RelativeMode) 
	{ 
		m_RelativeMode = CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE); 
	}

	m_LastX = 0;
	m_LastY = 0;
	m_RelativeX = INT32_MAX;
	m_RelativeY = INT32_MAX;

	m_InFocus = true;
	
	//ゲームパッド初期化
	m_Gamepads.resize(MAX_CONTROLLERS);//最大本数分確保
}

void Input::Uninit()
{
	// マウス終了処理
	SAFE_CLOSEHANDLE(m_ScrollWheelValue);
	SAFE_CLOSEHANDLE(m_RelativeRead);
	SAFE_CLOSEHANDLE(m_AbsoluteMode);
	SAFE_CLOSEHANDLE(m_RelativeMode);

	//ゲームパッド終了処理
	m_Gamepads.clear();
}

void Input::Update()
{
	//キーボード更新
	memcpy( m_OldKeyState, m_KeyState, 256 );
	GetKeyboardState( m_KeyState );

	//マウス更新
	Mouse_GetState();

	DWORD dwResult;

	//ゲームパッド更新
	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)//接続ポート確認
	{
		m_Gamepads[i].portNum = INVALID_ID;//無効で初期化

		dwResult = XInputGetState(i, &m_Gamepads[i].state);//接続番号代入

		if (dwResult == ERROR_SUCCESS)//接続成功時
		{
			if (m_Gamepads[i].portNum == INVALID_ID)//接続上書き防止
			{
				m_Gamepads[i].portNum = i;//接続番号代入
			}
		}
	}
	for (int i = 0; i < MAX_CONTROLLERS; i++)//接続ポート確認
	{
		if (m_Gamepads[i].state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//左スティック左入力
		{
			m_Gamepads[i].LEFT_STICK_X = STICK_LEFT;
		}
		else if (m_Gamepads[i].state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//左スティック右入力
		{
			m_Gamepads[i].LEFT_STICK_X = STICK_RIGHT;
		}
		else//左スティックX方向未入力
		{
			m_Gamepads[i].LEFT_STICK_X = STICK_NONE;
		}
		if (m_Gamepads[i].state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//左スティック上入力
		{
			m_Gamepads[i].LEFT_STICK_Y = STICK_UP;
		}
		else if (m_Gamepads[i].state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//左スティック下入力
		{
			m_Gamepads[i].LEFT_STICK_Y = STICK_DOWN;
		}
		else//左スティックY方向未入力
		{
			m_Gamepads[i].LEFT_STICK_Y = STICK_NONE;
		}
		if (m_Gamepads[i].state.Gamepad.sThumbRX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//右スティック左入力
		{
			m_Gamepads[i].RIGHT_STICK_X = STICK_LEFT;
		}
		else if (m_Gamepads[i].state.Gamepad.sThumbRX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//右スティック右入力
		{
			m_Gamepads[i].RIGHT_STICK_X = STICK_RIGHT;
		}
		else//右スティックX方向未入力
		{
			m_Gamepads[i].RIGHT_STICK_X = STICK_NONE;
		}
		if (m_Gamepads[i].state.Gamepad.sThumbRY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//右スティック上入力
		{
			m_Gamepads[i].RIGHT_STICK_Y = STICK_UP;
		}
		else if (m_Gamepads[i].state.Gamepad.sThumbRY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//右スティック下入力
		{
			m_Gamepads[i].RIGHT_STICK_Y = STICK_DOWN;
		}
		else//右スティックY方向未入力
		{
			m_Gamepads[i].RIGHT_STICK_Y = STICK_NONE;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)//Yボタン入力
		{
			m_Gamepads[i].Y_BUTTON = true;
		}
		else//Yボタン未入力
		{
			m_Gamepads[i].Y_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_X)//Xボタン入力
		{
			m_Gamepads[i].X_BUTTON = true;
		}
		else//Xボタン未入力
		{
			m_Gamepads[i].X_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_B)//Bボタン入力
		{
			m_Gamepads[i].B_BUTTON = true;
		}
		else//Bボタン未入力
		{
			m_Gamepads[i].B_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_A)//Aボタン入力
		{
			m_Gamepads[i].A_BUTTON = true;
		}
		else//Aボタン未入力
		{
			m_Gamepads[i].A_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_START)//START入力
		{
			m_Gamepads[i].START_BUTTON = true;
		}
		else//STARTボタン未入力
		{
			m_Gamepads[i].START_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)//BACKボタン入力
		{
			m_Gamepads[i].BACK_BUTTON = true;
		}
		else//BACKボタン未入力
		{
			m_Gamepads[i].BACK_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)//RBボタン入力
		{
			m_Gamepads[i].RB_BUTTON = true;
		}
		else//RBボタン未入力
		{
			m_Gamepads[i].RB_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)//LBボタン入力
		{
			m_Gamepads[i].LB_BUTTON = true;
		}
		else//LBボタン未入力
		{
			m_Gamepads[i].LB_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.bRightTrigger > 50)//RTボタン入力
		{
			m_Gamepads[i].RT_BUTTON = true;
		}
		else//RTボタン未入力
		{
			m_Gamepads[i].RT_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.bLeftTrigger > 50)//LTボタン入力
		{
			m_Gamepads[i].LT_BUTTON = true;
		}
		else//LTボタン未入力
		{
			m_Gamepads[i].LT_BUTTON = false;
		}
	}
}


//=============================================================================
//
// マウス
//
//=============================================================================
bool Input::GetButtonPress(EMouseButtonKind _kind) 
{
	switch (_kind)
	{
	case EMouseButtonKind::BUTTON_LEFT:

		return m_MouseState.leftButton;
		break;
	case EMouseButtonKind::BUTTON_RIGHT:

		return m_MouseState.rightButton;
		break;
	case EMouseButtonKind::BUTTON_MIDDLE:

		return m_MouseState.middleButton;
		break;
	}

	return false;
}


// マウスの状態取得
void Input::Mouse_GetState()
{
	m_MouseState.positionMode = m_Mode;

	DWORD result = WaitForSingleObjectEx(m_ScrollWheelValue, 0, FALSE);
	if (result == WAIT_FAILED)
	{ 
		return;
	}

	if (result == WAIT_OBJECT_0) 
	{

		m_MouseState.scrollWheelValue = 0;
	}

	if (m_MouseState.positionMode == MOUSE_POSITION_MODE_RELATIVE)
	{

		result = WaitForSingleObjectEx(m_RelativeRead, 0, FALSE);

		if (result == WAIT_FAILED)
		{ 
			return;
		}

		if (result == WAIT_OBJECT_0) 
		{
			m_MouseState.x = 0;
			m_MouseState.y = 0;
		}
		else
		{
			SetEvent(m_RelativeRead);
		}
	}
}


void Input::clipToWindow(void)
{
	assert(m_Window != NULL);

	RECT rect;
	GetClientRect(m_Window, &rect);

	POINT ul;
	ul.x = rect.left;
	ul.y = rect.top;

	POINT lr;
	lr.x = rect.right;
	lr.y = rect.bottom;

	MapWindowPoints(m_Window, NULL, &ul, 1);
	MapWindowPoints(m_Window, NULL, &lr, 1);

	rect.left = ul.x;
	rect.top = ul.y;

	rect.right = lr.x;
	rect.bottom = lr.y;

	ClipCursor(&rect);
}

// マウス制御のためのウィンドウメッセージプロシージャフック関数
void Input::Mouse_ProcessMessage(UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	HANDLE evts[3] = 
	{
		m_ScrollWheelValue,
		m_AbsoluteMode,
		m_RelativeMode
	};

	switch (WaitForMultipleObjectsEx(_countof(evts), evts, FALSE, 0, FALSE))
	{
	case WAIT_OBJECT_0:
		m_MouseState.scrollWheelValue = 0;
		ResetEvent(evts[0]);
		break;

	case (WAIT_OBJECT_0 + 1):
	{
		m_Mode = MOUSE_POSITION_MODE_ABSOLUTE;
		ClipCursor(nullptr);

		POINT point;
		point.x = m_LastX;
		point.y = m_LastY;

		// リモートディスクトップに対応するために移動前にカーソルを表示する
		ShowCursor(TRUE);

		if (MapWindowPoints(m_Window, nullptr, &point, 1)) 
		{
			SetCursorPos(point.x, point.y);
		}

		m_MouseState.x = m_LastX;
		m_MouseState.y = m_LastY;
	}
	break;

	case (WAIT_OBJECT_0 + 2):
	{
		ResetEvent(m_RelativeRead);

		m_Mode = MOUSE_POSITION_MODE_RELATIVE;
		m_MouseState.x = m_MouseState.y = 0;
		m_RelativeX = INT32_MAX;
		m_RelativeY = INT32_MAX;

		ShowCursor(FALSE);

		clipToWindow();
	}
	break;

	case WAIT_FAILED:
		return;
	}

	switch (_message)
	{
	case WM_ACTIVATEAPP:
		if (_wParam) {

			m_InFocus = true;

			if (m_Mode == MOUSE_POSITION_MODE_RELATIVE)
			{

				m_MouseState.x = m_MouseState.y = 0;
				ShowCursor(FALSE);
				clipToWindow();
			}
		}
		else {
			int scrollWheel = m_MouseState.scrollWheelValue;
			memset(&m_MouseState, 0, sizeof(m_MouseState));
			m_MouseState.scrollWheelValue = scrollWheel;
			m_InFocus = false;
		}
		return;

	case WM_INPUT:
		if (m_InFocus && m_Mode == MOUSE_POSITION_MODE_RELATIVE) 
		{

			RAWINPUT raw;
			UINT rawSize = sizeof(raw);

			GetRawInputData((HRAWINPUT)_lParam, RID_INPUT, &raw, &rawSize, sizeof(RAWINPUTHEADER));

			if (raw.header.dwType == RIM_TYPEMOUSE) 
			{

				if (!(raw.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE))
				{

					m_MouseState.x = raw.data.mouse.lLastX;
					m_MouseState.y = raw.data.mouse.lLastY;

					ResetEvent(m_RelativeRead);
				}
				else if (raw.data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) 
				{

					// リモートディスクトップなどに対応
					const int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
					const int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

					int x = (int)((raw.data.mouse.lLastX / 65535.0f) * width);
					int y = (int)((raw.data.mouse.lLastY / 65535.0f) * height);

					if (m_RelativeX == INT32_MAX) 
					{
						m_MouseState.x = m_MouseState.y = 0;
					}
					else 
					{
						m_MouseState.x = x - m_RelativeX;
						m_MouseState.y = y - m_RelativeY;
					}

					m_RelativeX = x;
					m_RelativeY = y;

					ResetEvent(m_RelativeRead);
				}
			}
		}
		return;


	case WM_MOUSEMOVE:
		break;

	case WM_LBUTTONDOWN:
		m_MouseState.leftButton = true;
		break;

	case WM_LBUTTONUP:
		m_MouseState.leftButton = false;
		break;

	case WM_RBUTTONDOWN:
		m_MouseState.rightButton = true;
		break;

	case WM_RBUTTONUP:
		m_MouseState.rightButton = false;
		break;

	case WM_MBUTTONDOWN:
		m_MouseState.middleButton = true;
		break;

	case WM_MBUTTONUP:
		m_MouseState.middleButton = false;
		break;

	case WM_MOUSEWHEEL:
		m_MouseState.scrollWheelValue += GET_WHEEL_DELTA_WPARAM(_wParam);
		return;

	case WM_XBUTTONDOWN:
		switch (GET_XBUTTON_WPARAM(_wParam))
		{
		case XBUTTON1:
			m_MouseState.xButton1 = true;
			break;

		case XBUTTON2:
			m_MouseState.xButton2 = true;
			break;
		}
		break;

	case WM_XBUTTONUP:
		switch (GET_XBUTTON_WPARAM(_wParam))
		{
		case XBUTTON1:
			m_MouseState.xButton1 = false;
			break;

		case XBUTTON2:
			m_MouseState.xButton2 = false;
			break;
		}
		break;

	case WM_MOUSEHOVER:
		break;

	default:
		// マウスに対するメッセージは無かった…
		return;
	}

	if (m_Mode == MOUSE_POSITION_MODE_ABSOLUTE)
	{

		// すべてのマウスメッセージに対して新しい座標を取得する
		int xPos = GET_X_LPARAM(_lParam);
		int yPos = GET_Y_LPARAM(_lParam);

		m_MouseState.x = m_LastX = xPos;
		m_MouseState.y = m_LastY = yPos;
	}
}
//-------------------------------
//マウスコンフィグ系
//-------------------------------

void Input::Mouse_ResetScrollWheelValue(void)
{
	SetEvent(m_ScrollWheelValue);
}

void Input::Mouse_SetMode(s_Mouse_PositionMode _mode)
{
	if (m_Mode == _mode)
	{//既に設定先と同じマウスモードならリターン
		return;
	}

	SetEvent((_mode == MOUSE_POSITION_MODE_ABSOLUTE) ? m_AbsoluteMode : m_RelativeMode);

	assert(m_Window != NULL);

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.dwFlags = TME_HOVER;
	tme.hwndTrack = m_Window;
	tme.dwHoverTime = 1;
	TrackMouseEvent(&tme);
}

void Input::Mouse_SetVisible(bool _visible)
{
	if (m_Mode == MOUSE_POSITION_MODE_RELATIVE) 
	{//相対座標モードならリターン
		return;
	}

	CURSORINFO info = { sizeof(CURSORINFO), 0, nullptr, {} };
	GetCursorInfo(&info);

	bool isVisible = (info.flags & CURSOR_SHOWING) != 0;

	if (isVisible != _visible) 
	{
		ShowCursor(_visible);
	}
}