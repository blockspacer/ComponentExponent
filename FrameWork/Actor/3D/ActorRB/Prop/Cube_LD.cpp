//=============================================================================
//
// レベルデザイン用キューブクラス [Cube_LD.h]
//  Date   : 2021/1/19
//
//=============================================================================

#include "Cube_LD.h"
#include "../../../../Component/Component_StaticMesh.h"
#include "../../../../Component/Component_Cube.h"
#include "../../../../Manager/Manager_Texture.h"

#include "../../../../System/Exchanger.h"
#include "../../../../Manager/Manager_Drawer/Manager_ImGui.h"

void  Cube_LD::Load()//リソース読み込み
{

	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//テクスチャーマネージャーキャッシュ
		manager_Texture->LoadTexture("LD","ROM/2D/Material/T_BasicGrid_M.png");
	}

}
void  Cube_LD::Unload()//リソース削除
{

	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//テクスチャーマネージャーキャッシュ
		manager_Texture->UnloadTexture("LD");
	}

}

void  Cube_LD::Init()//初期化
{
	//初期化
	ActorRB::Init();

	//キューブコンポーネント追加
	m_Component_Cube = AddComponent<Component_Cube>(0);

	if (m_Component_Cube != nullptr)
	{
		m_Component_Cube->SetTexture("LD");
	}

	//静的ボックスリジッドボディ追加
    AddBoxRB(m_Scale, m_Position, 0);
}

void Cube_LD::Update()//更新
{

	Manager_ImGui::Instance()->GetMainDebugWindow()->
		DebugLogDontDestroy(
			Exchanger::VEC3_TO_STRING(m_Scale),
			"LD", Vector4(1, 1, 1, 1));

	ActorRB::Update();

}

void Cube_LD::Uninit()
{
	ActorRB::Uninit();
}

void Cube_LD::OffOBB()
{
	//if (m_Component_OBB)
	//{
	//	m_Component_OBB->Uninit();
	//	delete m_Component_OBB;
	//	m_Component_OBB = nullptr;
	//}
	//m_Component_OBB->SetScale({ 0,0,0 });
	m_Component_Rigidbody->SetScale({ 0.001f,0.001f,0.001f });
}

void Cube_LD::AddOBB()
{
	//m_Component_OBB->SetScale({ 2.0f,2.0f,2.0f });
	m_Component_Rigidbody->SetScale({ 2,0.5f,2 });
}
