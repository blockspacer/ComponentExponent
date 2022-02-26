
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

	cacheTime = Time::Instance();//キャッシュ
	startTime = cacheTime->GetMilliSecond();//開始した時間取得
	currentTime = 0.0;//経過時間初期化
}

bool Complement_Base::GetIsFinished()
{
	if (isLoop)return false;//終わってない
	return currentTime >= playTime;
}

//経過時間更新
void Complement_Base::TimeUpdate()
{
	//リソースロック
	Manager_Thread::Instance()->LockResource();

	//経過時間更新
	currentTime += cacheTime->GetMilliSecond() - startTime;

	//実行時間が指定実行時間を超えた時
	if (currentTime >= playTime)
	{
		currentTime = playTime;

		//ループ設定ならリセットしてもう一度再生
		if (isLoop)
		{
			startTime = cacheTime->GetMilliSecond();

			//ループ時にstartとendを逆転するかどうか
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