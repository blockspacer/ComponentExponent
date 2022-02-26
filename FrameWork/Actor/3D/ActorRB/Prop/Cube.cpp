//=============================================================================
//
//  キューブクラス [Cube.cpp]
//  Date   : 2021/11/4
//
//=============================================================================

#include "Cube.h"
#include "../../../../Component/Component_StaticMesh.h"
#include "../../../../Component/Component_OBB.h"


void  Cube::Init()//初期化
{
	ActorRB::Init();//初期化

	//静的メッシュコンポーネント追加
	m_Component_StaticMesh = AddComponent<Component_StaticMesh>(0);
	m_Component_StaticMesh->LoadStaticMesh("ROM\\3D\\Test\\box.fbx", texName ? texName : "ROM\\2D\\Material\\T_Sand.png");
	m_Component_StaticMesh->SetScaleRate(0.5f);

	//静的ボックスリジッドボディ追加
    AddBoxRB(m_Scale, m_Position, 10000);
}

void Cube::Uninit()
{
	ActorRB::Uninit();
}


