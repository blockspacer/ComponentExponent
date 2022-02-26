//=============================================================================
//
//  �r���{�[�h�R���|�[�l���g [Component_Billboard.cpp]
//  Date   : 2021/11/05
//
//=============================================================================

#include "Component_Billboard.h"
#include "../Actor/3D/Camera.h"
#include "../Manager/Manager_Shader.h"
#include "../Manager/Manager_Game.h"
#include "../Manager/Manager_Texture.h"
#include "../Manager/Manager_Drawer/Owner/Manager_Drawer.h"
#include "../Manager/Manager_Drawer/Manager_Billboard.h"
#include "../Renderer/Renderer_DX11.h"
#include "../Level/Base/Level.h"



//�e�N�X�`���[�ݒ�
void Component_Billboard::SetTexture(const char* _Key)
{
	if (ID3D11ShaderResourceView* texture = Manager_Texture::Instance()->GetTexture(_Key))
	{//�w��̃L�[�̃e�N�X�`���[�����邩�m�F
		m_Texture = texture;//�ݒ�
	}
}


//������
void Component_Billboard::Init()
{
	Component::Init();//��ꏉ����

	m_ShaderType = Manager_Shader::SHADER_UNLIT;//�f�t�H���g�̓A�����b�g�V�F�[�_�[

	m_AnimationInformation.AnimCount    = 0;       //���݂̃t���[����
	m_AnimationInformation.MaxAnimCount = 0;       //���t���[���ōX�V���邩

	m_AnimationInformation.OneFrameSize    = { 0.0f,0.0f };//1�R�}�ӂ�̃T�C�Y
	m_AnimationInformation.CutStartPostion = { 0.0f,0.0f };//�؂���J�n���W

	m_AnimationInformation.Loop     = true;//���[�v�����邩�t���O

	m_Color = { 1.0f,1.0f,1.0f,1.0f };//�F�����l

	if (Manager_Billboard* manager_Billboard = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Billboard())
	{//�}�l�[�W���[�擾�ł�����o�^
		manager_Billboard->AddComponent(this);
	}


}
//�I��
void Component_Billboard::Uninit()
{
	if (m_Texture)
	{
		m_Texture = nullptr;
	}

	if (Manager_Billboard* manager_Billboard = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Billboard())
	{//�}�l�[�W���[�擾�ł�����o�^
		manager_Billboard->RemoveComponent(this);
	}
}

//�X�V
void Component_Billboard::Update()
{
	if (m_Active)//�L����
	{
		m_AnimationInformation.AnimCount++; //���݂̃t���[�������Z

		if (m_AnimationInformation.AnimCount == m_AnimationInformation.MaxAnimCount)
		{//�X�V�t���[�����B���ɃA�j���[�V�����X�V

			m_AnimationInformation.AnimCount = 0;//���݂̃t���[�������Z�b�g

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
					{
						m_Active = false;
					}
				}
			}
		}
	}
}


//�`��
void Component_Billboard::Draw()
{
	if (m_Active && m_Texture)//�L����
	{
		if (Actor3D* actor3D = dynamic_cast<Actor3D*>(m_Owner))//Actor3D�ɃL���X�g
		{
			if (!actor3D->GetIsActive())return;//��A�N�e�B�u�Ȃ�`�悵�Ȃ�

			//�V�F�[�_�[�ݒ�
			Manager_Shader::Instance()->SetShader(m_ShaderType);

			// �e�N�X�`���ݒ�
			Renderer_DX11::Instance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

			//�v���[���}�l�[�W���[�ŕ`��
			Renderer_Polygon::Instance()->DrawBillboard(actor3D->GetPosition(), actor3D->GetScale(),m_Color, m_AnimationInformation.OneFrameSize, m_AnimationInformation.CutStartPostion);
		}
	}
}
