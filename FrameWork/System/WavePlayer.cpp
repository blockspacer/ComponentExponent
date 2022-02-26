//=============================================================================
//
//  Wave再生 [WaveLPlayer.cpp]
//  Date   : 2021/11/2
//
//=============================================================================

#include "WavePlayer.h"
#include <time.h>

WavePlayer::WavePlayer()
{

}

WavePlayer::~WavePlayer()
{

}

bool WavePlayer::Play(IXAudio2* pXAudio, LPCSTR strFilename, int loop)
{
	HANDLE hFile = CreateFile(
		strFilename,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	XAUDIO2_BUFFER buffer = { 0 };

	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;

	BYTE* pData = nullptr;

	WaveLoader::ReadRiff(hFile, dwChunkSize, dwChunkPosition);
	WaveLoader::ReadWfx(hFile, Wfx, dwChunkSize, dwChunkPosition);
	WaveLoader::ReadData(hFile, pData, dwChunkSize, dwChunkPosition);

	buffer.pAudioData = pData;
	buffer.AudioBytes = dwChunkSize;
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	buffer.LoopCount = loop;// Loop回数

	pXAudio->CreateSourceVoice(&m_pSourceVoice, &Wfx);

	m_pSourceVoice->SubmitSourceBuffer(&buffer);
	m_pSourceVoice->Start();

	m_Volume = 1.0f;

	// ソードボイス作成
	HRESULT hr = m_pSourceVoice->SetVolume(m_Volume * m_Volume);

	if (FAILED(hr))
	{
		m_pSourceVoice->DestroyVoice();
		m_pSourceVoice = nullptr;
		return false;
	}

	m_isDestroy = false;
	m_isFade = false;
	m_isRunning = true;

	return true;
}


void WavePlayer::Update()
{
	XAUDIO2_VOICE_STATE state;
	m_pSourceVoice->GetState(&state);

	if (m_isRunning) {
		m_isRunning = (state.BuffersQueued > 0) != 0;
	}

	// フェードの更新
	FadeUpdate();

	// 終了
	if (!m_isRunning && !m_isFade)
	{
		// ノイズ対策
		m_pSourceVoice->SetVolume(0.0f);

		m_isDestroy = true;
	}
}

void WavePlayer::UnInit()
{
	m_pSourceVoice->DestroyVoice();
	m_pSourceVoice = nullptr;
}

// フェードの更新
void WavePlayer::FadeUpdate()
{
	if (m_isFade)
	{
		float now = static_cast<int>(clock()) * 1.0f;
		m_FadeTime = (now - m_StartFadeTime) / 1000.0f;

		m_Volume = m_StartVolume * (1.0f - (m_FadeTime / m_MaxFadeTime));

		m_pSourceVoice->SetVolume(m_Volume * m_Volume);

		if (m_FadeTime >= m_MaxFadeTime)
		{
			m_Volume = m_FadeVolume;

			m_pSourceVoice->SetVolume(m_Volume * m_Volume);

			m_FadeTime = m_MaxFadeTime;
			m_isFade = false;
		}
	}
}


void WavePlayer::SetFade(float time, float volume)
{
	m_MaxFadeTime = time;
	m_FadeTime = 0.0f;
	m_StartFadeTime = static_cast<int>(clock()) * 1.0f;
	m_FadeVolume = volume;
	m_StartVolume = m_Volume;

	m_isFade = true;
}

void WavePlayer::SetVolume(float volume)
{
	m_Volume = volume;
	// volume * volume  ←簡易フェードカーブ
	m_pSourceVoice->SetVolume(volume * volume);
}