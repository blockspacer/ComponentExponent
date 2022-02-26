//=============================================================================
//
//  スプライトコンポーネントマネージャー [Manager_Sprite.cpp]
//  Date   : 2021/11/03
//
//=============================================================================

//******************************************************************************
// インクルード
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

//更新関数
void Manager_Sprite::Draw()
{
	//所有スプライトコンポーネントを描画
	for (Component_Sprite* component_Sprite : m_ComponentList)
	{
		component_Sprite->Draw();
	}
}

//終了関数
void Manager_Sprite::Uninit()
{
	m_ComponentList.clear();//要素削除
}



