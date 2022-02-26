//=============================================================================
//
//  �L�[�{�[�h&�}�E�X&�Q�[���p�b�h [Input.cpp]
//  Date   : 2021/11/2
//
//=============================================================================

#include "Input.h"

#define SAFE_CLOSEHANDLE(h) if(h){CloseHandle(h); h = NULL;}

void Input::Init(HWND _window)
{
	//Window�擾
	m_Window = _window;

	//�L�[�{�[�h������
	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );

	// �}�E�X�̏���������
	RtlZeroMemory(&m_MouseState, sizeof(m_MouseState));

	assert(_window != NULL);

	RAWINPUTDEVICE Rid;
	Rid.usUsagePage = 0x01 /* HID_USAGE_PAGE_GENERIC */;
	Rid.usUsage = 0x02     /* HID_USAGE_GENERIC_MOUSE */;
	Rid.dwFlags = RIDEV_INPUTSINK;
	Rid.hwndTarget = _window;
	RegisterRawInputDevices(&Rid, 1, sizeof(RAWINPUTDEVICE));

	//��΍��W���[�h�ɐݒ�
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
	
	//�Q�[���p�b�h������
	m_Gamepads.resize(MAX_CONTROLLERS);//�ő�{�����m��
}

void Input::Uninit()
{
	// �}�E�X�I������
	SAFE_CLOSEHANDLE(m_ScrollWheelValue);
	SAFE_CLOSEHANDLE(m_RelativeRead);
	SAFE_CLOSEHANDLE(m_AbsoluteMode);
	SAFE_CLOSEHANDLE(m_RelativeMode);

	//�Q�[���p�b�h�I������
	m_Gamepads.clear();
}

void Input::Update()
{
	//�L�[�{�[�h�X�V
	memcpy( m_OldKeyState, m_KeyState, 256 );
	GetKeyboardState( m_KeyState );

	//�}�E�X�X�V
	Mouse_GetState();

	DWORD dwResult;

	//�Q�[���p�b�h�X�V
	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)//�ڑ��|�[�g�m�F
	{
		m_Gamepads[i].portNum = INVALID_ID;//�����ŏ�����

		dwResult = XInputGetState(i, &m_Gamepads[i].state);//�ڑ��ԍ����

		if (dwResult == ERROR_SUCCESS)//�ڑ�������
		{
			if (m_Gamepads[i].portNum == INVALID_ID)//�ڑ��㏑���h�~
			{
				m_Gamepads[i].portNum = i;//�ڑ��ԍ����
			}
		}
	}
	for (int i = 0; i < MAX_CONTROLLERS; i++)//�ڑ��|�[�g�m�F
	{
		if (m_Gamepads[i].state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//���X�e�B�b�N������
		{
			m_Gamepads[i].LEFT_STICK_X = STICK_LEFT;
		}
		else if (m_Gamepads[i].state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//���X�e�B�b�N�E����
		{
			m_Gamepads[i].LEFT_STICK_X = STICK_RIGHT;
		}
		else//���X�e�B�b�NX����������
		{
			m_Gamepads[i].LEFT_STICK_X = STICK_NONE;
		}
		if (m_Gamepads[i].state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//���X�e�B�b�N�����
		{
			m_Gamepads[i].LEFT_STICK_Y = STICK_UP;
		}
		else if (m_Gamepads[i].state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//���X�e�B�b�N������
		{
			m_Gamepads[i].LEFT_STICK_Y = STICK_DOWN;
		}
		else//���X�e�B�b�NY����������
		{
			m_Gamepads[i].LEFT_STICK_Y = STICK_NONE;
		}
		if (m_Gamepads[i].state.Gamepad.sThumbRX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//�E�X�e�B�b�N������
		{
			m_Gamepads[i].RIGHT_STICK_X = STICK_LEFT;
		}
		else if (m_Gamepads[i].state.Gamepad.sThumbRX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//�E�X�e�B�b�N�E����
		{
			m_Gamepads[i].RIGHT_STICK_X = STICK_RIGHT;
		}
		else//�E�X�e�B�b�NX����������
		{
			m_Gamepads[i].RIGHT_STICK_X = STICK_NONE;
		}
		if (m_Gamepads[i].state.Gamepad.sThumbRY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//�E�X�e�B�b�N�����
		{
			m_Gamepads[i].RIGHT_STICK_Y = STICK_UP;
		}
		else if (m_Gamepads[i].state.Gamepad.sThumbRY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//�E�X�e�B�b�N������
		{
			m_Gamepads[i].RIGHT_STICK_Y = STICK_DOWN;
		}
		else//�E�X�e�B�b�NY����������
		{
			m_Gamepads[i].RIGHT_STICK_Y = STICK_NONE;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)//Y�{�^������
		{
			m_Gamepads[i].Y_BUTTON = true;
		}
		else//Y�{�^��������
		{
			m_Gamepads[i].Y_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_X)//X�{�^������
		{
			m_Gamepads[i].X_BUTTON = true;
		}
		else//X�{�^��������
		{
			m_Gamepads[i].X_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_B)//B�{�^������
		{
			m_Gamepads[i].B_BUTTON = true;
		}
		else//B�{�^��������
		{
			m_Gamepads[i].B_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_A)//A�{�^������
		{
			m_Gamepads[i].A_BUTTON = true;
		}
		else//A�{�^��������
		{
			m_Gamepads[i].A_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_START)//START����
		{
			m_Gamepads[i].START_BUTTON = true;
		}
		else//START�{�^��������
		{
			m_Gamepads[i].START_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)//BACK�{�^������
		{
			m_Gamepads[i].BACK_BUTTON = true;
		}
		else//BACK�{�^��������
		{
			m_Gamepads[i].BACK_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)//RB�{�^������
		{
			m_Gamepads[i].RB_BUTTON = true;
		}
		else//RB�{�^��������
		{
			m_Gamepads[i].RB_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)//LB�{�^������
		{
			m_Gamepads[i].LB_BUTTON = true;
		}
		else//LB�{�^��������
		{
			m_Gamepads[i].LB_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.bRightTrigger > 50)//RT�{�^������
		{
			m_Gamepads[i].RT_BUTTON = true;
		}
		else//RT�{�^��������
		{
			m_Gamepads[i].RT_BUTTON = false;
		}
		if (m_Gamepads[i].state.Gamepad.bLeftTrigger > 50)//LT�{�^������
		{
			m_Gamepads[i].LT_BUTTON = true;
		}
		else//LT�{�^��������
		{
			m_Gamepads[i].LT_BUTTON = false;
		}
	}
}


//=============================================================================
//
// �}�E�X
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


// �}�E�X�̏�Ԏ擾
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

// �}�E�X����̂��߂̃E�B���h�E���b�Z�[�W�v���V�[�W���t�b�N�֐�
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

		// �����[�g�f�B�X�N�g�b�v�ɑΉ����邽�߂Ɉړ��O�ɃJ�[�\����\������
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

					// �����[�g�f�B�X�N�g�b�v�ȂǂɑΉ�
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
		// �}�E�X�ɑ΂��郁�b�Z�[�W�͖��������c
		return;
	}

	if (m_Mode == MOUSE_POSITION_MODE_ABSOLUTE)
	{

		// ���ׂẴ}�E�X���b�Z�[�W�ɑ΂��ĐV�������W���擾����
		int xPos = GET_X_LPARAM(_lParam);
		int yPos = GET_Y_LPARAM(_lParam);

		m_MouseState.x = m_LastX = xPos;
		m_MouseState.y = m_LastY = yPos;
	}
}
//-------------------------------
//�}�E�X�R���t�B�O�n
//-------------------------------

void Input::Mouse_ResetScrollWheelValue(void)
{
	SetEvent(m_ScrollWheelValue);
}

void Input::Mouse_SetMode(s_Mouse_PositionMode _mode)
{
	if (m_Mode == _mode)
	{//���ɐݒ��Ɠ����}�E�X���[�h�Ȃ烊�^�[��
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
	{//���΍��W���[�h�Ȃ烊�^�[��
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