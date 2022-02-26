//=============================================================================
//
//  回転キューブクラス [Spin_Cube.h]
//  Date   : 2021/12/4
//
//=============================================================================
#pragma once


#include "Cube.h"

class Spin_Cube : public Cube
{
private:

	float rot;

public:

	virtual void Init()override;//初期化
	virtual void Update()override;//更新
	void Uninit() override;


};