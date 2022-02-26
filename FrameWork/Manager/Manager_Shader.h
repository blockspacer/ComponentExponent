//=============================================================================
//
//  �V�F�[�_�[�}�l�[�W���[ [Manager_Shader.h]
//  Date   : 2021/11/05
//
//=============================================================================
#pragma once

#include <unordered_map>
#include"../System/Singleton.h"
#include "../System/Main.h"


//******************************************************************************
// �N���X��`
//******************************************************************************
class Manager_Shader : public Singleton<Manager_Shader>
{
private:

	friend class Singleton<Manager_Shader>;//�V���O���g��

	struct s_Shader
	{
		ID3D11InputLayout*    VertexLayout;//IL
		ID3D11VertexShader*   VertexShader;//VS
		ID3D11PixelShader*    PixelShader; //PS
	};//�V�F�[�_�[���

	std::unordered_map < int, s_Shader > m_Shaders;//�V�F�[�_�[

public:

	void Init(void);//����������
	void Uninit(void);//�I������

	void SetShader(int _key);//�V�F�[�_�[�ݒ�

	enum SHADER_TYPE//�V�F�[�_�[�^�C�v
	{
		SHADER_UNLIT,
		SHADER_VERTEX_LIGHTING,
		SHADER_PIXEL_LIGHTING,
		SHADER_ATOMOSPHERE,
		SHADER_TOON,
		SHADER_NORMALMAP,
		SHADER_ENVMAP
	};
};
