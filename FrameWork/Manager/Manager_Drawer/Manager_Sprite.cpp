//=============================================================================
//
//  �X�v���C�g�R���|�[�l���g�}�l�[�W���[ [Manager_Sprite.cpp]
//  Date   : 2021/11/03
//
//=============================================================================

//******************************************************************************
// �C���N���[�h
//******************************************************************************

#include "Manager_Sprite.h"
#include "../../Component/Component_Sprite.h"

void Manager_Sprite::RemoveComponent(Component_Sprite * _del)
{
	for (auto& i : m_ComponentList)
	{
		if (i == _del)
		{
			Component_Sprite* temp = i;
			m_ComponentList.remove(_del);
			return;
		}
	}
}

//�X�V�֐�
void Manager_Sprite::Draw()
{
	//���L�X�v���C�g�R���|�[�l���g��`��
	for (Component_Sprite* component_Sprite : m_ComponentList)
	{
		component_Sprite->Draw();
	}
}

//�I���֐�
void Manager_Sprite::Uninit()
{
	m_ComponentList.clear();//�v�f�폜
}



