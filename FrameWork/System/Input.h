//=============================================================================
//
//  �L�[�{�[�h&�}�E�X&�Q�[���p�b�h [Input.cpp]
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
	// �}�E�X�n
	enum EMouseButtonKind
	{
		BUTTON_NONE = 0,
		BUTTON_LEFT,
		BUTTON_RIGHT,
		BUTTON_MIDDLE,
		BUTTON_MAX
	};

	// �}�E�X���W���[�h
	typedef enum EMouse_PositionMode_tag
	{
		MOUSE_POSITION_MODE_ABSOLUTE, // ��΍��W���[�h
		MOUSE_POSITION_MODE_RELATIVE, // ���΍��W���[�h
	} s_Mouse_PositionMode;


	// �}�E�X��ԍ\����
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

	//�Q�[���p�b�h�n
	typedef struct 
	{
		XINPUT_STATE state;
		int portNum;//�|�[�h�ԍ�

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
		STICK_LEFT, //���|��
		STICK_RIGHT,//�E�|��
		STICK_UP,   //��|��
		STICK_DOWN  //���|��
	};

	enum EGamepadNum
	{
		GAMEPAD_1 = 0,//�Q�[���p�b�h1
		GAMEPAD_2,    //�Q�[���p�b�h2
		MAX_CONTROLLERS//�Q�[���p�b�h�ő�ڑ���
	};
	

private:
	friend class Singleton<Input>;//�V���O���g��

	//�L�[�{�[�h
	BYTE m_OldKeyState[256];
	BYTE m_KeyState[256];
	s_Mouse_State m_MouseState;

	//�}�E�X
	void Mouse_GetState();
	void clipToWindow(void);

	HWND               m_Window = NULL;
	s_Mouse_PositionMode m_Mode = MOUSE_POSITION_MODE_ABSOLUTE;//���΍��W���[�hor��΍��W���[�h�i�f�t�H���g�͐�΍��W���[�h�j
	HANDLE             m_ScrollWheelValue = NULL;
	HANDLE             m_RelativeRead = NULL;
	HANDLE             m_AbsoluteMode = NULL;
	HANDLE             m_RelativeMode = NULL;
	int                m_LastX = 0;
	int                m_LastY = 0;
	int                m_RelativeX = INT32_MAX;
	int                m_RelativeY = INT32_MAX;
	bool               m_InFocus = true;

	//�Q�[���p�b�h
	std::vector<s_Gamepad_State> m_Gamepads;//���L�Q�[���p�b�h

public:

	void Init(HWND _window);//������
	void Uninit();//�I��
	void Update();//�X�V

	//�L�[�{�[�h
	inline bool GetKeyPress(BYTE _KeyCode)     const { return (m_KeyState[_KeyCode] & 0x80); };
	inline bool GetKeyTrigger( BYTE _KeyCode ) const { return ((m_KeyState[_KeyCode] & 0x80) && !(m_OldKeyState[_KeyCode] & 0x80));}

	//�}�E�X
	inline s_Mouse_State GetMouse_State() const { return m_MouseState; }//�擾
	void Mouse_ProcessMessage(UINT _message, WPARAM _wParam, LPARAM _lParam);
	inline bool GetButtonPress(EMouseButtonKind _kind);
	//�}�E�X�R���t�B�O�n
	void Mouse_ResetScrollWheelValue(void);//�ݐς����}�E�X�X�N���[���z�C�[���l�����Z�b�g����
	void Mouse_SetMode(s_Mouse_PositionMode _mode);//�}�E�X�̃|�W�V�������[�h��ݒ肷��
	void Mouse_SetVisible(bool _visible);//�}�E�X�J�[�\���\����ݒ肷��
	inline s_Mouse_PositionMode GetMouse_PositionMode() const { return m_Mode; }//���݂̍��W���[�h�擾

	//�Q�[���p�b�h
	inline s_Gamepad_State GetGamepad_State(int _num) const { return m_Gamepads[_num]; }//��Ԏ擾�֐��B�����̓Q�[���p�b�henum

};
