//=============================================================================
//
//  ÉeÉXÉg [Level_TP.h]
//  Date   : 2021/11/03
//
//=============================================================================
#pragma once

#include "Base/Level.h"

class Level_Test : public Level
{
public:
	void Init()override;
	void Uninit()override;
	void Update()override;

	void AddScore();

private:

	bool isClear;
	int clearCount;

	int coinCount;
	std::string coinCountText;
	Vector2 coinTextPos;

	std::string clearText;
	Vector2 clearTextPos;

	std::string option;//ê‡ñæ
	Vector2 optionTextPos;

};