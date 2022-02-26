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

	//�⊮�������f�[�^��ǉ�
	template<typename T>
	inline void AddComplement(float _start, float _end, float _playTime,
		float* _data, bool _isLoop,bool _isLoopReverse, const char* _timeLineName)
	{
		//���\�[�X���b�N
		Manager_Thread::Instance()->LockResource();

		//���f�[�^�ƒǉ��p�f�[�^�ɓ����f�[�^���Ȃ�������
		if (DataList.count(_timeLineName) > 0)return;

		//�ǉ��p�z��ɒǉ�
		T* temp = new T(_start, _end, _playTime, _data, _isLoop, _isLoopReverse);
		DataList[_timeLineName] = temp;
	}

	//�X���b�h�Ŏ��s����֐�
	static unsigned _stdcall UpdateForThread(void* _arg);
	inline bool IsWaitForSync()
	{ 
		Manager_Thread::Instance()->LockResource();
		if (isChanged)//�l���ύX���ꂽ��
		{
			isChanged = false;//�l���ύX����Ă��Ȃ���Ԃɂ���
			return false;//�l���X�V���ꂽ���Ƃ��m�F�����̂ŁA���ҋ@������
		}
		return true;//�l���ύX����Ă��Ȃ��̂ŗ��ғ��ӁB

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

	bool isChanged;//�l�̕ύX�����������ǂ���
	bool isFinished;//Uninit���Ă΂ꂽ���ǂ���
	class Time* cacheTime;
	std::unordered_map<const char*, Complement_Base*>DataList;//���f�[�^���X�g
};
