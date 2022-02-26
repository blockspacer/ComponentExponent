//=============================================================================
//
//  �e�N�X�`���[�}�l�[�W���[ [Manager_Texturer.h]
//  Date   : 2021/11/02
//
//=============================================================================
#pragma once

#include <unordered_map>
#include"../System/Singleton.h"
#include "../System/Main.h"

//******************************************************************************
// �N���X��`
//******************************************************************************
class Manager_Texture : public Singleton<Manager_Texture>
{
private:

	friend class Singleton<Manager_Texture>;//�V���O���g��

	std::unordered_map < std::string, ID3D11ShaderResourceView* > m_Textures;//�e�N�X�`���[���

public:
	void Init(void);  //����������
	void Uninit(void);//�I������

	ID3D11ShaderResourceView* LoadTexture(const char* _key,const char* _fileName);//�ǂݍ���
	void UnloadTexture(const char* Key);//�폜
	void AddTexture(ID3D11ShaderResourceView* _texture,const char* _key)//���Ƀ��[�h���ꂽ�e�N�X�`�����Ǘ�
	{
		for (auto& i : m_Textures)
		{
			if (i.second == _texture)return;
		}

		//�����e�N�X�`�������������̂Œǉ�
		m_Textures[_key] = _texture;
	}

	inline ID3D11ShaderResourceView*  GetTexture(const char* _key) 
	{
		if (m_Textures[_key])
		{
			return m_Textures[_key];
		}

		//�e�N�X�`�������݂��Ȃ��ꍇ�̓G���[�e�N�X�`����Ԃ�
		return m_Textures["Error"];
		
	}//�摜�擾
};
