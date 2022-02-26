//=============================================================================
//
//  �r���{�[�h�R���|�[�l���g�}�l�[�W���[ [Manager_Billboard.cpp]
//  Date   : 2021/11/03
//
//=============================================================================

//******************************************************************************
// �C���N���[�h
//******************************************************************************

#include "Manager_Billboard.h"
#include "../../Component/Component_Billboard.h"

void Manager_Billboard::RemoveComponent(Component_Billboard * _del)
{
	for (auto& i : m_ComponentList)
	{
		if (i == _del)
		{
			Component_Billboard* temp = i;
			m_ComponentList.remove(_del);
			delete temp;
			temp = nullptr;
			return;
		}
	}
}

//�X�V�֐�
void Manager_Billboard::Draw()
{
	//���L�r���{�[�h�R���|�[�l���g��`��
	for (Component_Billboard* component_Billboard : m_ComponentList)
	{
		component_Billboard->Draw();
	}
}

//�I���֐�
void Manager_Billboard::Uninit()
{
	m_ComponentList.clear();//�v�f�폜
}



