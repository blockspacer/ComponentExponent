//=============================================================================
//
//  OBB�R���|�[�l���g [Component_OBB.cpp]
//  Author : Ryosuke Kotera
//  Date   : 2021/10/18
//
//=============================================================================
#pragma once

#include <math.h>
#include "../Level/Base/Level.h"
#include "../Manager/Manager_Shader.h"
#include "../Manager/Manager_Drawer/Owner/Manager_Drawer.h"
#include "../Manager/Manager_Drawer/Manager_OBB.h"
#include "../Manager/Manager_Game.h"
#include "../Renderer/Renderer_Polygon.h"
#include "../Actor/3D/Base/Actor3D.h"
#include "../Actor/3D/ActorRB/Charcter/Base/Character.h"
#include "../Actor/3D/ActorRB/Charcter//Enemy.h"
#include "../Actor/3D/Camera.h"
#include "../Actor/3D/Prop/Bullet.h"
#include "Component_OBB.h"

constexpr float HALF_SIZE = 0.5f;

void Component_OBB::Init()
{
	Component::Init();//��ꏉ����

	m_ShaderType = Manager_Shader::SHADER_UNLIT;//�f�t�H���g�̓A�����b�g�V�F�[�_�[

	m_Color = Color{ 1.0f,1.0f,1.0f,1.0f };//�F�𔒂ɐݒ�

	//�f�o�b�O�R���W�����`�悵�Ȃ�
	m_Visibility = false;

	//���C�ɔ������邩
	m_isReactRay = true;

	//�����}�e���A���ݒ�(�����̓R���N���[�g)
	m_PhysicsMaterial = PM_CONCRETE;

	if (Actor3D* actor3D = dynamic_cast<Actor3D*>(m_Owner))//Actor3D�ɃL���X�g
	{//�I�[�i�[���擾
		m_Position = actor3D->GetPosition();//���W
		m_Rotation = actor3D->GetRotation();//��]
		m_Scale    = actor3D->GetScale();   //�X�P�[��

		//OBB�}�l�[�W���[�L���b�V��
		m_Manager_OBB = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_OBB();

		if (m_Manager_OBB)
		{
			//todo:�L���X�g��߂�typeid�̃n�b�V���Ŕ�r����悤�ɂ���

			//�}�l�[�W���[�擾�ł�����o�^
			if (Enemy* enemy = dynamic_cast<Enemy*>(actor3D))
			{
				m_Manager_OBB->AddComponent(this, COLLISION_ENEMY);
			}
			else if (Bullet* bullet = dynamic_cast<Bullet*>(actor3D))
			{
				m_Manager_OBB->AddComponent(this, COLLISION_BULLET);
			}
			else if (Camera* bullet = dynamic_cast<Camera*>(actor3D))
			{
				m_Manager_OBB->AddComponent(this, COLLISION_CAMERA);
			}
			else if (Character* bullet = dynamic_cast<Character*>(actor3D))
			{
				m_Manager_OBB->AddComponent(this, COLLISION_PLAYER);
			}
			else if (actor3D->GetTag() == std::string("item"))
			{
				m_Manager_OBB->AddComponent(this, COLLISION_ITEM);
			}
			else//�u���b�N
			{
				m_Manager_OBB->AddComponent(this, COLLISION_BLOCK);
			}
		}
	}
}

void Component_OBB::Uninit()
{
	if (m_Manager_OBB)
	{
		m_Manager_OBB->RemoveComponent(this);
	}
}

void Component_OBB::Update()
{
	if (Actor3D* actor3D = dynamic_cast<Actor3D*>(m_Owner))//Actor3D�ɃL���X�g
	{//�I�[�i�[���擾

		m_Position = actor3D->GetPosition();//���W
		m_Rotation = actor3D->GetRotation();//��]

		m_Color = Color{ 1.0f,1.0f,1.0f,1.0f };//�F�𔒂ɐݒ�

		//�}�g���N�X�v�Z
		D3DXMATRIX rot, scale, world;

		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		world = scale * rot;

		//OBB�v�Z
		m_NormaDirectionX = Vector3{ world._11 ,world._12,world._13} *HALF_SIZE;
		m_NormaDirectionY = Vector3{ world._21 ,world._22,world._23} *HALF_SIZE;
		m_NormaDirectionZ = Vector3{ world._31 ,world._32,world._33} *HALF_SIZE;

	    //OBB�̐����̒����v�Z
		m_LengthX = D3DXVec3Length(&m_NormaDirectionX);
		m_LengthY = D3DXVec3Length(&m_NormaDirectionY);
		m_LengthZ = D3DXVec3Length(&m_NormaDirectionZ);

	}
}
void Component_OBB::Draw()
{
	//�`�惂�[�h�I���̎�
	if (m_Visibility)
	{
		if (Actor3D* actor3D = dynamic_cast<Actor3D*>(m_Owner))//Actor3D�ɃL���X�g
		{
			if (!actor3D->GetIsActive())return;//��A�N�e�B�u�Ȃ�`�悵�Ȃ�

			//�V�F�[�_�[�ݒ�
			Manager_Shader::Instance()->SetShader(m_ShaderType);

			//���C���[�t���[����
			Renderer_DX11::Instance()->SetRasterizerState(D3D11_FILL_WIREFRAME);

			//�|���S�������_���[����`��
		    Renderer_Polygon::Instance()->DrawBox(m_Position, m_Rotation, m_Scale, m_Color);

			//���C���[�t���[������
			Renderer_DX11::Instance()->SetRasterizerState(D3D11_FILL_SOLID);
		}
	}
}

void Component_OBB::SetOwnerPosition(Vector3 position)
{
	if (Actor3D* actor3D = dynamic_cast<Actor3D*>(m_Owner))//Actor3D�ɃL���X�g
	{
		actor3D->SetPosition(position);
	}
}

bool Component_OBB::Raycast(Vector3 _startPos, Vector3 _endPos,
	Actor * _ignoreMySelf, bool _isDispRay, std::list<ACTOR_TYPE> _ignoreList, Color _drawRayColor, int _drawRayLife)
{
	if (m_Manager_OBB != nullptr)
	{
		return m_Manager_OBB->Raycast(_startPos, _endPos, _ignoreMySelf, _isDispRay, _ignoreList, _drawRayColor, _drawRayLife);
	}

	return false;
}

int Component_OBB::RaycastPM(Vector3 _startPos, Vector3 _endPos,
	Actor * _ignoreMySelf, bool _isDispRay, std::list<ACTOR_TYPE> _ignoreList, Color _drawRayColor, int _drawRayLife)
{
	if (m_Manager_OBB != nullptr)
	{
		return m_Manager_OBB->RaycastPM(_startPos, _endPos, _ignoreMySelf, _isDispRay, _ignoreList, _drawRayColor, _drawRayLife);
	}

	return INVALID_ID;
}

std::vector<Manager_OBB::s_RaycastHit> Component_OBB::RaycastGetHit(Vector3 _startPos,
	Vector3 _endPos, Actor * _ignoreMySelf, bool _isDispRay, std::list<ACTOR_TYPE> _ignoreList, Color _drawRayColor, int _drawRayLife)
{
	if (m_Manager_OBB != nullptr)
	{
		return m_Manager_OBB->RaycastGetHit(_startPos, _endPos, _ignoreMySelf, _isDispRay, _ignoreList, _drawRayColor, _drawRayLife);
	}

	return std::vector<Manager_OBB::s_RaycastHit>();
}

bool Component_OBB::SaveComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName, "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Save<Component_OBB>(_fileName, this, sizeof(Component_OBB), tempName))
	{
		return false;
	}
	return true;
}

bool Component_OBB::LoadComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName,  "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Load<Component_OBB>(_fileName, this, tempName))
	{
		return false;
	}
	return true;
}
