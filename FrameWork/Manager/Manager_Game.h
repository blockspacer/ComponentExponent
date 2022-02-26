//=============================================================================
//
//  �Q�[���}�l�[�W���[ [Manager_Game.h]
//  Author : Ryosuke Kotera
//  Date   : 2021/11/02
//
//=============================================================================
#pragma once

#include <vector>
#include "../System/Singleton.h"
#include "../Level/Base/Level.h"
#include "../PlayerController/Base/PlayerController.h"



class Manager_Game : public Singleton<Manager_Game>
{
private: 
	friend class Singleton<Manager_Game>;//�V���O���g��

	class Level*             m_Level;    //���݂̃��x��
	class Level*             m_NextLevel;//���̃��x��

	class PlayerController*  m_PlayerController;//�v���C���[�R���g���[���[

public:

	void Init();  //������
	void Update();//�X�V
	void Draw();  //�`��
	void Uninit();//�I��

	inline class Level*            GetLevel()            const { return m_Level;}            //���݂̃��x���擾
	inline class PlayerController* GetPlayerController() const { return m_PlayerController; }//�v���C���[�R���g���[���[�擾


	//���x���ύX�֐�(�t�F�[�h����)
	template <typename T>
	void LoadLevelByClass(float _time)
	{
		m_NextLevel = new T();//���x���쐬

		//���݂̃��x��������ꍇ�t�F�[�h�J�n
		if (m_Level != nullptr)
		{
			m_Level->SetFade(_time);
		}
		else if (m_NextLevel != nullptr)
		{//���݂̃��x���Ȃ��ꍇ

			//���x���ǂݍ���	
			m_Level = m_NextLevel;//�ǂݍ���
			m_Level->Init();//������

		}
	}

	template <class T>
	void LoadLevelByClass_New()
	{
		if (m_Level)//���݂̃��x��������ꍇ�͍폜
		{
			m_Level->Uninit();
			delete m_Level;
			m_Level = nullptr;
		}
		m_Level = new T();
		m_Level->Init();

		m_NextLevel = nullptr;
	}

	//���̃��x���ɕύX
	void OpenLevel()
	{
		//���݂̃��x��������ꍇ�I��
		if (m_Level != nullptr)
		{
			m_Level->Uninit();//�I��
			delete m_Level;//�폜
			m_Level = nullptr;//nullptr���
		}

		//�V�������x���ǂݍ���	
		if (m_NextLevel != nullptr)
		{
			m_Level = m_NextLevel;
			m_Level->Init();//������
		}
	}

	//�v���C���[�R���g���[���[�ύX�֐�
	template <typename T>
	void ChangePlayerController()
	{
		if (m_PlayerController != nullptr)
		{//���݂̃v���C���[�R���g���[���[�I��
			delete m_PlayerController;//�폜
			m_PlayerController = nullptr;//nullptr���
		}
		m_PlayerController = new T();//�쐬
		m_PlayerController->Init();//������
	}
};