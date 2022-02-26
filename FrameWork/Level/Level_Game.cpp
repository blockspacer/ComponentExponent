

#include "../System/MainWindow.h"
#include "Level_Game.h"
#include "../Actor/3D/ActorRB/Prop/Coin.h"
#include "../Actor/3D/ActorRB/Prop/Cube_LD.h"
#include "../Component/Component_OBB.h"
#include "../Component/Component_Cube.h"
#include "../Actor/3D/Camera.h"
#include "../System/Input.h"
#include "../Manager/Manager_Texture.h"
#include "../Actor/3D/ActorRB/Prop/Move_Cube.h"
#include "../Actor/2D/Atomosphere/Atomosphere.h"
#include "../Actor/3D/ActorRB/Prop/Cube_NongravityMass.h"
#include "../Actor/3D/ActorRB/Charcter/Base/Character.h"
#include "../Actor/3D/ActorRB/Prop/Treasure.h"
#include "../Manager/Manager_Drawer/Manager_ImGui.h"
#include "../Manager/Manager_Audio.h"
#include "../Manager/Manager_Game.h"
#include "../Level/Level_TP.h"
#include "../Manager/Manager_Drawer/Manager_Font2D.h"
#include "../System/Exchanger.h"

void Level_Game::Init()
{
	Level::Init();

	Cube_LD::Load();
	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//�e�N�X�`���[�}�l�[�W���[�L���b�V��
		manager_Texture->LoadTexture("Sand", "ROM/2D/Material/T_Sand.png");
		manager_Texture->LoadTexture("Metal", "ROM/2D/Material/T_Hex.png");
		manager_Texture->LoadTexture("Gold", "ROM/2D/Material/T_Gold.png");
		manager_Texture->LoadTexture("Wood", "ROM/2D/Material/T_Wood.png");
	}

	if (Manager_Drawer* manager_Drawer = GetManager_Drawer())
	{//�`��}�l�[�W���[���L���b�V��
		manager_Drawer->AddManager_Cube();         //�L���[�u�}�l�[�W���[�ǉ�
		manager_Drawer->AddManager_Model();        //FBX�}�l�[�W���[�ǉ�
		manager_Drawer->AddManager_Billboard();    //�r���{�[�h�}�l�[�W���[�ǉ�
		manager_Drawer->AddManager_Sprite();       //�X�v���C�g�}�l�[�W���[�ǉ�
		manager_Drawer->AddManager_OBB();          //OBB�}�l�[�W���[�ǉ�
		manager_Drawer->AddManager_Rigidbody();    //Rigidbody�}�l�[�W���[�ǉ�
		manager_Drawer->AddManager_Font3D();       //Font3D�}�l�[�W���[�ǉ�

		//�J�����ǉ�
		m_MainCamera = AddActor<Camera>();
		m_MainCamera->SetName("mainCamera");
		m_MainCamera->SetPosition({ 0,10,10 });
		m_MainCamera->SetRotation({ -30,0,0 });

		//�������
		Move_Cube* moveCube = AddActor<Move_Cube>();
		moveCube->SetRigidbodyPostion({ 0,0,-5 });
		moveCube->SetRigidbodyScale({ 10,1,15 });

		//��
		Cube_LD* floor = AddActor<Cube_LD>();
		floor->SetRigidbodyPostion(Vector3{ 0.0f,-1.0f,0.0f });
		floor->SetRigidbodyScale(Vector3{ 10.0f,1.0f,30.0f });
		floor->GetComponentByClass<Component_OBB>()->SetPhysicsMaterial(PHYSICS_MATERIAL::PM_CONCRETE);
		floor->GetComponentByClass<Component_Cube>()->SetTexture("Metal");

		//���̕�
		Cube_LD* table = AddActor<Cube_LD>();
		table->SetRigidbodyPostion(Vector3{ 0,3,-14 });
		table->SetRigidbodyScale(Vector3{ 10,5,1 });
		table->GetComponentByClass<Component_OBB>()->SetPhysicsMaterial(PHYSICS_MATERIAL::PM_CONCRETE);
		table->GetComponentByClass<Component_Cube>()->SetTexture("Metal");

		//�E�̕�
		Cube_LD* wallR = AddActor<Cube_LD>();
		wallR->SetRigidbodyPostion(Vector3{ 6.0f,0.0f,0.0f });
		wallR->SetRigidbodyScale(Vector3{ 1.0f,5.0f,30.0f });
		wallR->GetComponentByClass<Component_OBB>()->SetPhysicsMaterial(PHYSICS_MATERIAL::PM_CONCRETE);
		wallR->GetComponentByClass<Component_Cube>()->SetTexture("Metal");

		//���̕�
		Cube_LD* wallL = AddActor<Cube_LD>();
		wallL->SetRigidbodyPostion(Vector3{ -6.0f,0.0f,0.0f });
		wallL->SetRigidbodyScale(Vector3{ 1.0f,5.0f,30.0f });
		wallL->GetComponentByClass<Component_OBB>()->SetPhysicsMaterial(PHYSICS_MATERIAL::PM_CONCRETE);
		wallL->GetComponentByClass<Component_Cube>()->SetTexture("Metal");
	}

	Manager_Audio::Instance()->Init();
	Manager_Audio::Instance()->Load_Resource("coin", "ROM\\Sound\\SE\\Coin.wav");
	Manager_Audio::Instance()->Load_Resource("coins", "ROM\\Sound\\SE\\Coins.wav");
	Manager_Audio::Instance()->Play("coin", 0);
}

void Level_Game::Update()
{
	Level::Update();

#pragma region MoveCamera

	const float moveSpeed = 0.3f;
	if (Input::Instance()->GetKeyPress('W'))
	{
		m_MainCamera->Move(moveSpeed, 0.0f);
	}
	if (Input::Instance()->GetKeyPress('S'))
	{
		m_MainCamera->Move(-moveSpeed, 0.0f);
	}
	if (Input::Instance()->GetKeyPress('A'))
	{
		m_MainCamera->Move(0.0f, -moveSpeed);
	}
	if (Input::Instance()->GetKeyPress('D'))
	{
		m_MainCamera->Move(0.0f, moveSpeed);
	}
	if (Input::Instance()->GetKeyPress('E'))
	{
		m_MainCamera->SetPosition(m_MainCamera->GetPosition() + Vector3(0, moveSpeed, 0));
	}
	if (Input::Instance()->GetKeyPress('Q'))
	{
		m_MainCamera->SetPosition(m_MainCamera->GetPosition() + Vector3(0, -moveSpeed, 0));
	}
	const float rotSpeed = 0.05f;
	if (Input::Instance()->GetKeyPress(VK_UP))
	{
		m_MainCamera->SetRotation(m_MainCamera->GetRotation() + Vector3(-rotSpeed, 0, 0));
	}
	if (Input::Instance()->GetKeyPress(VK_DOWN))
	{
		m_MainCamera->SetRotation(m_MainCamera->GetRotation() + Vector3(rotSpeed, 0, 0));
	}
	if (Input::Instance()->GetKeyPress(VK_LEFT))
	{
		m_MainCamera->SetRotation(m_MainCamera->GetRotation() + Vector3(0, -rotSpeed, 0));
	}
	if (Input::Instance()->GetKeyPress(VK_RIGHT))
	{
		m_MainCamera->SetRotation(m_MainCamera->GetRotation() + Vector3(0, rotSpeed, 0));
	}
#pragma endregion

	if (Input::Instance()->GetKeyTrigger(VK_SPACE))
	{
		Coin* coin = AddActor<Coin>();
		coin->SetRigidbodyPostion({ static_cast<float>((rand() % 5) * (rand() % 2 ? 1 : -1)),5,static_cast<float>((rand() % 5) * (rand() % 2 ? 1 : -1)) });
		coin->SetRigidbodyScale({ 1,0.3f,-1 });
		coin->SetScale({ 1,0.15f,1 });

		coinList.push_back(coin);

		Manager_Audio::Instance()->Play("coin", 0);
	}
	if (Input::Instance()->GetKeyPress('F'))
	{
		Manager_Game::Instance()->LoadLevelByClass_New<Level_Test>();
	}

	coinList.remove_if([](Coin* _coin) {return _coin->GetPosition().y <= -15.0f; });
}

void Level_Game::Uninit()
{
	Level::Uninit();
}