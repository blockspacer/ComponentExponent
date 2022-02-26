
#include "Main.h"
#include <time.h>
#include "Time.h"
#include "Exchanger.h"


int Time::GetCurrentHour()
{
	time_t now = time(nullptr);
	struct tm* pnow = localtime(&now);
	return pnow->tm_hour;
}

int Time::GetCurrentMinute()
{
	time_t now = time(nullptr);
	struct tm* pnow = localtime(&now);
	return pnow->tm_min;
}

int Time::GetCurrentSecond()
{
	time_t now = time(nullptr);
	struct tm* pnow = localtime(&now);
	return pnow->tm_sec;
}

double Time::GetMilliSecond()
{
	time_t now = time(nullptr);
	//return now * 0.001f;
	return now * 0.000001;
}

std::string Time::GetCurrentHourString()
{
	time_t now = time(nullptr);
	struct tm* pnow = localtime(&now);
	return Exchanger::INT_TO_STRING(pnow->tm_hour);
}

std::string Time::GetCurrentMinuteString()
{
	time_t now = time(nullptr);
	struct tm* pnow = localtime(&now);
	return Exchanger::INT_TO_STRING(pnow->tm_min);
}

std::string Time::GetCurrentSecondString()
{
	time_t now = time(nullptr);
	struct tm* pnow = localtime(&now);
	return Exchanger::INT_TO_STRING(pnow->tm_sec);
}