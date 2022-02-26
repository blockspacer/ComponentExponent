//=============================================================================
//
//  球体クラス [Sphere.cpp]
//  Date   : 2021/1/10
//
//=============================================================================
#include "../../../../Component/Component_StaticMesh.h"
#include "../../../../Component/Component_OBB.h"
#include "../../../../Manager/Manager_Shader.h"
#include "Sphere.h"


void  Sphere::Init()//初期化
{
	ActorRB::Init();//初期化

	//静的メッシュコンポーネント追加
	m_Component_StaticMesh = AddComponent<Component_StaticMesh>(0);
	int ra = rand() % 3 + 0;
	switch (ra)
	{
	case 0:
		m_Component_StaticMesh->LoadStaticMesh("ROM\\3D\\Prop\\SM_Sphere.fbx", "ROM\\2D\\Color\\T_Red.png");
		break;
	case 1:
		m_Component_StaticMesh->LoadStaticMesh("ROM\\3D\\Prop\\SM_Sphere.fbx", "ROM\\2D\\Color\\T_Blue.png");
		break;
	case 2:
		m_Component_StaticMesh->LoadStaticMesh("ROM\\3D\\Prop\\SM_Sphere.fbx", "ROM\\2D\\Color\\T_Green.png");
		break;
	}
	m_Component_StaticMesh->SetScaleRate(0.1f);
	m_Component_StaticMesh->SetShaderType(Manager_Shader::SHADER_TYPE::SHADER_PIXEL_LIGHTING);

	//動的球体リジッドボディ追加
	AddSphereRB(m_Scale, m_Position, 0.1f);

}

void Sphere::Update()//更新
{
	ActorRB::Update();

	m_LifeTime++;

	if (m_LifeTime > 240)
	{
		DestroyActor();
	}


}
