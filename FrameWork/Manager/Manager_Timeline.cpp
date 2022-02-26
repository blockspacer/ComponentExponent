
#include "../System/Time.h"
#include "Manager_Timeline.h"
#include "Manager_Drawer/Manager_ImGui.h"
#include "..\System\Complement\Complement_Base.h"

const char* TIMELINE_THREAD_NAME = "timeline";//スレッド名

void Manager_Timeline::Init()
{
	isChanged = false;
	isFinished = false;

	cacheTime = Time::Instance();

	Manager_Thread::s_ThreadArg arg = {};
	arg.arg = Manager_Timeline::Instance();
	arg.isFinish = false;

	//スレッドに関数を登録
	if (!Manager_Thread::Instance()->CreateThread(Manager_Timeline::UpdateForThread, arg, TIMELINE_THREAD_NAME))
	{
		MessageBox(nullptr, "タイムラインスレッド作成失敗", "ERROR", MB_OK);
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

	//全ての補完クラスを削除して終了
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

		//経過時間更新
		itr->second->TimeUpdate();

		//補完
		itr->second->Update();

		//終了したものはリストから削除する。
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
		MessageBox(nullptr, "タイムラインスレッド引数取得エラー", "ERROR", MB_OK);
		return 0;
	}
	Manager_Timeline* mgT = Manager_Timeline::Instance();

	//終了フラグが立つまで実行
	while (!arg->isFinish)
	{
		if (mgT->GetIsFinished())break;

		mgT->Update();
	}

	return 0;
}