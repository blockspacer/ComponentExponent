//=============================================================================
//
//  ビルボードコンポーネントマネージャー [Manager_Billboard.cpp]
//  Date   : 2021/11/03
//
//=============================================================================

//******************************************************************************
// インクルード
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

//更新関数
void Manager_Billboard::Draw()
{
	//所有ビルボードコンポーネントを描画
	for (Component_Billboard* component_Billboard : m_ComponentList)
	{
		component_Billboard->Draw();
	}
}

//終了関数
void Manager_Billboard::Uninit()
{
	m_ComponentList.clear();//要素削除
}



