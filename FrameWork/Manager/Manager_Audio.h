//=============================================================================
//
//  オーディオマネージャー [Manager_Audio.h]
//  Date   : 2021/12/19
//
//=============================================================================
#pragma once
#include <Windows.h>
#include <vector>
#include <unordered_map>
#include "../System/WavePlayer.h"
#include "../System/Singleton.h"

constexpr int Max_Player = 100;
constexpr int All_Use_Player = -1; // Playerが全て使われている

class Manager_Audio : public Singleton<Manager_Audio>
{
public:

	void Init();
	void UnInit();
	void Update();

	int IsUsePlayer();

	// XAUDIO2_LOOP_INFINITEで無限ループ
	int Play(const char* _key, int loop);
	void SetVolume(int _num, float volume) { m_Player[_num]->SetVolume(volume); }
	bool IsInElement(int _num);
	void End(int _num) { m_Player[_num]->Destroy(); }
	void Fade(int _num, float _time, float _volume);
	void Load_Resource(const char* _key, LPCSTR _fileName);
	void Unload_Resource(const char* _key);
	void Unload_Resource_All();
	LPCSTR GetResource(const char* _key) { return m_Resource[_key]; }

private:

	IXAudio2* m_XAudio2;
	IXAudio2MasteringVoice* m_MasterVoice;
	static std::vector<WavePlayer*> m_Player;
	static std::unordered_map<std::string, LPCSTR> m_Resource;
};