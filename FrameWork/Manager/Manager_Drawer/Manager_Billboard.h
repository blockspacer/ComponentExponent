//=============================================================================
//
//  �r���{�[�h�R���|�[�l���g�}�l�[�W���[ [Manager_Billboard.h]
//  Date   : 2021/11/03
//
//=============================================================================
#pragma once

#include <unordered_map>
#include "ManagerComponent_SingleArray_Base.h"

//******************************************************************************
// �N���X��`
//******************************************************************************
class Manager_Billboard /*: public ManagerComponent_SingleArray_Base<class Component_Billboard>*/
{
private:

	std::list<class Component_Billboard*>m_ComponentList;

public:

	void AddComponent(class Component_Billboard* _add)
	{
		m_ComponentList.push_back(_add);
	}

	void RemoveComponent(class Component_Billboard* _del);

	void Draw();//�`��
	void Uninit();//�I��
};
