//=============================================================================
//
//  �����h�X�P�[�v�}�l�[�W���[ [Manager_Landscape.cpp]
//  Date   : 2021/11/03
//
//=============================================================================

//******************************************************************************
// �C���N���[�h
//******************************************************************************

#include "Manager_Landscape.h"
#include "../../Actor/3D/Landscape/MeshField.h"


void Manager_Landscape::RemoveComponent(MeshField * _del)
{
	if (m_ComponentList.size() > 0)
	{
		m_ComponentList.remove(_del);
	}
}

//�X�V�֐�
void Manager_Landscape::Draw()
{
	//���L���b�V���t�B�[���h��`��
	for (auto& i : m_ComponentList)
	{
		i->Draw();
	}
}

//�I���֐�
void Manager_Landscape::Uninit()
{
	m_ComponentList.clear();//�v�f�폜
}



