//=============================================================================
//
//  キャラクター基底クラス [Character.cpp]
//  Date   : 2021/11/4
//
//=============================================================================

#include "../../../../../System/MainWindow.h"
#include "../../../../../Component/Component_SkeletalMesh.h"
#include "../../../../../Manager/Manager_Shader.h"
#include "../../../../../Sound/FootStep.h"
#include "StateMachine/StateMachine_Character.h"
#include "Character.h"

constexpr int   PLAYER_MASS = 100;                 //プレイヤーの質量
constexpr float LERP_ROTATION_SPEED     = 0.2f;    //回転角の補間速度
constexpr float ONEFRAME_MAX_MOMENTUM   = 1.5f;    //1フレームの最大運動量
constexpr float EFFECTIVE_VALUE         = 0.1f;    //有効判定に用いる値
constexpr float DIADONAL_MOVEMENT_VALUE = 0.71428f;//45度斜め移動に用いる値
constexpr float INERTIA                 = 0.98f;   //慣性

//読み込み
void  Character::Load()
{
	FootStep::Load();
}
//削除
void  Character::Unload()
{
	FootStep::Unload();
}


void  Character::Init()//初期化
{
	//基底初期化
	ActorRB::Init();
	//Actor3D::Init();

	//3次元回転初期値
	m_Rotation = Vector3{ DegToRad(0.0f),DegToRad(0.0f),DegToRad(0.0f)};

	//3次元サイズ初期値
	m_Scale    = Vector3{ 1.0f,1.0f,1.0f };

	//移動速度
	m_PlayerVelocity = 0.0f;

	//スケルタルメッシュコンポーネント追加
	m_Component_SkeletalMesh = AddComponent<Component_SkeletalMesh>(0);

	if (m_Component_SkeletalMesh != nullptr)
	{

		//スケルタルメッシュ読み込み
		m_Component_SkeletalMesh->LoadSkeletalMesh("ROM\\3D\\Character\\Hutao\\SK_Jean.fbx");

		//アニメーション読み込み
		m_Component_SkeletalMesh->LoadAnimation("ROM\\3D\\Character\\Hutao\\A_Idle.fbx",         "Idle");
		m_Component_SkeletalMesh->LoadAnimation("ROM\\3D\\Character\\Hutao\\A_Running.fbx",      "Walk");
		m_Component_SkeletalMesh->LoadAnimation("ROM\\3D\\Character\\Hutao\\A_Sprint.fbx",       "Run");
		m_Component_SkeletalMesh->LoadAnimation("ROM\\3D\\Character\\Hutao\\A_Jump_Start.fbx",   "Jump_Start");
		m_Component_SkeletalMesh->LoadAnimation("ROM\\3D\\Character\\Hutao\\A_Jump_Loop.fbx",    "Jump_Loop");
		m_Component_SkeletalMesh->LoadAnimation("ROM\\3D\\Character\\Hutao\\A_falling_idle.fbx", "Fall");

		//シェーダー設定
		m_Component_SkeletalMesh->SetShaderType(Manager_Shader::SHADER_TYPE::SHADER_UNLIT);

		//初期ステート
		m_Component_SkeletalMesh->SetCurrentAnimationName("Idle");

		//描画サイズ比率変更
		m_Component_SkeletalMesh->SetScaleRate(0.0005f);//モデルデータの0.0005倍

		//オフセット調整
		m_Component_SkeletalMesh->SetPositionOffset(Vector3(0.0f, -0.525f, 0.0f));//座標オフセット　X軸　Y軸 Z軸
		m_Component_SkeletalMesh->SetRotationOffset(Vector3{ DegToRad(0.0f),DegToRad(180.0f),DegToRad(0.0f) });//回転オフセット X軸　Y軸 Z軸
	}

	//半径0.5,高さ1.0のカプセル型リジッドボディ追加
	AddCapsuleRB(0.5f, 1.0f, m_Position, m_Rotation, PLAYER_MASS);
	

	//ステートマシン初期化
	if (m_StateMachine = new StateMachine_Character)
	{
		m_StateMachine->Init(this);
	}


	//足音クラス生成
	m_FootStep = new FootStep;

	if (m_FootStep != nullptr)
	{
		m_FootStep->Init();
	}

}

void Character::AddRigid()
{
	if (!m_Component_Rigidbody)
	{
		//Rigidbodyコンポーネント追加
		m_Component_Rigidbody = AddComponent<Component_Rigidbody>(0);

		//OBBコンポーネント追加
		m_Component_OBB = AddComponent<Component_OBB>(0);
		m_Component_OBB->SetVisibility(true);//可視性
		m_Component_OBB->SetScale(Vector3{ 1,1,1 });//スケール設定
	}
}

void Character::Update()//更新
{
	//ステートマシン更新
	if (m_StateMachine != nullptr)
	{
		m_StateMachine->Update();
	}

	//移動速度に慣性加算
	m_PlayerVelocity *= INERTIA;

	//継承元更新
	ActorRB::Update();

}
void Character::Uninit()//終了
{
	//ステートマシン終了
	if (m_StateMachine != nullptr)
	{
		m_StateMachine->Uninit();
		delete m_StateMachine;
		m_StateMachine = nullptr;
	}

	if (m_FootStep != nullptr)
	{//足音削除
		m_FootStep->Uninit();
		delete m_FootStep;
		m_FootStep = nullptr;
	}

	ActorRB::Uninit();
}

//回転
void Character::MoveRotation(float _mouseX)
{
	m_Rotation.y += _mouseX;
}


//軸移動
void Character::MoveAxis(float _forward_value, float _right_value)
{
	//ステートマシンからステートに軸移動命令
	if (m_StateMachine != nullptr)
	{
		m_StateMachine->MoveAxis(_forward_value, _right_value);
	}
}

//スプリント
void Character::Sprint(bool _input)
{
	//ステートマシンからステートにスプリント命令
	if (m_StateMachine != nullptr)
	{
		m_StateMachine->Sprint(_input);
	}
}

//ジャンプ
void Character::Jump()
{
	//ステートマシンからステートにジャンプ命令
	if (m_StateMachine != nullptr)
	{
		m_StateMachine->Jump();
	}
}

//足音再生
void Character::PlayFootStep(int _physicsMaterial)
{
	if (m_FootStep != nullptr)
	{
		m_FootStep->Play(_physicsMaterial);
	}
}
