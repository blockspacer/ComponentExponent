//=============================================================================
//
//  キューブコンポーネントマネージャー [Manager_Cube.cpp]
//  Date   : 2022/1/19
//
//=============================================================================

//******************************************************************************
// インクルード
//******************************************************************************

#include <stdarg.h>
#include <codecvt>
#include "Manager_Cube.h"
#include "../../Component/Component_Cube.h"



//描画関数
void Manager_Cube::Draw()
{
	//所有ビルボードコンポーネントを描画
	for (Component_Cube* component_Cube : m_ComponentList)
	{
		component_Cube->Draw();
	}

}

//終了関数
void Manager_Cube::Uninit()
{
	m_ComponentList.clear();//要素削除
}
