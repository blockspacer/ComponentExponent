//=//=============================================================================
//
//  タイトルシーン用プレイヤーコントローラー [PC_Title.h]
//  Date   : 2021/1/8
//
//=============================================================================
#pragma once

#include "Base/PlayerController.h"

class PC_Title : public PlayerController
{

public:

	void Init()override;
	void Update()override;

};
