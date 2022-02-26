//=============================================================================
//
//  �L���[�u�R���|�[�l���g [Component_Cube.h]
//  Author : Ryosuke Kotera
//  Date   : 2021/1/19
//
//=============================================================================
#pragma once

#include "Base/Component.h"
#include "../Manager/Manager_Drawer/Manager_Cube.h"

class Component_Cube : public Component
{
public:

	Component_Cube(class Actor* _owner, int _updatePriority) : Component(_owner, _updatePriority) {}

	void Init()   override;//������
	void Uninit() override;//�I��
	void Update() override {};//�X�V

	//�`��
	void Draw();

	//�e�N�X�`���[�Z�b�g
	void SetTexture(const char* _Key);

	//�F�ݒ�
	inline void SetColor(Color _color) { m_Color = _color; }

	bool SaveComponentData(const char* _fileName) override;
	bool LoadComponentData(const char* _fileName) override;

private:

	//�e�N�X�`���[
	ID3D11ShaderResourceView*  m_Texture;

	//�L���[�u�}�l�[�W���[�L���b�V���p
	class Manager_Cube* m_Manager_Cube;

	//�F
	Color   m_Color;

	//�g�p����V�F�[�_�[�̎��
	int  m_ShaderType;
	
};


