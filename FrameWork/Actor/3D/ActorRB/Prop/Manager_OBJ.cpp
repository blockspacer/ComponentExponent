
#include ""
#include "Manager_OBJ.h"

void Manager_OBJ::Draw()
{
	//所有スプライトコンポーネントを描画
	for (Component_OBJ* component_Obj : m_Component_OBJ)
	{
		component_Obj->Draw();
	}
}

void Manager_OBJ::Uninit()
{
	m_Component_OBJ.clear();//要素削除
}
