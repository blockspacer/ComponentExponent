//=============================================================================
//
//  �e�X�g�V�[���p�v���C���[�R���g���[���[ [PC_Test.cpp]
//  Date   : 2021/11/20
//
//=============================================================================

#include "PC_Test.h"
#include "../Manager/Manager_Game.h"
#include "../System/Main.h"
#include "../System/MainWindow.h"
#include "../System/Math.h"
#include "../Level/Base/Level.h"
#include "../Level/Level_TP.h"
#include "../Actor/3D/ActorRB/Charcter/Base/Character.h"
#include "../Actor/3D/Camera.h"

void PC_Test::Init()
{
	PlayerController::Init();

	//�}�E�X���΍��W���[�h�ɕύX
	if (m_Input != nullptr)
	{
		m_Input->Mouse_SetMode(Input::MOUSE_POSITION_MODE_RELATIVE);
	}
}

void PC_Test::Update()
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


		if (m_Input->GetMouse_PositionMode() == Input::MOUSE_POSITION_MODE_RELATIVE)
		{//���΍��W���[�h�Ȃ�v���C���[����
			if (Camera* camera = Manager_Game::Instance()->GetLevel()->GetActor<Camera>())
			{
				if (Character* character = Manager_Game::Instance()->GetLevel()->GetActor<Character>())
				{//�L�����N�^�[�����x���ɑ��݂���ꍇ

					//=============================================================================
					//���ړ�
					//=============================================================================

					character->MoveAxis(MoveForward(), MoveRight());

					//=============================================================================
					//�J��������
					//=============================================================================

					//Yaw���J�����p�x
					if (camera->GetOrientRotationtoMovement())
					{//��Ƀv���C���[���J��������������
						character->MoveRotation(Turn());
					}
					else
					{//�I�[�\�h�b�N�X�ȃT�[�h�p�[�\���r���[
						camera->AddCameraYaw(Turn());
					}
					//Pitch���J�����p�x
					camera->AddCameraPitch(LookUp());

					//�J��������
					camera->AddTargetLength(Zoom());
			
					//=============================================================================
					//�A�N�V����
					//=============================================================================

					if (InputAction_Main())
					{//���C���A�N�V����
						
					}
					if (InputAction_Jump())
					{//�W�����v
						character->Jump();
					}

					//�X�v�����g
					character->Sprint(InputAction_Sprint());

				}
			}
		}
	}
}
