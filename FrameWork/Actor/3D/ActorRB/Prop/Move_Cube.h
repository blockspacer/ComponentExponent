#pragma once

#include "Cube.h"

class Move_Cube : public Cube
{
private:

	float speed;
	float moveSize;

public:

	Move_Cube() :Cube("ROM/2D/Material/T_Wood.png"){}

	void Init() override;
	void Update() override;

};