#pragma once

#include "../System/Main.h"
#include <unordered_map>
#include <process.h>
#include <mutex>
#include "../System/Singleton.h"


//使用方法
//1:unsigned int _stdcall 関数名(void*)の関数を定義。
//2:CreateThreadでスレッドと関数を関連付けて実行。
//3:FinishThreadでスレッドを終了できる。
//3:スレッドに登録した関数が、関数外の条件に関係せずにreturnする場合は特に追加の関数を呼ぶ必要はない。
//4:呼び出す関数の引数は一つにまとめて渡す。（一つの変数ならそのポインタ、複数なら構造体やクラスのポインタで渡す）
//5:FinishThreadでは、引数のs_ThreadArgのisFinishをtrueにして終了を待機するので、
//isFinishで終了する様に書いていない場合は、FinishThreadを呼ぶ前に関数が終了する状態にする必要がある。
class Manager_Thread : public Singleton<Manager_Thread>
{
private:

	DWORD mainThreadID;//メインスレッドID
	std::mutex myMutex;//ミューテックス

public:

	//関数ポインタの名前が長いからわかり易い名前を付ける。
	typedef _beginthreadex_proc_type pThreadFunc;

	//スレッドに渡す
	struct s_ThreadArg
	{
		void* arg = nullptr;
		bool isFinish = false;
	};

	//スレッド作成関数　失敗でfalseをリターン。
	//第２引数TRUEでブロッキング
	bool CreateThread(pThreadFunc _func, s_ThreadArg _arg, const char* _threadName, bool isBlocking = false);

	//スレッドが終了しているか確認
	bool GetIsThreadSignalState(const char* _threadName);

	//指定したスレッドの終了と削除。
	//※※自スレッド内で自分をこの関数で削除しようとするとエラー。※※
	//メインスレッド内で実行すること。
	bool FinishThread(const char* _threadName);

	//メインスレッドの番号を保存
	void Init();

	//全てのスレッドを終了するまで待機
	void Uninit();

	//資源を排他制御にする。他のスレッドと共有の資源を使う場合は必須。
	//※※※資源を使う前の行に書く。※※※
	inline void LockResource() { std::lock_guard<std::mutex>lock(myMutex); }

	inline void LockStart() { myMutex.lock(); }//ロック開始
	inline void LockEnd() { myMutex.unlock(); }//ロック解放※ロックしたら必ず解放すること。

	//スレッドをシグナル状態にする。
	//※※※必ずスレッドの関数内に書く。メインスレッドで実行するとプログラム終了。※※※
	void ThreadToSignalState(void* _threadHandle);

	const HANDLE GetThreadHandle(const char* _threadName);
	const unsigned GetThreadID(const char* _threadName);

	void DeleteThreadFromList(const char* _threadName);

private:

	friend Singleton<Manager_Thread>;

	//スレッドの情報
	struct s_ThreadData
	{
		pThreadFunc func;
		unsigned int threadID;
		HANDLE threadHandle;
		s_ThreadArg arg;//スレッドに渡す引数の実体
	};
	std::unordered_map<const char*, s_ThreadData>threadList;
};