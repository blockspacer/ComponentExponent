//=============================================================================
//
//  �I�[�f�B�I�}�l�[�W���[ [Manager_Audio.cpp]
//  Date   : 2021/12/19
//
//=============================================================================
#include "Manager_Audio.h"

std::vector<WavePlayer*> Manager_Audio::m_Player;
std::unordered_map<std::string, LPCSTR> Manager_Audio::m_Resource;

void Manager_Audio::Init()
{
	// COM�̏�����
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	XAudio2Create(&m_XAudio2);
	m_XAudio2->CreateMasteringVoice(&m_MasterVoice);

	m_Player.resize(Max_Player);
}

void Manager_Audio::UnInit()
{
	// �R���e�i�̗v�f�̔j��
	for (int i = 0; i < Max_Player; i++)
	{
		if (m_Player[i] != nullptr)
		{
			m_Player[i]->UnInit();
		}

		delete m_Player[i];
		m_Player[i] = nullptr;
	}

	// �}�X�^�[�{�C�X�̔j��
	m_MasterVoice->DestroyVoice();
	m_MasterVoice = nullptr;

	// XAudio2�{�̂̔j��
	m_XAudio2->Release();
	m_XAudio2 = nullptr;

	CoUninitialize();
}

// �X�V
void Manager_Audio::Update()
{
	for (int i = 0; i < Max_Player; i++)
	{
		if (!m_Player[i])continue;
		// �\�[�X�{�C�X��NULL��������WavePlalyer���폜
		if (m_Player[i]->GetIsDestroy())
		{
			m_Player[i]->UnInit();
			delete m_Player[i];
			m_Player[i] = nullptr;
		}
	}

	for (int i = 0; i < Max_Player; i++)
	{		
		if (!m_Player[i])continue;
		m_Player[i]->Update();
	}
}

// �g���Ă��Ȃ��R���e�i��T��
int Manager_Audio::IsUsePlayer()
{
	for (int i = 0; i < Max_Player; i++)
	{
		if (!IsInElement(i))
		{
			return i;
		}
	}

	return All_Use_Player;
}

// �����̃R���e�i���g���Ă��邩�̊m�F
bool Manager_Audio::IsInElement(int _num)
{
	if (m_Player[_num])return true;

	return false;
}

// �T�E���h�Đ�
int Manager_Audio::Play(const char* _key,int loop)
{
	int num = IsUsePlayer();

	if (num != -1)
	{
		m_Player[num] = new WavePlayer;
		bool IsSucceed = m_Player[num]->Play(m_XAudio2, GetResource(_key), loop);

		// �Đ����s�������̏���
		if (IsSucceed == false)
		{
			delete m_Player[num];
			m_Player[num] = nullptr;

			num = All_Use_Player;
		}

		return num;
	}

	return All_Use_Player;
}

// �t�F�[�h�Z�b�g
void Manager_Audio::Fade(int _num, float _time, float _volume)
{
	m_Player[_num]->SetFade(_time, _volume);
}

// ���\�[�X�̍Đ�
void Manager_Audio::Load_Resource(const char* _key, LPCSTR _fileName)
{
	if (!m_Resource[_key])
	{
		m_Resource[_key] = _fileName;
	}
}

void Manager_Audio::Unload_Resource(const char* _key)
{
	m_Resource.erase(_key);
}

void Manager_Audio:: Unload_Resource_All()
{
	m_Resource.clear();
}