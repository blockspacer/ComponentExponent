//=============================================================================
//
//  �X�v���C�g�R���|�[�l���g�}�l�[�W���[ [Manager_Sprite.h]
//  Date   : 2021/11/03
//
//=============================================================================
#pragma once

#include <list>
#include "ManagerComponent_SingleArray_Base.h"

//******************************************************************************
// �N���X��`
//******************************************************************************
class Manager_Sprite/* : public ManagerComponent_SingleArray_Base<class Component_Sprite>*/
{
private:

	std::list<class Component_Sprite*>m_ComponentList;

public:


	void AddComponent(class Component_Sprite* _add)
	{
		m_ComponentList.push_back(_add);
	}

	void RemoveComponent(class Component_Sprite* _del);

	void Draw();  //�`��
	void Uninit();//�I��
};
