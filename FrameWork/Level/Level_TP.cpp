//=============================================================================
//
//  �e�X�g [Level_Test.cpp]
//  Date   : 2021/11/03
//
//=============================================================================

#include "Level_TP.h"
#include "Level_Game.h"
#include "../PlayerController/PC_Test.h"
#include "../Manager/Manager_Game.h"
#include "../Manager/Manager_Drawer/Owner/Manager_Drawer.h"
#include "../Manager/Manager_Drawer/Manager_Font2D.h"
#include "../Manager/Manager_Font3D.h"
#include "../Manager/Manager_Drawer/Manager_ImGui.h"
#include "../Manager/Manager_Grid.h"
#include "../Manager/Manager_Texture.h"
#include "../Actor/3D/ActorRB/Prop/Cube_Obj_Test.h"
#include "../Actor/3D/Camera.h"
#include "../Actor/3D/Prop/SkySphere.h"
#include "../Actor/3D/Billboard/Test/TestBillboard.h"
#include "../Actor/3D/Landscape/MeshField.h"
#include "../Actor/3D/ActorRB/Charcter/Base/Character.h"
#include "../Actor/3D/ActorRB/Charcter/Enemy.h"
#include "../Actor/3D/ActorRB/Prop/Cube.h"
#include "../Actor/3D/ActorRB/Prop/Cube_LD.h"
#include "../Actor/3D/ActorRB/Prop/Spin_Cube.h"
#include "../Actor/2D/Static/Test/TestActor2D_Static.h"
#include "../Actor/2D/Atomosphere/Atomosphere.h"
#include "../Actor/2D/Animation/Test/TestActor2D_Animation.h"
#include "../Actor/HUD/HUD_Test.h"
#include "../Component/Component_StaticMesh.h"
#include "../Component/Component_OBB.h"
#include "../Component/Component_Cube.h"
#include "../System/Exchanger.h"
#include "../Actor/3D/Prop/ItemSphere.h"
#include "../Manager/Manager_Audio.h"
#include "../System/DataSaver.h"
#include "Level_Title.h"

#include "../Component/Component_SkeletalMesh.h"

void Level_Test::Init()
{
	Level::Init();

	//�v���C���[�R���g���[���[��PC_Test�ɐݒ�
	Manager_Game::Instance()->ChangePlayerController<PC_Test>();

	//���\�[�X�ǂݍ���
	Character::Load();
	Atomosphere::Load();
	Cube_LD::Load();
	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//�e�N�X�`���[�}�l�[�W���[�L���b�V��
		manager_Texture->LoadTexture("Sand",  "ROM/2D/Material/T_Sand.png");
		manager_Texture->LoadTexture("Metal", "ROM/2D/Material/T_Metal.png");
		manager_Texture->LoadTexture("Wood", "ROM/2D/Material/T_Wood.png");
	}

	if (Manager_Audio* manager_ar = Manager_Audio::Instance())
	{
		manager_ar->Load_Resource("coin", "ROM\\Sound\\SE\\Coin.wav");
	}

	//�`��}�l�[�W���[�ǉ�
	if (Manager_Drawer* manager_Drawer = GetManager_Drawer())
	{//�`��}�l�[�W���[���L���b�V��
		manager_Drawer->AddManager_Cube();         //�L���[�u�}�l�[�W���[�ǉ�
		manager_Drawer->AddManager_Model();        //FBX�}�l�[�W���[�ǉ�
		manager_Drawer->AddManager_Billboard();    //�r���{�[�h�}�l�[�W���[�ǉ�
		manager_Drawer->AddManager_Sprite();       //�X�v���C�g�}�l�[�W���[�ǉ�
		manager_Drawer->AddManager_OBB();          //OBB�}�l�[�W���[�ǉ�
		manager_Drawer->AddManager_Rigidbody();    //Rigidbody�}�l�[�W���[�ǉ�
		manager_Drawer->AddManager_Font3D();       //Font3D�}�l�[�W���[�ǉ�

		//��C�ǉ�
		AddActor<Atomosphere>();

		//�A�N�^�[�ǉ�
		Spin_Cube* spin = AddActor<Spin_Cube>();
		spin->SetRigidbodyPostion(Vector3{ 0.0f,1.5f,-5.0f });
		spin->SetRigidbodyScale(Vector3{ 1,1,1 });
		spin->SetName("spin");

		//��
		Cube_LD* test = AddActor<Cube_LD>();
		test->SetRigidbodyPostion(Vector3{ 2.5f,1.0f,2.5f });
		test->SetRigidbodyScale(Vector3{ 2.0f,0.5f,2.0f });
		test->SetName("Cube_1");
		test->GetComponentByClass<Component_OBB>()->SetPhysicsMaterial(PM_METAL);
		test->GetComponentByClass<Component_OBB>()->SetIsReactRay(false);
		test->GetComponentByClass<Component_Cube>()->SetTexture("Metal");

		ItemSphere* tempItemA = AddActor<ItemSphere>();
		tempItemA->GetComponentByClass<Component_OBB>()->SetOwnerPosition(Vector3(2.5f, 2.0f, 2.5f));

		//��
		Cube_LD* test2 = AddActor<Cube_LD>();
		test2->SetRigidbodyPostion(Vector3{ 5.0f,1.0f,2.5f });
		test2->SetRigidbodyScale(Vector3{ 2.0f,0.5f,2.0f });
		test2->SetName("Cube_2");
		test2->GetComponentByClass<Component_OBB>()->SetPhysicsMaterial(PHYSICS_MATERIAL::PM_CONCRETE);
		test2->GetComponentByClass<Component_Cube>()->SetTexture("Wood");

		//�R�C��
		ItemSphere* tempItemB = AddActor<ItemSphere>();
		tempItemB->GetComponentByClass<Component_OBB>()->SetOwnerPosition(Vector3(5.0f, 2.0f, 2.5f));

		//��
		Cube_LD* test3 = AddActor<Cube_LD>();
		test3->SetRigidbodyPostion(Vector3{ 7.5f,2.0f,2.5f });
		test3->SetRigidbodyScale(Vector3{ 2.0f,0.5f,2.0f });
		test3->SetName("Cube_3");
		test3->GetComponentByClass<Component_OBB>()->SetPhysicsMaterial(PM_SAND);
		test3->GetComponentByClass<Component_Cube>()->SetTexture("Sand");

		//�R�C��
		ItemSphere* tempItemC = AddActor<ItemSphere>();
		tempItemC->GetComponentByClass<Component_OBB>()->SetOwnerPosition(Vector3(7.5f, 3.0f, 2.5f));

		//��
		Cube_LD* floor = AddActor<Cube_LD>();
		floor->SetRigidbodyPostion(Vector3{ 0.0f,0.0f,0.0f });
		floor->SetRigidbodyScale(Vector3{ 20.0f,1.0f,20.0f });
		floor->GetComponentByClass<Component_OBB>()->SetPhysicsMaterial(PM_SAND);
		floor->GetComponentByClass<Component_Cube>()->SetTexture("Wood");
		floor->SetName("floor");

		//����
		Cube_LD* cube2 = AddActor<Cube_LD>();
		cube2->SetRotation(Vector3{ DegToRad(0.0f),DegToRad(90.0f),DegToRad(0.0f) });
		cube2->SetRigidbodyPostion(Vector3{ 10.5f,5.0f,0.0f });
		cube2->SetRigidbodyScale(Vector3{ 20.0f,10.0f,1.0f });
		cube2->SetName("Wall_Back");
		cube2->GetComponentByClass<Component_OBB>()->SetPhysicsMaterial(PM_SAND);
		cube2->GetComponentByClass<Component_Cube>()->SetTexture("Wood");

		//��O��
		Cube_LD* cube3 = AddActor<Cube_LD>();
		cube3->SetRotation(Vector3{ DegToRad(0.0f),DegToRad(90.0f),DegToRad(0.0f) });
		cube3->SetRigidbodyPostion(Vector3{ -10.5f,5.0f,0.0f });
		cube3->SetRigidbodyScale(Vector3{ 20.0f,10.0f,1.0f });
		cube3->SetName("Wall_Foward");
		cube3->GetComponentByClass<Component_OBB>()->SetPhysicsMaterial(PM_SAND);
		cube3->GetComponentByClass<Component_Cube>()->SetTexture("Wood");

		//����
		Cube_LD* cube4 = AddActor<Cube_LD>();
		cube4->SetRigidbodyPostion(Vector3{ 0.0f,5.0f,10.5f });
		cube4->SetRigidbodyScale(Vector3{ 20.0f,10.0f,1.0f });
		cube4->SetName("Wall_Left");
		cube4->GetComponentByClass<Component_OBB>()->SetPhysicsMaterial(PM_SAND);
		cube4->GetComponentByClass<Component_Cube>()->SetTexture("Wood");

		//�E��
		Cube_LD* cube5 = AddActor<Cube_LD>();
		cube5->SetRigidbodyPostion(Vector3{ 0.0f,5.0f,-10.5f });
		cube5->SetRigidbodyScale(Vector3{ 20.0f,10.0f,1.0f });
		cube5->SetName("Wall_Right");
		cube5->GetComponentByClass<Component_OBB>()->SetPhysicsMaterial(PM_SAND);
		cube5->GetComponentByClass<Component_Cube>()->SetTexture("Wood");

		//�L�����N�^�[
		Character* tempChar = AddActor<Character>();
		tempChar->SetPosition({ 0,5,0 });
		tempChar->SetName("mainPlayer");

		//�J�����ǉ�
		Camera* tempCam = AddActor<Camera>();
		tempCam->SetOwner(tempChar);
		tempCam->SetName("mainCamera");

		//HUD�ݒ�
		manager_Drawer->SetHUD<HUD_Test>();

		coinCount = 0;
		coinCountText = "" + Exchanger::INT_TO_STRING(coinCount);
		coinTextPos = { 0,-500 };

		//�R�C����
		Manager_Font2D* mf2d = Manager_Font2D::Instance();
		mf2d->SetAilgningMode(Manager_Font::e_Aligning::center);
		mf2d->SetFontSize(64.0f);
		mf2d->SetFontWeight(FW_BLACK);
		mf2d->SetFont(L"none", L"none");
		mf2d->AddPrintData(coinTextPos, coinCountText, "count");

		clearText = "";
		clearTextPos = { 0,16 };

		//�N���A����
		mf2d->SetFont(L"���͂�ѕM-free-", L"���͂�ѕM-free-");
		mf2d->SetFontSize(64.0f);
		mf2d->SetFontWeight(FW_BLACK);
		mf2d->SetFontColor(Color(0, 0, 0, 1));
		mf2d->AddPrintData(clearTextPos, clearText, "clear");
	}

	Manager_Grid2D::Instance()->SetColor(Color(0.0f, 0.5f, 0.5f, 1.0f));
	Manager_Grid2D::Instance()->SetCutNum({ 10,10 });

	isClear = false;
}

void Level_Test::Update()
{
	Level::Update();

	if (isClear)
	{
		clearCount++;
		if (clearCount > 60 * 5)
		{
			Manager_Font2D::Instance()->DeleteAllText();
			//Manager_Game::Instance()->LoadLevelByClass_New<Level_Title>();
			Manager_Game::Instance()->LoadLevelByClass_New<Level_Game>();
		}
	}

	if (Input::Instance()->GetKeyTrigger('F'))
	{
		Manager_Game::Instance()->LoadLevelByClass_New<Level_Game>();
	}

	//�A�N�^�[�폜
	if (Input::Instance()->GetKeyTrigger('G'))
	{
		Manager_Game::Instance()->GetLevel()->DeleteActorByString<Cube_LD>("Cube_1", "test");
	}
}

void Level_Test::AddScore()
{
	coinCount++;
	if (coinCount > 2)
	{
		clearText = "�N���A!!";
		isClear = true;
	}
	coinCountText = "" + Exchanger::INT_TO_STRING(coinCount);
};

void Level_Test::Uninit()
{
	Level::Uninit();

	//���\�[�X���
	Atomosphere::Unload();
	Cube_LD::Unload();
	Character::Unload();
};