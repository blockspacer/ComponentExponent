//=============================================================================
//
//  �����T�E���h�N���X [FootStep.h]
//  Date   : 2021/1/21
//
//=============================================================================


#include "FootStep.h"
#include "../Manager/Manager_Audio.h"

#include "../Manager/Manager_Drawer/Manager_ImGui.h"
#include "../System/Exchanger.h"

constexpr int MAX_SOUND = 6;

//���\�[�X�ǂݍ���
void FootStep::Load()
{
	if (Manager_Audio* manager_audio = Manager_Audio::Instance())
	{//�L���b�V��

		//�R���N���[�g
		manager_audio->Load_Resource("Concrete1", "ROM\\Sound\\SE\\FootStep\\Concrete\\Concrete1.WAV");
		manager_audio->Load_Resource("Concrete2", "ROM\\Sound\\SE\\FootStep\\Concrete\\Concrete2.WAV");
		manager_audio->Load_Resource("Concrete3", "ROM\\Sound\\SE\\FootStep\\Concrete\\Concrete3.WAV");
		manager_audio->Load_Resource("Concrete4", "ROM\\Sound\\SE\\FootStep\\Concrete\\Concrete4.WAV");
		manager_audio->Load_Resource("Concrete5", "ROM\\Sound\\SE\\FootStep\\Concrete\\Concrete5.WAV");
		manager_audio->Load_Resource("Concrete6", "ROM\\Sound\\SE\\FootStep\\Concrete\\Concrete6.WAV");

		//����
		manager_audio->Load_Resource("Metal1", "ROM\\Sound\\SE\\FootStep\\Metal\\Metal1.WAV");
		manager_audio->Load_Resource("Metal2", "ROM\\Sound\\SE\\FootStep\\Metal\\Metal2.WAV");
		manager_audio->Load_Resource("Metal3", "ROM\\Sound\\SE\\FootStep\\Metal\\Metal3.WAV");
		manager_audio->Load_Resource("Metal4", "ROM\\Sound\\SE\\FootStep\\Metal\\Metal4.WAV");
		manager_audio->Load_Resource("Metal5", "ROM\\Sound\\SE\\FootStep\\Metal\\Metal5.WAV");
		manager_audio->Load_Resource("Metal6", "ROM\\Sound\\SE\\FootStep\\Metal\\Metal6.WAV");

		//��
		manager_audio->Load_Resource("Sand1", "ROM\\Sound\\SE\\FootStep\\Sand\\Sand1.WAV");
		manager_audio->Load_Resource("Sand2", "ROM\\Sound\\SE\\FootStep\\Sand\\Sand2.WAV");
		manager_audio->Load_Resource("Sand3", "ROM\\Sound\\SE\\FootStep\\Sand\\Sand3.WAV");
		manager_audio->Load_Resource("Sand4", "ROM\\Sound\\SE\\FootStep\\Sand\\Sand4.WAV");
		manager_audio->Load_Resource("Sand5", "ROM\\Sound\\SE\\FootStep\\Sand\\Sand5.WAV");
		manager_audio->Load_Resource("Sand6", "ROM\\Sound\\SE\\FootStep\\Sand\\Sand6.WAV");

	}


}

//���\�[�X�폜
void FootStep::Unload()
{
	if (Manager_Audio* manager_audio = Manager_Audio::Instance())
	{
		manager_audio->Unload_Resource("Concrete1");
		manager_audio->Unload_Resource("Concrete2");
		manager_audio->Unload_Resource("Concrete3");
		manager_audio->Unload_Resource("Concrete4");
		manager_audio->Unload_Resource("Concrete5");
		manager_audio->Unload_Resource("Concrete6");

		manager_audio->Unload_Resource("Metal1");
		manager_audio->Unload_Resource("Metal2");
		manager_audio->Unload_Resource("Metal3");
		manager_audio->Unload_Resource("Metal4");
		manager_audio->Unload_Resource("Metal5");
		manager_audio->Unload_Resource("Metal6");

		manager_audio->Unload_Resource("Sand1");
		manager_audio->Unload_Resource("Sand2");
		manager_audio->Unload_Resource("Sand3");
		manager_audio->Unload_Resource("Sand4");
		manager_audio->Unload_Resource("Sand5");
		manager_audio->Unload_Resource("Sand6");
	}
}

//������
void FootStep::Init()
{
	//���U�[�u
	for (int i = 0; i < MAX_PM_TYPE; i++)
	{
		m_FootStepes[i].reserve(MAX_SOUND);
	}

	//�R���N���[�g
	m_FootStepes[PM_CONCRETE].push_back("Concrete1");
	m_FootStepes[PM_CONCRETE].push_back("Concrete2");
	m_FootStepes[PM_CONCRETE].push_back("Concrete3");
	m_FootStepes[PM_CONCRETE].push_back("Concrete4");
	m_FootStepes[PM_CONCRETE].push_back("Concrete5");
	m_FootStepes[PM_CONCRETE].push_back("Concrete6");
	//����
	m_FootStepes[PM_METAL].push_back("Metal1");
	m_FootStepes[PM_METAL].push_back("Metal2");
	m_FootStepes[PM_METAL].push_back("Metal3");
	m_FootStepes[PM_METAL].push_back("Metal4");
	m_FootStepes[PM_METAL].push_back("Metal5");
	m_FootStepes[PM_METAL].push_back("Metal6");
	//��
	m_FootStepes[PM_SAND].push_back("Sand1");
	m_FootStepes[PM_SAND].push_back("Sand2");
	m_FootStepes[PM_SAND].push_back("Sand3");
	m_FootStepes[PM_SAND].push_back("Sand4");
	m_FootStepes[PM_SAND].push_back("Sand5");
	m_FootStepes[PM_SAND].push_back("Sand6");
}

//�I��
void FootStep::Uninit()
{
	for (int i = 0; i < MAX_PM_TYPE; i++)
	{//�N���A
		m_FootStepes[i].clear();
	}
}

//�v���C
void FootStep::Play(int _physicsMaterial)
{
	if (Manager_Audio* manager_audio = Manager_Audio::Instance())
	{//�L���b�V��

		if (_physicsMaterial != INVALID_ID && _physicsMaterial < MAX_PM_TYPE)
		{
			int random = rand() % MAX_SOUND;
	
			manager_audio->Play(m_FootStepes[_physicsMaterial][random], 0);
		}

	}
}

