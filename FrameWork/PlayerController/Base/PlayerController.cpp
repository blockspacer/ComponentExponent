//=============================================================================
//
//  �v���C���[�R���g���[���[ [PlayerConroller.h]
//  Date   : 2021/12/24 oh.....
//
//=============================================================================

#include "PlayerController.h"
#include "../../System/Math.h"
#include "../../System/MainWindow.h"

constexpr float EFFECTIVE_VALUE = 1.0f;
constexpr float ZOOM_SPEED      = 0.25f;

void PlayerController::Init()
{
	//�C���v�b�g�L���b�V��
	m_Input = Input::Instance();

	//Window�L���b�V��
	m_MainWindow = MainWindow::Instance();
}


//�O��ړ�
float PlayerController::MoveForward()
{
	float value = 0.0f;

	if (m_Input != nullptr)
	{
		if (m_Input->GetKeyPress('W') || m_Input->GetGamepad_State(Input::GAMEPAD_1).LEFT_STICK_Y == Input::STICK_UP)
		{//�O�i
			value += EFFECTIVE_VALUE;
		}
		if (m_Input->GetKeyPress('S') || m_Input->GetGamepad_State(Input::GAMEPAD_1).LEFT_STICK_Y == Input::STICK_DOWN)
		{//��i
			value -= EFFECTIVE_VALUE;
		}
	}
	return value;
}

//���E�ړ�
float PlayerController::MoveRight()
{
	float value = 0.0f;

	if (m_Input != nullptr)
	{
		if (m_Input->GetKeyPress('D') || m_Input->GetGamepad_State(Input::GAMEPAD_1).LEFT_STICK_X == Input::STICK_RIGHT)
		{//�E�ړ�
			value += EFFECTIVE_VALUE;
		}

		if (m_Input->GetKeyPress('A') || m_Input->GetGamepad_State(Input::GAMEPAD_1).LEFT_STICK_X == Input::STICK_LEFT)
		{//���ړ�
			value -= EFFECTIVE_VALUE;
		}
	}
	return value;
}

//�J�������E
float PlayerController::Turn()
{
	float value = 0.0f;

	if (m_Input != nullptr)
	{
		//�p�b�h
		if (m_Input->GetGamepad_State(Input::GAMEPAD_1).RIGHT_STICK_X == Input::STICK_RIGHT)
		{//�E����
			return  m_Gamepad_Sensi;
		}
		else if (m_Input->GetGamepad_State(Input::GAMEPAD_1).RIGHT_STICK_X == Input::STICK_LEFT)
		{//������
			return -m_Gamepad_Sensi;
		}
		//�}�E�X
		if (m_Input->GetMouse_State().x > HalfSize(m_MainWindow->GetWindowSize().x))
		{//�E��]
			value -= static_cast<float>(m_Input->GetMouse_State().x) * m_Mouse_Sensi;
		}
		if (m_Input->GetMouse_State().x < HalfSize(m_MainWindow->GetWindowSize().x))
		{//����]
			value += static_cast<float>(m_Input->GetMouse_State().x) * m_Mouse_Sensi;
		}
	
	}
	return value;
}
//�J�����㉺
float PlayerController::LookUp()
{
	if (m_Input != nullptr)
	{
		//�p�b�h
		if (m_Input->GetGamepad_State(Input::GAMEPAD_1).RIGHT_STICK_Y == Input::STICK_DOWN)
		{//������
			return  m_Gamepad_Sensi;
		}
		else if (m_Input->GetGamepad_State(Input::GAMEPAD_1).RIGHT_STICK_Y == Input::STICK_UP)
		{//�����
			return -m_Gamepad_Sensi;
		}
		//�}�E�X
		else if (m_Input->GetMouse_State().y > HalfSize(m_MainWindow->GetWindowSize().y))	//�}�E�X
		{//����]
			return  -static_cast<float>(m_Input->GetMouse_State().y) * m_Mouse_Sensi;
		}
		else
		{//���]
			return   static_cast<float>(m_Input->GetMouse_State().y) * m_Mouse_Sensi;
		}
	}
	return 0.0f;
}
//�J�����Y�[��
float PlayerController::Zoom()
{
	if (m_Input != nullptr)
	{
		if (m_Input->GetMouse_State().scrollWheelValue > 0)
		{//�}�E�X�z�C�[���e�X�g�Ń|�[�Y����
			m_Input->Mouse_ResetScrollWheelValue();
			return -ZOOM_SPEED;
		}
		else if (m_Input->GetMouse_State().scrollWheelValue < 0)
		{//�}�E�X�z�C�[���e�X�g�Ń|�[�Y����
			m_Input->Mouse_ResetScrollWheelValue();
			return  ZOOM_SPEED;
		}
		else if (m_Input->GetGamepad_State(Input::GAMEPAD_1).LT_BUTTON)
		{
			return  ZOOM_SPEED;
		}
		else if (m_Input->GetGamepad_State(Input::GAMEPAD_1).RT_BUTTON)
		{
			return  -ZOOM_SPEED;
		}
	}
	return 0.0f;
}

//���C���A�N�V�������蓖��
bool PlayerController::InputAction_Main()
{
	if (m_Input != nullptr)
	{
		if (m_Input->GetMouse_State().leftButton || m_Input->GetGamepad_State(Input::GAMEPAD_1).B_BUTTON)
		{
			return true;
		}
	}
	return false;
}

//�X�v�����g
bool PlayerController::InputAction_Sprint()
{
	if (m_Input != nullptr)
	{
		if (m_Input->GetKeyPress(VK_SHIFT) || m_Input->GetGamepad_State(Input::GAMEPAD_1).LB_BUTTON
			|| m_Input->GetMouse_State().rightButton)
		{
			return true;
		}
	}
	return false;
}

//�W�����v
bool PlayerController::InputAction_Jump()
{
	if (m_Input != nullptr)
	{
		if (m_Input->GetKeyTrigger(VK_SPACE) || m_Input->GetGamepad_State(Input::GAMEPAD_1).A_BUTTON)
		{
			return true;
		}
	}
	return false;
}

//�|�[�Y
bool PlayerController::InputAction_Pause()
{
	if (m_Input != nullptr)
	{
		if (m_Input->GetKeyTrigger(VK_CONTROL) || m_Input->GetGamepad_State(Input::GAMEPAD_1).START_BUTTON)
		{
			return true;
		}
	}
	return false;
}