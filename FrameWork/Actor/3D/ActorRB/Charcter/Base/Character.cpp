//=============================================================================
//
//  �L�����N�^�[���N���X [Character.cpp]
//  Date   : 2021/11/4
//
//=============================================================================

#include "../../../../../System/MainWindow.h"
#include "../../../../../Component/Component_SkeletalMesh.h"
#include "../../../../../Manager/Manager_Shader.h"
#include "../../../../../Sound/FootStep.h"
#include "StateMachine/StateMachine_Character.h"
#include "Character.h"

constexpr int   PLAYER_MASS = 100;                 //�v���C���[�̎���
constexpr float LERP_ROTATION_SPEED     = 0.2f;    //��]�p�̕�ԑ��x
constexpr float ONEFRAME_MAX_MOMENTUM   = 1.5f;    //1�t���[���̍ő�^����
constexpr float EFFECTIVE_VALUE         = 0.1f;    //�L������ɗp����l
constexpr float DIADONAL_MOVEMENT_VALUE = 0.71428f;//45�x�΂߈ړ��ɗp����l
constexpr float INERTIA                 = 0.98f;   //����

//�ǂݍ���
void  Character::Load()
{
	FootStep::Load();
}
//�폜
void  Character::Unload()
{
	FootStep::Unload();
}


void  Character::Init()//������
{
	//��ꏉ����
	ActorRB::Init();
	//Actor3D::Init();

	//3������]�����l
	m_Rotation = Vector3{ DegToRad(0.0f),DegToRad(0.0f),DegToRad(0.0f)};

	//3�����T�C�Y�����l
	m_Scale    = Vector3{ 1.0f,1.0f,1.0f };

	//�ړ����x
	m_PlayerVelocity = 0.0f;

	//�X�P���^�����b�V���R���|�[�l���g�ǉ�
	m_Component_SkeletalMesh = AddComponent<Component_SkeletalMesh>(0);

	if (m_Component_SkeletalMesh != nullptr)
	{

		//�X�P���^�����b�V���ǂݍ���
		m_Component_SkeletalMesh->LoadSkeletalMesh("ROM\\3D\\Character\\Hutao\\SK_Jean.fbx");

		//�A�j���[�V�����ǂݍ���
		m_Component_SkeletalMesh->LoadAnimation("ROM\\3D\\Character\\Hutao\\A_Idle.fbx",         "Idle");
		m_Component_SkeletalMesh->LoadAnimation("ROM\\3D\\Character\\Hutao\\A_Running.fbx",      "Walk");
		m_Component_SkeletalMesh->LoadAnimation("ROM\\3D\\Character\\Hutao\\A_Sprint.fbx",       "Run");
		m_Component_SkeletalMesh->LoadAnimation("ROM\\3D\\Character\\Hutao\\A_Jump_Start.fbx",   "Jump_Start");
		m_Component_SkeletalMesh->LoadAnimation("ROM\\3D\\Character\\Hutao\\A_Jump_Loop.fbx",    "Jump_Loop");
		m_Component_SkeletalMesh->LoadAnimation("ROM\\3D\\Character\\Hutao\\A_falling_idle.fbx", "Fall");

		//�V�F�[�_�[�ݒ�
		m_Component_SkeletalMesh->SetShaderType(Manager_Shader::SHADER_TYPE::SHADER_UNLIT);

		//�����X�e�[�g
		m_Component_SkeletalMesh->SetCurrentAnimationName("Idle");

		//�`��T�C�Y�䗦�ύX
		m_Component_SkeletalMesh->SetScaleRate(0.0005f);//���f���f�[�^��0.0005�{

		//�I�t�Z�b�g����
		m_Component_SkeletalMesh->SetPositionOffset(Vector3(0.0f, -0.525f, 0.0f));//���W�I�t�Z�b�g�@X���@Y�� Z��
		m_Component_SkeletalMesh->SetRotationOffset(Vector3{ DegToRad(0.0f),DegToRad(180.0f),DegToRad(0.0f) });//��]�I�t�Z�b�g X���@Y�� Z��
	}

	//���a0.5,����1.0�̃J�v�Z���^���W�b�h�{�f�B�ǉ�
	AddCapsuleRB(0.5f, 1.0f, m_Position, m_Rotation, PLAYER_MASS);
	

	//�X�e�[�g�}�V��������
	if (m_StateMachine = new StateMachine_Character)
	{
		m_StateMachine->Init(this);
	}


	//�����N���X����
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
		//Rigidbody�R���|�[�l���g�ǉ�
		m_Component_Rigidbody = AddComponent<Component_Rigidbody>(0);

		//OBB�R���|�[�l���g�ǉ�
		m_Component_OBB = AddComponent<Component_OBB>(0);
		m_Component_OBB->SetVisibility(true);//����
		m_Component_OBB->SetScale(Vector3{ 1,1,1 });//�X�P�[���ݒ�
	}
}

void Character::Update()//�X�V
{
	//�X�e�[�g�}�V���X�V
	if (m_StateMachine != nullptr)
	{
		m_StateMachine->Update();
	}

	//�ړ����x�Ɋ������Z
	m_PlayerVelocity *= INERTIA;

	//�p�����X�V
	ActorRB::Update();

}
void Character::Uninit()//�I��
{
	//�X�e�[�g�}�V���I��
	if (m_StateMachine != nullptr)
	{
		m_StateMachine->Uninit();
		delete m_StateMachine;
		m_StateMachine = nullptr;
	}

	if (m_FootStep != nullptr)
	{//�����폜
		m_FootStep->Uninit();
		delete m_FootStep;
		m_FootStep = nullptr;
	}

	ActorRB::Uninit();
}

//��]
void Character::MoveRotation(float _mouseX)
{
	m_Rotation.y += _mouseX;
}


//���ړ�
void Character::MoveAxis(float _forward_value, float _right_value)
{
	//�X�e�[�g�}�V������X�e�[�g�Ɏ��ړ�����
	if (m_StateMachine != nullptr)
	{
		m_StateMachine->MoveAxis(_forward_value, _right_value);
	}
}

//�X�v�����g
void Character::Sprint(bool _input)
{
	//�X�e�[�g�}�V������X�e�[�g�ɃX�v�����g����
	if (m_StateMachine != nullptr)
	{
		m_StateMachine->Sprint(_input);
	}
}

//�W�����v
void Character::Jump()
{
	//�X�e�[�g�}�V������X�e�[�g�ɃW�����v����
	if (m_StateMachine != nullptr)
	{
		m_StateMachine->Jump();
	}
}

//�����Đ�
void Character::PlayFootStep(int _physicsMaterial)
{
	if (m_FootStep != nullptr)
	{
		m_FootStep->Play(_physicsMaterial);
	}
}
