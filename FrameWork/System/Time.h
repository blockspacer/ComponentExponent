#pragma once

#include <string>
#include "Singleton.h"

class Time : public Singleton<Time>
{
private:

	friend Singleton<Time>;
	float timeScale;

public:

	Time() :timeScale(0.0f) {}

	inline float GetTimeScale()const { return timeScale; }
	inline void SetTimeScale(float _set) { timeScale = _set; }

	int GetCurrentHour();
	int GetCurrentMinute();
	int GetCurrentSecond();
	double GetMilliSecond();
	std::string GetCurrentHourString();
	std::string GetCurrentMinuteString();
	std::string GetCurrentSecondString();
};