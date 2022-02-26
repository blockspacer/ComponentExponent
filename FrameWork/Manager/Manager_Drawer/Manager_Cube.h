//=============================================================================
//
//  �L���[�u�R���|�[�l���g�}�l�[�W���[ [Manager_Cube.h]
//  Date   : 2022/1/19
//
//=============================================================================
#pragma once

#include <unordered_map>
#include "ManagerComponent_SingleArray_Base.h"


//******************************************************************************
// �N���X��`
//******************************************************************************
class Manager_Cube /*: public ManagerComponent_SingleArray_Base<class Component_Cube>*/
{
private:

	std::list<class Component_Cube*>m_ComponentList;

public:

	void AddComponent(class Component_Cube* _add)
	{
		m_ComponentList.push_back(_add);
	}

	void RemoveComponent(class Component_Cube* _del)
	{
		if (_del)
		{
			m_ComponentList.remove(_del);
		}
	}

	void Draw();//�`��
	void Uninit();//�I��
};