//=============================================================================
//
//  テストシーン用プレイヤーコントローラー [PC_Test.cpp]
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

	//マウス相対座標モードに変更
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
		{//ポーズ処理
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
		{//相対座標モードならプレイヤー操作
			if (Camera* camera = Manager_Game::Instance()->GetLevel()->GetActor<Camera>())
			{
				if (Character* character = Manager_Game::Instance()->GetLevel()->GetActor<Character>())
				{//キャラクターがレベルに存在する場合

					//=============================================================================
					//軸移動
					//=============================================================================

					character->MoveAxis(MoveForward(), MoveRight());

					//=============================================================================
					//カメラ制御
					//=============================================================================

					//Yaw軸カメラ角度
					if (camera->GetOrientRotationtoMovement())
					{//常にプレイヤーがカメラ方向を向く
						character->MoveRotation(Turn());
					}
					else
					{//オーソドックスなサードパーソンビュー
						camera->AddCameraYaw(Turn());
					}
					//Pitch軸カメラ角度
					camera->AddCameraPitch(LookUp());

					//カメラ距離
					camera->AddTargetLength(Zoom());
			
					//=============================================================================
					//アクション
					//=============================================================================

					if (InputAction_Main())
					{//メインアクション
						
					}
					if (InputAction_Jump())
					{//ジャンプ
						character->Jump();
					}

					//スプリント
					character->Sprint(InputAction_Sprint());

				}
			}
		}
	}
}
