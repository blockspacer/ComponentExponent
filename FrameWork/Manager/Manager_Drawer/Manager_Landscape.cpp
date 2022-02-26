//=============================================================================
//
//  ランドスケープマネージャー [Manager_Landscape.cpp]
//  Date   : 2021/11/03
//
//=============================================================================

//******************************************************************************
// インクルード
//******************************************************************************

#include "Manager_Landscape.h"
#include "../../Actor/3D/Landscape/MeshField.h"


void Manager_Landscape::RemoveComponent(MeshField * _del)
{
	if (m_ComponentList.size() > 0)
	{
		m_ComponentList.remove(_del);
	}
}

//更新関数
void Manager_Landscape::Draw()
{
	//所有メッシュフィールドを描画
	for (auto& i : m_ComponentList)
	{
		i->Draw();
	}
}

//終了関数
void Manager_Landscape::Uninit()
{
	m_ComponentList.clear();//要素削除
}



