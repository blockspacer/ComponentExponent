//=============================================================================
//
//  ランドスケープマネージャー [Manager_Landscape.h]
//  Date   : 2021/11/03
//
//=============================================================================
#pragma once

#include <unordered_map>
#include "ManagerComponent_SingleArray_Base.h"

//******************************************************************************
// クラス定義
//******************************************************************************
class Manager_Landscape/* : public ManagerComponent_SingleArray_Base<class MeshField>*/
{
private:

	std::list<class MeshField*>m_ComponentList;

public:

	void AddComponent(class MeshField* _add)
	{
		m_ComponentList.push_back(_add);
	}

	void RemoveComponent(class MeshField* _del);

	void Draw();//描画
	void Uninit();//終了
};
