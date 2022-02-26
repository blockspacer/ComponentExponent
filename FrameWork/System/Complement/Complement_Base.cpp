
#include "../Time.h"
#include "Complement_Base.h"
#include "../../Manager/Manager_Thread.h"


Complement_Base::Complement_Base(float _start, float _end, float _playTime, float * _out, bool _isLoop,bool _isLoopReverse)
{
	start = _start;
	end = _end;
	playTime = _playTime;
	current = _out;
	isLoop = _isLoop;
	isLoopRevers = _isLoopReverse;

	cacheTime = Time::Instance();//�L���b�V��
	startTime = cacheTime->GetMilliSecond();//�J�n�������Ԏ擾
	currentTime = 0.0;//�o�ߎ��ԏ�����
}

bool Complement_Base::GetIsFinished()
{
	if (isLoop)return false;//�I����ĂȂ�
	return currentTime >= playTime;
}

//�o�ߎ��ԍX�V
void Complement_Base::TimeUpdate()
{
	//���\�[�X���b�N
	Manager_Thread::Instance()->LockResource();

	//�o�ߎ��ԍX�V
	currentTime += cacheTime->GetMilliSecond() - startTime;

	//���s���Ԃ��w����s���Ԃ𒴂�����
	if (currentTime >= playTime)
	{
		currentTime = playTime;

		//���[�v�ݒ�Ȃ烊�Z�b�g���Ă�����x�Đ�
		if (isLoop)
		{
			startTime = cacheTime->GetMilliSecond();

			//���[�v����start��end���t�]���邩�ǂ���
			if (isLoopRevers)
			{
				*current = end;
				float temp = end;
				end = start;
				start = temp;
				currentTime = 0.0;
			}
			else
			{
				*current = start;
				currentTime = 0.0;
			}
		}
	}
}