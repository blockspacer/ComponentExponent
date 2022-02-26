//=============================================================================
//
//  �L���[�u�R���|�[�l���g [Component_Cube.cpp]
//  Author : Ryosuke Kotera
//  Date   : 2021/1/19
//
//=============================================================================
#pragma once

#include <math.h>
#include "../Level/Base/Level.h"
#include "../Manager/Manager_Shader.h"
#include "../Manager/Manager_Texture.h"
#include "../Manager/Manager_Drawer/Owner/Manager_Drawer.h"
#include "../Manager/Manager_Drawer/Manager_OBB.h"
#include "../Manager/Manager_Game.h"
#include "../Renderer/Renderer_Polygon.h"
#include "../Actor/3D/Base/Actor3D.h"
#include "Component_Cube.h"


//�e�N�X�`���[�ݒ�
void Component_Cube::SetTexture(const char* _Key)
{
	if (ID3D11ShaderResourceView* texture = Manager_Texture::Instance()->GetTexture(_Key))
	{//�w��̃L�[�̃e�N�X�`���[�����邩�m�F
		m_Texture = texture;//�ݒ�
	}
}

bool Component_Cube::SaveComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName,  "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Save<Component_Cube>(_fileName, this, sizeof(Component_Cube), tempName))
	{
		return false;
	}
	return true;
}

bool Component_Cube::LoadComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName,  "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Load<Component_Cube>(_fileName, this, tempName))
	{
		return false;
	}
	return true;
}

void Component_Cube::Init()
{
	Component::Init();//��ꏉ����

	m_ShaderType = Manager_Shader::SHADER_UNLIT;//�f�t�H���g�̓A�����b�g�V�F�[�_�[

	m_Color = Color{ 1.0f,1.0f,1.0f,1.0f };//�F�𔒂ɐݒ�

	//�L���[�u�}�l�[�W���[�L���b�V��
	if (m_Manager_Cube = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Cube())
	{
		m_Manager_Cube->AddComponent(this);
	}
}

void Component_Cube::Uninit()
{
	if (m_Manager_Cube)
	{
		m_Manager_Cube->RemoveComponent(this);
		//delete this;
	}
}


void Component_Cube::Draw()
{
	if (Actor3D* actor3D = dynamic_cast<Actor3D*>(m_Owner))//Actor3D�ɃL���X�g
	{
		if (!actor3D->GetIsActive() || !m_Active)return;//��A�N�e�B�u�Ȃ�`�悵�Ȃ�

		//�V�F�[�_�[�ݒ�
		Manager_Shader::Instance()->SetShader(m_ShaderType);

		// �e�N�X�`���ݒ�
		Renderer_DX11::Instance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		//�|���S�������_���[����`��
		Renderer_Polygon::Instance()->DrawBox(actor3D->GetPosition(), actor3D->GetRotation(), actor3D->GetScale(), m_Color);

	}
}
