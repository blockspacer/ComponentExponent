//=============================================================================
//
//  プレイヤーコントローラー [PlayerConroller.h]
//  Date   : 2021/10/13
//
//=============================================================================
#pragma once

#include "../../System/Input.h"

class PlayerController
{
protected :

	class Input* m_Input;          //キャッシュ
	class MainWindow* m_MainWindow;//キャッシュ

	float m_Mouse_Sensi   = 0.002f;//マウス感度
	float m_Gamepad_Sensi = 0.04f;//ゲームパッド感度

	//前後移動
	float MoveForward();

	//左右移動
	float MoveRight();
	
	//カメラ左右
	float Turn();
	
	//カメラ上下
	float LookUp();

	//カメラズーム
	float Zoom();

	//メインアクション割り当て
	bool InputAction_Main();

	//スプリント
	bool InputAction_Sprint();

	//ジャンプ
	bool InputAction_Jump();

	//ポーズ
	bool InputAction_Pause();

public:

	virtual void Init();//初期化
	virtual void Update() = 0;//更新

};
