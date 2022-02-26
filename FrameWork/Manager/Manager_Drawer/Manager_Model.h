//=============================================================================
//
//  モデルマネージャー [Manager_Model.h]
//  Date   : 2021/11/05
//
//=============================================================================
#pragma once

#include <unordered_map>
#include "ManagerComponent_SingleArray_Base.h"

//******************************************************************************
// クラス定義
//******************************************************************************
class Manager_Model/* : public ManagerComponent_SingleArray_Base<class Component_ModelBase>*/
{
private:

	std::list<class Component_ModelBase*>m_ComponentList;

public:

	void AddComponent(class Component_ModelBase* _add)
	{
		m_ComponentList.push_back(_add);
	}

	void RemoveComponent(class Component_ModelBase* _del);

	void Draw();//描画
	void Uninit();//終了
};
