//=============================================================================
//
//  �n�ʕ`�� [MeshField.h]
//  Date   : 2021/11/20
//
//=============================================================================
#pragma once

#include "../Base/Actor3D.h"
#include "../../../Renderer/Renderer_DX11.h"
#include "../../../System/Main.h"
//******************************************************************************
// �N���X��`
//******************************************************************************
class MeshField : public Actor3D
{
private:

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;

	ID3D11ShaderResourceView* m_Texture = NULL;


	VERTEX_3D  m_Vertex[21][21];

	int   m_ShaderType;//�g�p����V�F�[�_�[�̎��


public:

	static void Load();//���\�[�X�ǂݍ���
	static void Unload();//���\�[�X�폜

	void Init();//������
	void Uninit();//�I��
	void Update();//�X�V
	void Draw();//�`��

	float GetHeight(Vector3 _position);//�����擾
};
