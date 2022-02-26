
#include "Manager_Thread.h"
#include "../Manager/Manager_Drawer/Manager_ImGui.h"

bool Manager_Thread::CreateThread(pThreadFunc _func, s_ThreadArg _arg, const char* _threadName, bool isBlocking)
{
	//�������ɓ����̃X���b�h�����s���Ȃ�쐬���Ȃ�
	if (threadList.count(_threadName) > 0)
	{
		Manager_ImGui::Instance()->GetMainDebugWindow()->DebugLog(_threadName + std::string(" Thread is Already Run"));
		return false;
	}

	//�\���̏�����
	s_ThreadData threadData = {};
	threadData.func = _func;
	threadData.arg = _arg;
	threadList[_threadName] = threadData;//�X���b�h���X�g�ɓo�^

	//�X���b�h�쐬
	threadList[_threadName].threadHandle = (HANDLE)_beginthreadex(NULL, 0, _func, &threadList[_threadName].arg, 0, &threadList[_threadName].threadID);

	//�X���b�h�쐬�G���[
	if (threadList[_threadName].threadHandle == nullptr)
	{
		char temp[512] = {};
		sprintf_s(temp, sizeof(temp), "%s �X���b�h�쐬�@���s�B������߂�l�ɖ�肪���邩������܂���B", _threadName);
		MessageBox(NULL, temp, "ERROR", MB_OK);

		//�o�^������
		threadList.erase(_threadName);

		return false;
	}

	//�X���b�h���s
	if (threadData.threadHandle != 0)
	{
		ResumeThread(threadData.threadHandle);
	}

	if (isBlocking)//�u���b�L���O�ݒ�Ȃ�
	{
		//�I������܂őҋ@
		WaitForSingleObject(__threadhandle, INFINITE);

		FinishThread(_threadName);//�X���b�h�폜
	}

	//�쐬����
	return true;
}

bool Manager_Thread::GetIsThreadSignalState(const char * _threadName)
{
	if (threadList.count(_threadName) == 0)return true;//�X���b�h���o�^

	//�I���m�F
	DWORD threadState = WaitForSingleObject(threadList[_threadName].threadHandle, 0);
	if (threadState == WAIT_OBJECT_0)return true;//�X���b�h���V�O�i�����

	//�X���b�h���V�O�i����Ԃ���Ȃ������琶���Ă��邩�G���[�B
	return false;
}

bool Manager_Thread::FinishThread(const char * _threadName)
{
	if (threadList.count(_threadName) == 0)return false;//�X���b�h���o�^

	//�X���b�h�̊֐��Ŏg���Ă��邩������Ȃ��I���t���O�𗧂Ă�
	threadList[_threadName].arg.isFinish = true;

	HANDLE temp = threadList[_threadName].threadHandle;

	//�҂����Ƃ��X���b�h���V�O�i����ԂȂ炷���ɃX���b�h���폜����
	if (WaitForSingleObject(threadList[_threadName].threadHandle, 0) == WAIT_OBJECT_0)
	{
		//�X���b�h�폜
		if (!CloseHandle(threadList[_threadName].threadHandle))
		{
			//�폜���s
			DWORD err = GetLastError();
			std::string errtext = "CloseHandle���s�@�R�[�h:" + err;
			MessageBox(NULL, errtext.c_str(), "ERROR", MB_OK);
		}
		return true;
	}

	//�܂��X���b�h����V�O�i����Ԃ��A�G���[�ŏI�������肵�Ă���ꍇ�͏I���܂őҋ@����
	DWORD threadState = WaitForSingleObject(threadList[_threadName].threadHandle, INFINITE);
	if (threadState == WAIT_OBJECT_0)
	{
		//�X���b�h�폜
		if (!CloseHandle(threadList[_threadName].threadHandle))
		{
			//�폜���s
			DWORD err = GetLastError();
			std::string errtext = "CloseHandle���s�@�R�[�h:" + err;
			MessageBox(NULL, errtext.c_str(), "ERROR", MB_OK);
		}
		return true;
	}
	else if (threadState == WAIT_ABANDONED)
	{
		MessageBox(nullptr, "�X���b�h�Ń��b�N���Ă������̂�������ꂸ�I���B���b�N�����������ĉ������B", "ERROR", MB_OK);
		return false;
	}
	else if (threadState == WAIT_FAILED)
	{
		DWORD errCode = GetLastError();
		char temp[1024] = {};
		sprintf_s(temp, sizeof(temp), "�X���b�h�I���G���[�@�R�[�h:%d", errCode);
		MessageBox(nullptr, temp, "ERROR", MB_OK);
		return false;
	}
	else if (threadState == WAIT_TIMEOUT)
	{
		MessageBox(nullptr, (std::string(_threadName) + std::string(" :�֐��̏I���m�F���^�C���A�E�g�B�I�������������ĉ������B")).c_str(), "ERROR", MB_OK);
		return false;
	}

	//�҂������ǏI�����Ȃ�
	char msg[512] = {};
	sprintf_s(msg, sizeof(msg), "�X���b�h���I�����܂���B�X���b�h�̏I�������������ĉ������B�X���b�h��:%s", _threadName);
	MessageBox(NULL, msg, "ERROR", MB_OK);
	return false;
}

void Manager_Thread::Init()
{
	mainThreadID = GetCurrentThreadId();
}

void Manager_Thread::Uninit()
{
	for(auto itr = threadList.begin();itr != threadList.end();itr++)
	{
		if (FinishThread(itr->first))
		{
			//�폜
			itr = threadList.erase(itr);
		}
	}
	threadList.clear();
}

void Manager_Thread::ThreadToSignalState(void * _threadHandle)
{
	DWORD exitCode = 0;
	if (!GetExitCodeThread(static_cast<HANDLE>(_threadHandle), &exitCode))//�I���R�[�h���擾
	{
		DWORD errCode = GetLastError();
		char errTxt[256] = {};
		sprintf_s(errTxt, sizeof(errTxt), "�X���b�h�I���R�[�h�擾�G���[�@�R�[�h:%d", errCode);
		MessageBox(NULL, errTxt, "ERROR", MB_OK);
		return;
	}

	if (mainThreadID != GetCurrentThreadId())
	{
		_endthreadex(exitCode);//�X���b�h���V�O�i����Ԃɂ���
	}
}

const HANDLE Manager_Thread::GetThreadHandle(const char * _threadName)
{
	if (threadList.count(_threadName) == 0)return nullptr;

	return threadList[_threadName].threadHandle;
}

const unsigned Manager_Thread::GetThreadID(const char * _threadName)
{
	if (threadList.count(_threadName) == 0)return -1;

	return threadList[_threadName].threadID;
}

void Manager_Thread::DeleteThreadFromList(const char * _threadName)
{
	if (threadList.count(_threadName) == 0)return;

	threadList.erase(_threadName);
}
