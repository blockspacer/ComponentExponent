//=============================================================================
//
//  �X�v���C�g�R���|�[�l���g [Component_Sprite.h]
//  Date   : 2021/11/02
//
//=============================================================================
#pragma once
#include "Base/Component.h"
#include "../Manager/Manager_Shader.h"

class Component_Sprite : public Component
{

protected:

	//�e�N�X�`���L���b�V���[
	ID3D11ShaderResourceView*  m_Texture;
	//�g�p����V�F�[�_�[�̎��
	int   m_ShaderType;
	//���]�t���O
	bool  m_Reverse;
	//�f�t�H���g�e�N�X�`�����g�p���邩�̃t���O
	bool m_isUseDefaultTexture;
	//�񎟌����W
	Vector2 m_Position2D;
	//�񎟌��T�C�Y
	Vector2 m_Scale2D;
	//�F
	Color m_Color;
	//Z���W
	float m_PostionZ;

public:

	Component_Sprite(class Actor* owner, int updatePriority) : Component(owner, updatePriority) {}


	virtual void Init()override;	//������
	virtual void Uninit()override;  //�I��
	virtual void Update()override{};//�X�V
	
	//�`��
	virtual void Draw();

	//�摜�Z�b�g
	void SetTexture(const char* Key);
	void SetShaderType(Manager_Shader::SHADER_TYPE type) { m_ShaderType = type; }

	//�t�H���g�p�ɒ��Ƀe�N�X�`�����Z�b�g����֐�
	inline void SetTexture(ID3D11ShaderResourceView* _rv) { m_Texture = _rv; }

	//�񎟌����W�ݒ�
	inline void SetPosition2D(Vector2 position3D) { m_Position2D = position3D; }
	//�񎟌��T�C�Y�ݒ�
	inline void SetScale2D(Vector2 scale2D)       { m_Scale2D = scale2D; }
	//�F�ݒ�
	inline void SetColor(Color color)             { m_Color = color; }
	//Z���W�ݒ�
	inline void SetPostionZ(float z)              { m_PostionZ = z; }

	bool SaveComponentData(const char* _fileName) override;
	bool LoadComponentData(const char* _fileName) override;
};
