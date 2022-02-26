//=============================================================================
//
//  �r���{�[�h�R���|�[�l���g [Component_Billboard.cpp]
//  Date   : 2021/11/05
//
//=============================================================================
#pragma once
#include "Base/Component.h"
#include "../Renderer/Renderer_Polygon.h"

class Component_Billboard : public Component
{
private:

	//�e�N�X�`���[
	ID3D11ShaderResourceView*  m_Texture;

	//�X�v���C�g�A�j���[�V�����\����
	s_SpriteAnimation m_AnimationInformation;

	//�g�p����V�F�[�_�[�̎��
	int   m_ShaderType; 

	//�F 
	Color m_Color;


public:

	Component_Billboard(class Actor* _owner, int _updatePriority) : Component(_owner, _updatePriority) {}


	virtual void Init()override;    //������
	virtual void Uninit()override;  //�I��
	virtual void Update() override;	//�X�V

	virtual void Draw();//�`��

	void SetTexture(const char* _Key);//�摜�Z�b�g

	inline void SetColor(Color _color) { m_Color = _color;}//�F�ݒ�

	inline void CalculationOneFrameSize(int _W_frame, int _H_frame)
	{//1�R�}�ӂ�̃T�C�Y����o��
		m_AnimationInformation.OneFrameSize.x = static_cast<float>(1.0f / _W_frame);
		m_AnimationInformation.OneFrameSize.y = static_cast<float>(1.0f / _H_frame);
	}

	inline void SetLoop(bool _loop) { m_AnimationInformation.Loop = _loop; }//���[�v�����邩�ݒ�
	inline void SetMaxAnimCount(int _max_count) { m_AnimationInformation.MaxAnimCount = _max_count; }//���t���[���ōX�V���邩�ݒ�
};
