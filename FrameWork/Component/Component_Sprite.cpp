//=============================================================================
//
//  �X�v���C�g�R���|�[�l���g [Component_Sprite.cpp]
//  Date   : 2021/11/02
//
//=============================================================================

#include "Component_Sprite.h"
#include "../Actor/2D/Base/Actor2D.h"
#include "../Manager/Manager_Shader.h"
#include "../Manager/Manager_Game.h"
#include "../Manager/Manager_Texture.h"
#include "../Manager/Manager_Drawer/Owner/Manager_Drawer.h"
#include "../Manager/Manager_Drawer/Manager_Sprite.h"
#include "../Renderer/Renderer_DX11.h"
#include "../Renderer/Renderer_Polygon.h"
#include "../Level/Base/Level.h"


//�e�N�X�`���[�ݒ�
void Component_Sprite::SetTexture(const char* Key)
{
	if (ID3D11ShaderResourceView* texture = Manager_Texture::Instance()->GetTexture(Key))
	{//�w��̃L�[�̃e�N�X�`���[�����邩�m�F
		m_Texture = texture;//�ݒ�
	}
}

bool Component_Sprite::SaveComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName,  "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Save<Component_Sprite>(_fileName, this, sizeof(Component_Sprite), tempName))
	{
		return false;
	}
	return true;
}

bool Component_Sprite::LoadComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName, "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Load<Component_Sprite>(_fileName, this, tempName))
	{
		return false;
	}
	return true;
}


//������
void Component_Sprite::Init()
{
	//��ꏉ����
	Component::Init();

	//�f�t�H���g�̓A�����b�g�V�F�[�_�[
	m_ShaderType = Manager_Shader::SHADER_UNLIT;

	//���]�t���O
	m_Reverse = false;

	//�f�t�H���g�e�N�X�`���g�p�t���O
	m_isUseDefaultTexture = false;

	//�F�����l
	m_Color = { 1.0f,1.0f,1.0f,1.0f };

	//Z���W�����l
	m_PostionZ = 0.0f;

	if (Actor2D* actor2D = dynamic_cast<Actor2D*>(m_Owner))//Actor2D�ɃL���X�g
	{
		//�I�[�i�[�̍��W�L���b�V��
		m_Position2D = actor2D->GetPosition2D();
		//�I�[�i�[�̃X�P�[���L���b�V��
		m_Scale2D = actor2D->GetScale2D();
	}

	if (Manager_Sprite* manager_Sprite = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Sprite())
	{//�X�v���C�g�}�l�[�W���[���擾�ł�����o�^����
		manager_Sprite->AddComponent(this);
	}

}
//�I��
void Component_Sprite::Uninit()
{
	if (m_Texture)
	{//�L���b�V�������e�N�X�`����nullptr��
		m_Texture = nullptr;
	}

	if (Manager_Sprite* manager_Sprite = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Sprite())
	{//�X�v���C�g�}�l�[�W���[���擾�ł�����o�^����
		//manager_Sprite->AddComponent<Component_Sprite>(this);
		manager_Sprite->RemoveComponent(this);
	}
}

//�`��
void Component_Sprite::Draw()
{
	if (m_Active && m_Texture)//�L����
	{
		//�V�F�[�_�[�ݒ�
		Manager_Shader::Instance()->SetShader(m_ShaderType);

		// �e�N�X�`���ݒ�
		Renderer_DX11::Instance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		//�v���[�������_���[����`��
		Renderer_Polygon::Instance()->Draw2D(m_Position2D, m_Scale2D, m_Color, m_Reverse, m_PostionZ);
	}
}
