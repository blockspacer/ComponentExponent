
#include "Manager_Thread.h"
#include "../Manager/Manager_Drawer/Manager_ImGui.h"

bool Manager_Thread::CreateThread(pThreadFunc _func, s_ThreadArg _arg, const char* _threadName, bool isBlocking)
{
	//もし既に同名のスレッドを実行中なら作成しない
	if (threadList.count(_threadName) > 0)
	{
		Manager_ImGui::Instance()->GetMainDebugWindow()->DebugLog(_threadName + std::string(" Thread is Already Run"));
		return false;
	}

	//構造体初期化
	s_ThreadData threadData = {};
	threadData.func = _func;
	threadData.arg = _arg;
	threadList[_threadName] = threadData;//スレッドリストに登録

	//スレッド作成
	threadList[_threadName].threadHandle = (HANDLE)_beginthreadex(NULL, 0, _func, &threadList[_threadName].arg, 0, &threadList[_threadName].threadID);

	//スレッド作成エラー
	if (threadList[_threadName].threadHandle == nullptr)
	{
		char temp[512] = {};
		sprintf_s(temp, sizeof(temp), "%s スレッド作成　失敗。引数や戻り値に問題があるかもしれません。", _threadName);
		MessageBox(NULL, temp, "ERROR", MB_OK);

		//登録取り消し
		threadList.erase(_threadName);

		return false;
	}

	//スレッド実行
	if (threadData.threadHandle != 0)
	{
		ResumeThread(threadData.threadHandle);
	}

	if (isBlocking)//ブロッキング設定なら
	{
		//終了するまで待機
		WaitForSingleObject(__threadhandle, INFINITE);

		FinishThread(_threadName);//スレッド削除
	}

	//作成成功
	return true;
}

bool Manager_Thread::GetIsThreadSignalState(const char * _threadName)
{
	if (threadList.count(_threadName) == 0)return true;//スレッド未登録

	//終了確認
	DWORD threadState = WaitForSingleObject(threadList[_threadName].threadHandle, 0);
	if (threadState == WAIT_OBJECT_0)return true;//スレッドがシグナル状態

	//スレッドがシグナル状態じゃなかったら生きているかエラー。
	return false;
}

bool Manager_Thread::FinishThread(const char * _threadName)
{
	if (threadList.count(_threadName) == 0)return false;//スレッド未登録

	//スレッドの関数で使われているかもしれない終了フラグを立てる
	threadList[_threadName].arg.isFinish = true;

	HANDLE temp = threadList[_threadName].threadHandle;

	//待たずともスレッドがシグナル状態ならすぐにスレッドを削除する
	if (WaitForSingleObject(threadList[_threadName].threadHandle, 0) == WAIT_OBJECT_0)
	{
		//スレッド削除
		if (!CloseHandle(threadList[_threadName].threadHandle))
		{
			//削除失敗
			DWORD err = GetLastError();
			std::string errtext = "CloseHandle失敗　コード:" + err;
			MessageBox(NULL, errtext.c_str(), "ERROR", MB_OK);
		}
		return true;
	}

	//まだスレッドが非シグナル状態か、エラーで終了したりしている場合は終了まで待機する
	DWORD threadState = WaitForSingleObject(threadList[_threadName].threadHandle, INFINITE);
	if (threadState == WAIT_OBJECT_0)
	{
		//スレッド削除
		if (!CloseHandle(threadList[_threadName].threadHandle))
		{
			//削除失敗
			DWORD err = GetLastError();
			std::string errtext = "CloseHandle失敗　コード:" + err;
			MessageBox(NULL, errtext.c_str(), "ERROR", MB_OK);
		}
		return true;
	}
	else if (threadState == WAIT_ABANDONED)
	{
		MessageBox(nullptr, "スレッドでロックしていたものが解放されず終了。ロック解除を書いて下さい。", "ERROR", MB_OK);
		return false;
	}
	else if (threadState == WAIT_FAILED)
	{
		DWORD errCode = GetLastError();
		char temp[1024] = {};
		sprintf_s(temp, sizeof(temp), "スレッド終了エラー　コード:%d", errCode);
		MessageBox(nullptr, temp, "ERROR", MB_OK);
		return false;
	}
	else if (threadState == WAIT_TIMEOUT)
	{
		MessageBox(nullptr, (std::string(_threadName) + std::string(" :関数の終了確認がタイムアウト。終了処理を書いて下さい。")).c_str(), "ERROR", MB_OK);
		return false;
	}

	//待ったけど終了しない
	char msg[512] = {};
	sprintf_s(msg, sizeof(msg), "スレッドが終了しません。スレッドの終了処理を書いて下さい。スレッド名:%s", _threadName);
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
			//削除
			itr = threadList.erase(itr);
		}
	}
	threadList.clear();
}

void Manager_Thread::ThreadToSignalState(void * _threadHandle)
{
	DWORD exitCode = 0;
	if (!GetExitCodeThread(static_cast<HANDLE>(_threadHandle), &exitCode))//終了コードを取得
	{
		DWORD errCode = GetLastError();
		char errTxt[256] = {};
		sprintf_s(errTxt, sizeof(errTxt), "スレッド終了コード取得エラー　コード:%d", errCode);
		MessageBox(NULL, errTxt, "ERROR", MB_OK);
		return;
	}

	if (mainThreadID != GetCurrentThreadId())
	{
		_endthreadex(exitCode);//スレッドをシグナル状態にする
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
