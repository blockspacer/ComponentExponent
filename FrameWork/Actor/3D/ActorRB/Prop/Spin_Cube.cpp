//=============================================================================
//
//  回転キューブクラス [Spin_Cube.h]
//  Date   : 2021/12/4
//
//=============================================================================

#include "Spin_Cube.h"
#include "../../../../Component/Component_SkeletalMesh.h"

#include "../../../../Manager/Manager_Timeline.h"
#include "../../../../Manager/Manager_Drawer/Manager_ImGui.h"
#include "../../../../System/Exchanger.h"
#include "../../../../System/Complement/Complement_Lerp.h"

void  Spin_Cube::Init()//初期化
{
	//ActorRB::Init();

	Cube::Init();//初期化

	rot = 0.0f;
	Manager_Timeline::Instance()->AddComplement<Complement_Lerp>(0.0f, 5.0f, 10.0f, &rot, true, true, "Spin_Cube");
}

void Spin_Cube::Update()//更新
{
	m_Rotation.x += 0.02f;

	Cube::Update();

	//ActorRB::Update();

	//m_Position.x = rot;
	//m_Position.x = Manager_Timeline::Instance()->GetData("test");
	//Manager_ImGui::Instance()->GetMainDebugWindow()->
	//	DebugLog("cubeX = " + Exchanger::FLOAT_TO_STRING(m_Position.x), "none", { 1,1,1,1 }, 1);
}

void Spin_Cube::Uninit()
{
	Cube::Uninit();
}
