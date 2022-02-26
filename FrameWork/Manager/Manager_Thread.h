#pragma once

#include "../System/Main.h"
#include <unordered_map>
#include <process.h>
#include <mutex>
#include "../System/Singleton.h"


//�g�p���@
//1:unsigned int _stdcall �֐���(void*)�̊֐����`�B
//2:CreateThread�ŃX���b�h�Ɗ֐����֘A�t���Ď��s�B
//3:FinishThread�ŃX���b�h���I���ł���B
//3:�X���b�h�ɓo�^�����֐����A�֐��O�̏����Ɋ֌W������return����ꍇ�͓��ɒǉ��̊֐����ĂԕK�v�͂Ȃ��B
//4:�Ăяo���֐��̈����͈�ɂ܂Ƃ߂ēn���B�i��̕ϐ��Ȃ炻�̃|�C���^�A�����Ȃ�\���̂�N���X�̃|�C���^�œn���j
//5:FinishThread�ł́A������s_ThreadArg��isFinish��true�ɂ��ďI����ҋ@����̂ŁA
//isFinish�ŏI������l�ɏ����Ă��Ȃ��ꍇ�́AFinishThread���ĂԑO�Ɋ֐����I�������Ԃɂ���K�v������B
class Manager_Thread : public Singleton<Manager_Thread>
{
private:

	DWORD mainThreadID;//���C���X���b�hID
	std::mutex myMutex;//�~���[�e�b�N�X

public:

	//�֐��|�C���^�̖��O����������킩��Ղ����O��t����B
	typedef _beginthreadex_proc_type pThreadFunc;

	//�X���b�h�ɓn��
	struct s_ThreadArg
	{
		void* arg = nullptr;
		bool isFinish = false;
	};

	//�X���b�h�쐬�֐��@���s��false�����^�[���B
	//��Q����TRUE�Ńu���b�L���O
	bool CreateThread(pThreadFunc _func, s_ThreadArg _arg, const char* _threadName, bool isBlocking = false);

	//�X���b�h���I�����Ă��邩�m�F
	bool GetIsThreadSignalState(const char* _threadName);

	//�w�肵���X���b�h�̏I���ƍ폜�B
	//�������X���b�h���Ŏ��������̊֐��ō폜���悤�Ƃ���ƃG���[�B����
	//���C���X���b�h���Ŏ��s���邱�ƁB
	bool FinishThread(const char* _threadName);

	//���C���X���b�h�̔ԍ���ۑ�
	void Init();

	//�S�ẴX���b�h���I������܂őҋ@
	void Uninit();

	//������r������ɂ���B���̃X���b�h�Ƌ��L�̎������g���ꍇ�͕K�{�B
	//�������������g���O�̍s�ɏ����B������
	inline void LockResource() { std::lock_guard<std::mutex>lock(myMutex); }

	inline void LockStart() { myMutex.lock(); }//���b�N�J�n
	inline void LockEnd() { myMutex.unlock(); }//���b�N��������b�N������K��������邱�ƁB

	//�X���b�h���V�O�i����Ԃɂ���B
	//�������K���X���b�h�̊֐����ɏ����B���C���X���b�h�Ŏ��s����ƃv���O�����I���B������
	void ThreadToSignalState(void* _threadHandle);

	const HANDLE GetThreadHandle(const char* _threadName);
	const unsigned GetThreadID(const char* _threadName);

	void DeleteThreadFromList(const char* _threadName);

private:

	friend Singleton<Manager_Thread>;

	//�X���b�h�̏��
	struct s_ThreadData
	{
		pThreadFunc func;
		unsigned int threadID;
		HANDLE threadHandle;
		s_ThreadArg arg;//�X���b�h�ɓn�������̎���
	};
	std::unordered_map<const char*, s_ThreadData>threadList;
};