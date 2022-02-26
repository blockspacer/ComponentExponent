#pragma once

#include "Complement_Base.h"

class Complement_Lerp : public Complement_Base
{
public:

	Complement_Lerp(float _start, float _end, float _playTime, float* _out, bool _isLoop = false,bool _isLoopRev = false)
		: Complement_Base(_start,_end,_playTime,_out,_isLoop,_isLoopRev) {};

	void Update() override;
};