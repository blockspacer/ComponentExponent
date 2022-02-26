//=============================================================================
//
//  �L�����N�^�[���N���X [Character.h]
//  Date   : 2021/11/4
//
//=============================================================================
#pragma once


#include "../../Base/ActorRB.h"
#include "../../../../../PlayerController/Base/PlayerController.h"

constexpr float MAX_WALK_VELOCITY   = 60.0f;  //�����ړ����x
constexpr float MAX_SPRINT_VELOCITY = 90.0f;  //����ړ����x

class Character : public ActorRB
{
protected:

	//�X�P���^�����b�V���R���|�[�l���g
	class Component_SkeletalMesh*  m_Component_SkeletalMesh;         

	//�X�e�[�g�}�V��
	class StateMachine_Character*  m_StateMachine;

	//�ړ����x
	float m_PlayerVelocity;

	//�����N���X
	class FootStep* m_FootStep;
	
public:

	//���\�[�X�ǂݍ��݁E�폜
	static void Load();
	static void Unload();

	//�������E�X�V�E�I��
	virtual void Init()  override;
	virtual void Update()override;
	virtual void Uninit()override;

	//���ړ�
	void MoveAxis(float _forward_value, float _right_value);

	//��]
	void MoveRotation(float _mouseX);

	//�X�v�����g
	void Sprint(bool _input);

	//�W�����v
	void Jump();

	//�����Đ�
	void PlayFootStep(int _physicsMaterial);

	//�X�e�[�g�}�V���擾
	inline StateMachine_Character* GetStateMachine() const{ return m_StateMachine; }

	//�ړ����x�擾
	inline float GetPlayerVelocity() const { return m_PlayerVelocity; }
	//�ړ����x�ݒ�
	inline void SetPlayerVelocity(float _value){  m_PlayerVelocity = _value; }

	void AddRigid();

};