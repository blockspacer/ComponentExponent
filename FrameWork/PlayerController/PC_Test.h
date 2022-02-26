//=//=============================================================================
//
//  テストシーン用プレイヤーコントローラー [PC_Test.h]
//  Date   : 2021/11/20
//
//=============================================================================
#pragma once

#include "Base/PlayerController.h"

class PC_Test : public PlayerController
{

public:

	void Init()override;
	void Update()override;

};
