
#include "../Math.h"
#include "Complement_Lerp.h"
#include "../../Manager/Manager_Thread.h"


void Complement_Lerp::Update()
{
	Manager_Thread::Instance()->LockResource();

	*current = static_cast<float>(Math::lerpl(static_cast<double>(start), static_cast<double>(end), currentTime, playTime));
}
