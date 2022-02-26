//=============================================================================
//
//  ���f���}�l�[�W���[ [Manager_Model.cpp]
//  Date   : 2021/11/05
//
//=============================================================================

//******************************************************************************
// �C���N���[�h
//******************************************************************************

#include "Manager_Model.h"
#include "../../Component/Component_SkeletalMesh.h"

void Manager_Model::RemoveComponent(Component_ModelBase * _del)
{
	for (auto& i : m_ComponentList)
	{
		if (i == _del)
		{
			Component_ModelBase* temp = i;
			m_ComponentList.remove(_del);
			return;
		}
	}
}

//�X�V�֐�
void Manager_Model::Draw()
{
	//���L���f���R���|�[�l���g��`��
	for (Component_ModelBase* component_Model : m_ComponentList)
	{
		component_Model->Draw();
	}
}

//�I���֐�
void Manager_Model::Uninit()
{
	m_ComponentList.clear();//�v�f�폜
}



