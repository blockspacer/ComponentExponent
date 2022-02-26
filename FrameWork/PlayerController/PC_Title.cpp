//=============================================================================
//
//  �^�C�g���V�[���p�v���C���[�R���g���[���[ [PC_Title.cpp]
//  Date   : 2021/1/08
//
//=============================================================================

#include "PC_Title.h"
#include "../Manager/Manager_Game.h"
#include "../System/Main.h"
#include "../System/MainWindow.h"
#include "../System/Math.h"
#include "../Level/Base/Level.h"
#include "../Level/Level_TP.h"
#include "../Level/Level_TP.h"
#include "../Actor/3D/Camera.h"

constexpr float HALF_SPEED  = 0.5f;
constexpr float ACCEL_SPEED = 2.0f;

void PC_Title::Init()
{
	PlayerController::Init();

	//�}�E�X���΍��W���[�h�ɕύX
	if (m_Input != nullptr)
	{
		m_Input->Mouse_SetMode(Input::MOUSE_POSITION_MODE_ABSOLUTE);
	}
}

void PC_Title::Update()
{
	if (m_Input != nullptr)
	{
		if (InputAction_Pause())
		{//�|�[�Y����
			if (m_Input->GetMouse_PositionMode() == Input::MOUSE_POSITION_MODE_RELATIVE)
			{
				m_Input->Mouse_SetMode(Input::MOUSE_POSITION_MODE_ABSOLUTE);
			}
			else
			{
				m_Input->Mouse_SetMode(Input::MOUSE_POSITION_MODE_RELATIVE);
			}
		}
		if (InputAction_Main())
		{//���C���A�N�V����
			Manager_Game::Instance()->LoadLevelByClass<Level_Test>(2.0f);//�e�X�g�V�[���Z�b�g
		}

		if (m_Input->GetMouse_PositionMode() == Input::MOUSE_POSITION_MODE_RELATIVE)
		{//���΍��W���[�h�Ȃ�v���C���[����
			if (Camera* camera = Manager_Game::Instance()->GetLevel()->GetActor<Camera>())
			{
		
				float velocity = HALF_SPEED;

				if (InputAction_Main())
				{
					velocity = velocity * ACCEL_SPEED;
				}
				if (InputAction_Sprint())
				{
					velocity = velocity * ACCEL_SPEED;
				}

				//�J�����ړ�
				camera->Move(MoveForward() * velocity, MoveRight() * velocity);

				//Yaw���J�����p�x
				camera->AddCameraYaw(Turn());
				//Pitch���J�����p�x
				camera->AddCameraPitch(LookUp());

			}
		}
	}
}
