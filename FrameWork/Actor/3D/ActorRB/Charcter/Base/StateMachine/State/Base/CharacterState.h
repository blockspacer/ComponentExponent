//=============================================================================
//
//  �L�����N�^�[�X�e�[�g��� [CharacterState.h]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "../../../Character.h"
#include "../../../../../../../../Manager/Manager_Drawer/Manager_Font2D.h"

constexpr float STOP_VELOCITY = 0.25f;//��~����
constexpr float ONE_FRAME_UP_VELOCITY = 5.0f;//1�t���[���ɉ��Z���������x
constexpr float GROUND_RAY_LENGTH = 0.45f;//�ڒn����ɗp���郌�C�̒���

class CharacterState
{
public:

	//�������E�X�V�E�I��
	virtual void Init(Character* _owner);
	virtual void Update() = 0;
	virtual void Uninit() = 0;

	//���ړ�
	virtual void MoveAxis(float _forward_value, float _right_value);

	//�W�����v
	virtual void Jump();

	//�X�v�����g
	virtual void Sprint(bool _input) {};

protected:

	//���̃X�e�[�g���������Ă���L�����N�^�[
	class Character*           m_Owner;
	//�I�[�i�[�̃��W�b�h�{�f�B�L���b�V��
	class Component_Rigidbody* m_Component_Rigidbody;

};