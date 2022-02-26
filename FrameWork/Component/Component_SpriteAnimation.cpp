//=============================================================================
//
//  �A�j���[�V�����X�v���C�g�R���|�[�l���g [Component_SpriteAnimation.cpp]
//  Date   : 2021/11/04
//
//=============================================================================

#include "Component_SpriteAnimation.h"
#include "../Actor/2D/Base/Actor2D.h"
#include "../Manager/Manager_Shader.h"
#include "../Manager/Manager_Game.h"
#include "../Manager/Manager_Texture.h"
#include "../Manager/Manager_Drawer/Manager_Sprite.h"
#include "../Renderer/Renderer_DX11.h"
#include "../Level/Base/Level.h"


//������
void Component_SpriteAnimation::Init()
{
	//������
	Component_Sprite::Init();

	m_AnimationInformation.AnimCount       = 0;//���݂̃t���[����
	m_AnimationInformation.MaxAnimCount    = 0;//���t���[���ōX�V���邩

	m_AnimationInformation.OneFrameSize    = {0.0f,0.0f};//1�R�}�ӂ�̃T�C�Y
	m_AnimationInformation.CutStartPostion = {0.0f,0.0f};//�؂���J�n���W

	//���[�v�����邩�t���O
	m_AnimationInformation.Loop = true;
}

//�X�V
void Component_SpriteAnimation::Update()
{
	if (m_Active)//�L����
	{
		//���ݕ��̃t���[�������Z
		m_AnimationInformation.AnimCount++; 

		if (m_AnimationInformation.AnimCount == m_AnimationInformation.MaxAnimCount)
		{//�X�V�t���[�����B���ɃA�j���[�V�����X�V

			//���݂̗ݐσt���[�������Z�b�g
			m_AnimationInformation.AnimCount = 0;

			if (m_AnimationInformation.CutStartPostion.x + m_AnimationInformation.OneFrameSize.x < MAX_CUT_START_POSTION)
			{//X�����ɃA�j���[�V�����X�V�\���H
				m_AnimationInformation.CutStartPostion.x += m_AnimationInformation.OneFrameSize.x;//X�؂�����W��1�R�}�����炷
			}
			else
			{//X�����ɃA�j���[�V�����X�V�s��

				m_AnimationInformation.CutStartPostion.x = 0.0f;//X�؂�����W������

				if (m_AnimationInformation.CutStartPostion.y + m_AnimationInformation.OneFrameSize.y < MAX_CUT_START_POSTION)
				{//Y�����ɃA�j���[�V�����X�V�\���H
					m_AnimationInformation.CutStartPostion.y += m_AnimationInformation.OneFrameSize.y;//Y�؂�����W��1�R�}�����炷
				}
				else
				{//X��Y���ɍX�V�s��

					m_AnimationInformation.CutStartPostion.y = 0.0f;//Y�؂�����W������

					if (!m_AnimationInformation.Loop)//���[�v���Ȃ��A�j���[�V�����̏ꍇ
					{//�A�N�e�B�u��false��
						m_Active = false;
					}
				}
			}
		}
	}
}

void Component_SpriteAnimation::Uninit()
{
	Component_Sprite::Uninit();
}

//�`��
void Component_SpriteAnimation::Draw()
{
	if (m_Active && m_Texture)//�L����
	{
		//�V�F�[�_�[�ݒ�
		Manager_Shader::Instance()->SetShader(m_ShaderType);

		// �e�N�X�`���ݒ�
		Renderer_DX11::Instance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		//�v���[�������_���[����`��
		Renderer_Polygon::Instance()->Draw2DAnimation(m_Position2D, m_Scale2D, m_Color,
			m_AnimationInformation.OneFrameSize, m_AnimationInformation.CutStartPostion, m_Reverse);
	}
}

bool Component_SpriteAnimation::SaveComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName,  "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Save<Component_SpriteAnimation>(_fileName, this, sizeof(Component_SpriteAnimation), tempName))
	{
		return false;
	}
	return true;
}

bool Component_SpriteAnimation::LoadComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName, "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Load<Component_SpriteAnimation>(_fileName, this, tempName))
	{
		return false;
	}
	return true;
}
