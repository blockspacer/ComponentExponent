//=============================================================================
//
//  Wave再生 [WaveLPlayer.h]
//  Date   : 2021/11/2
//
//=============================================================================
#pragma once

#include "WaveLoder.h"
//#include <XAudio2.h>


class WavePlayer
{
public:
	WavePlayer();
	~WavePlayer();

	// XAUDIO2_LOOP_INFINITEで無限ループ
	bool Play(IXAudio2* pXAudio, LPCSTR strFilename, int loop);
	void Update();
	void UnInit();

	const IXAudio2SourceVoice* GetSourceVoice() { return m_pSourceVoice; }
	const bool GetIsDestroy() { return m_isDestroy; }
	void Destroy() { m_isDestroy = true; }
	void SetFade(float time, float volume);
	void SetVolume(float volume);

private:
	IXAudio2SourceVoice* m_pSourceVoice = nullptr;
	WAVEFORMATEX Wfx = { 0 };
	float m_Volume = 0.0f;
	float m_Pitch = 0.0f;

	BOOL m_isRunning = TRUE;
	BOOL m_isFade = FALSE;
	BOOL m_isDestroy = FALSE;

	int m_ManageNam = 0;

	float m_MaxFadeTime = 0.0f;     // フェードする時間
	float m_FadeTime = 0.0f;        // フェードした時間
	float m_StartFadeTime = 0.0f;   // フェードがスタートした時間(clock()で取っている)
	float m_FadeVolume = 0.0f;      // フェード後のボリューム
	float m_StartVolume = 0.0f;     // フェード前のボリューム


protected:

	void FadeUpdate();
};
