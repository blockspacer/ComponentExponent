//=============================================================================
//
//  ƒ^ƒCƒgƒ‹ [Level_Title.h]
//  Date   : 2021/1/09
//
//=============================================================================
#pragma once

#include "Base/Level.h"

class Level_Title : public Level
{
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
private:

	int moveSceneCount;
};