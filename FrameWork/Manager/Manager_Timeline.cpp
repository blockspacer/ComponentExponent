
#include "../System/Time.h"
#include "Manager_Timeline.h"
#include "Manager_Drawer/Manager_ImGui.h"
#include "..\System\Complement\Complement_Base.h"

const char* TIMELINE_THREAD_NAME = "timeline";//�X���b�h��

void Manager_Timeline::Init()
{
	isChanged = false;
	isFinished = false;

	cacheTime = Time::Instance();

	Manager_Thread::s_ThreadArg arg = {};
	arg.arg = Manager_Timeline::Instance();
	arg.isFinish = false;

	//�X���b�h�Ɋ֐���o�^
	if (!Manager_Thread::Instance()->CreateThread(Manager_Timeline::UpdateForThread, arg, TIMELINE_THREAD_NAME))
	{
		MessageBox(nullptr, "�^�C�����C���X���b�h�쐬���s", "ERROR", MB_OK);
	}
}

void Manager_Timeline::Uninit()
{
	Manager_Thread::Instance()->LockResource();

	isFinished = true;

	if (Manager_Thread::Instance()->FinishThread(TIMELINE_THREAD_NAME))
	{
		Manager_Thread::Instance()->DeleteThreadFromList(TIMELINE_THREAD_NAME);
	}

	//�S�Ă̕⊮�N���X���폜���ďI��
	for (auto itr = DataList.begin();itr != DataList.end();itr++)
	{
		delete itr->second;
		itr->second = nullptr;
	}
	DataList.clear();
}

void Manager_Timeline::Update()
{
	if (DataList.empty() || GetIsFinished())return;

	for (auto itr = DataList.begin(); itr != DataList.end();itr++)
	{
		if (!IsWaitForSync())return;

		//�o�ߎ��ԍX�V
		itr->second->TimeUpdate();

		//�⊮
		itr->second->Update();

		//�I���������̂̓��X�g����폜����B
		if (itr->second->GetIsFinished())
		{
			delete itr->second;
			itr->second = nullptr;
			itr = DataList.erase(itr);
		}
	}

	SetIsChanged();
}

unsigned _stdcall Manager_Timeline::UpdateForThread(void* _arg)
{
	Manager_Thread::s_ThreadArg* arg = static_cast<Manager_Thread::s_ThreadArg*>(_arg);
	if (!arg)
	{
		MessageBox(nullptr, "�^�C�����C���X���b�h�����擾�G���[", "ERROR", MB_OK);
		return 0;
	}
	Manager_Timeline* mgT = Manager_Timeline::Instance();

	//�I���t���O�����܂Ŏ��s
	while (!arg->isFinish)
	{
		if (mgT->GetIsFinished())break;

		mgT->Update();
	}

	return 0;
}