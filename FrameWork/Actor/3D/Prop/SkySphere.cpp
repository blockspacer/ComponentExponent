//=============================================================================
//
//  SkySphereクラス [SkySphere.cpp]
//  Date   : 2021/1/16
//
//=============================================================================

#include "SkySphere.h"
#include "../../../Component/Component_SkeletalMesh.h"
#include "../../../Component/Component_OBB.h"


constexpr float PLAYER_VELOCITY = 0.15f;


void  SkySphere::Init()//初期化
{
	Actor3D::Init();//初期化

	//静的メッシュコンポーネント追加
	m_Component_StaticMesh = AddComponent<Component_StaticMesh>(0);
	m_Component_StaticMesh->LoadStaticMesh("ROM\\3D\\Prop\\night_test_03.fbx", "ROM\\3D\\Prop\\T_Stars4.png");
	m_Component_StaticMesh->SetScaleRate(250.0f);

}

void SkySphere::Update()//更新
{
	Actor3D::Update();
}

