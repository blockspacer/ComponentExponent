//=============================================================================
//
//  階段クラス [Stair.h]
//  Date   : 2021/12/4
//
//=============================================================================
#include "../../../../Component/Component_StaticMesh.h"
#include "../../../../Component/Component_OBB.h"
#include "Stair.h"


void  Stair::Init()//初期化
{
	ActorRB::Init();//初期化

	//静的メッシュコンポーネント追加
	m_Component_StaticMesh = AddComponent<Component_StaticMesh>(0);
	m_Component_StaticMesh->LoadStaticMesh("ROM\\3D\\Prop\\SM_Stair_new.fbx", "ROM\\2D\\Test\\T_Sand.png");


	//静的ボックスリジッドボディ追加
	AddBoxRB(m_Scale, m_Position, 0);

}

void Stair::Update()//更新
{
	ActorRB::Update();
}
