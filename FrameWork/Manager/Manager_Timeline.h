#pragma once

#include <unordered_map>
#include "Manager_Thread.h"
#include "../System/Singleton.h"
#include "../System/Complement/Complement_Base.h"

class Manager_Timeline : public Singleton<Manager_Timeline>
{
public:

	void Init();
	void Update();
	void Uninit();

	//補完したいデータを追加
	template<typename T>
	inline void AddComplement(float _start, float _end, float _playTime,
		float* _data, bool _isLoop,bool _isLoopReverse, const char* _timeLineName)
	{
		//リソースロック
		Manager_Thread::Instance()->LockResource();

		//実データと追加用データに同じデータがないか検索
		if (DataList.count(_timeLineName) > 0)return;

		//追加用配列に追加
		T* temp = new T(_start, _end, _playTime, _data, _isLoop, _isLoopReverse);
		DataList[_timeLineName] = temp;
	}

	//スレッドで実行する関数
	static unsigned _stdcall UpdateForThread(void* _arg);
	inline bool IsWaitForSync()
	{ 
		Manager_Thread::Instance()->LockResource();
		if (isChanged)//値が変更されたか
		{
			isChanged = false;//値が変更されていない状態にする
			return false;//値が更新されたことを確認したので、一回待機させる
		}
		return true;//値が変更されていないので両者同意。

		isChanged = false;
		return false;
	}
	inline void SetIsChanged()
	{
		Manager_Thread::Instance()->LockResource();
		isChanged = true;
	}
	bool GetIsFinished()const 
	{ 
		Manager_Thread::Instance()->LockResource();
		return isFinished;
	}

private:

	friend Singleton<Manager_Timeline>;

	bool isChanged;//値の変更があったかどうか
	bool isFinished;//Uninitが呼ばれたかどうか
	class Time* cacheTime;
	std::unordered_map<const char*, Complement_Base*>DataList;//実データリスト
};
