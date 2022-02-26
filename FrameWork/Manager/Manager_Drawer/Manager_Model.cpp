//=============================================================================
//
//  モデルマネージャー [Manager_Model.cpp]
//  Date   : 2021/11/05
//
//=============================================================================

//******************************************************************************
// インクルード
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

//更新関数
void Manager_Model::Draw()
{
	//所有モデルコンポーネントを描画
	for (Component_ModelBase* component_Model : m_ComponentList)
	{
		component_Model->Draw();
	}
}

//終了関数
void Manager_Model::Uninit()
{
	m_ComponentList.clear();//要素削除
}



