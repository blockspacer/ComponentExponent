//=============================================================================
//
//  �L�����N�^�[�X�e�[�g��� [CharacterState.cpp]
//  Date   : 2021/11/28
//
//=============================================================================
#pragma once

#include "CharacterState.h"
#include "../../../../../../Camera.h"
#include "../../../../../../../../Manager/Manager_Game.h"
#include "../../../../../../../../Component/Component_Rigidbody.h"

constexpr float LERP_ROTATION_SPEED = 0.2f;        //��]�p�̕�ԑ��x
constexpr float ONEFRAME_MAX_MOMENTUM = 1.5f;      //1�t���[���̍ő�^����
constexpr float EFFECTIVE_VALUE = 0.1f;            //�L������ɗp����l
constexpr float DIADONAL_MOVEMENT_VALUE = 0.71428f;//45�x�΂߈ړ��ɗp����l

void CharacterState::Init(Character* _owner)
{
	if (_owner!= nullptr)
	{//�I�[�i�[�擾
		m_Owner = _owner;
		m_Component_Rigidbody = m_Owner->GetComponentByClass<Component_Rigidbody>();
	}
}

//���ړ�
void CharacterState::MoveAxis(float _forward_value, float _right_value)
{
	if (m_Component_Rigidbody != nullptr && m_Owner != nullptr)
	{
		if (fabsf(_forward_value) + fabsf(_right_value) >= ONEFRAME_MAX_MOMENTUM)
		{//�΂߈ړ����̉^���͐���
			_forward_value = DIADONAL_MOVEMENT_VALUE * _forward_value;
			_right_value   = DIADONAL_MOVEMENT_VALUE * _right_value;
		}

		//���x
		float velocity = m_Owner->GetPlayerVelocity();

		//�����x�N�g��
		Vector3 forward, right, up;

		if (Camera* camera = Manager_Game::Instance()->GetLevel()->GetActor<Camera>())
		{//�J����������ꍇ

			if (camera->GetOrientRotationtoMovement())
			{//�v���C���[���ړ����[�h

				forward = m_Owner->GetForward();//�L�����N�^�[�̃t�H���[�h�擾
				right   = m_Owner->GetRight();  //�L�����N�^�[�̃��C�g�擾

				//�O�㎲
				m_Component_Rigidbody->SetMoveXZ(forward * velocity * _forward_value);
				//���E��
				m_Component_Rigidbody->SetMoveXZ(right   * velocity * _right_value);

			}
			else
			{//�J�������ړ����[�h

				//���݂̉�]�擾
				float rotationY       = m_Owner->GetRotation().y;
				//�J�����̉�]�擾
				float cameraRotationY = camera->GetRotation().y;

				//�J�����̕����x�N�g���擾
				forward = camera->GetForward();//�J�����̃t�H���[�h�擾
				right   = camera->GetRight();  //�J�����̃��C�g�擾
				up      = camera->GetUp();     //�J�����̃A�b�v�擾

				//�A�b�v�ƃt�H���[�h��Pitch���ŕ�Ԃ��Ĉړ��x�N�g���ɗp����
				D3DXVec3Lerp(&forward, &forward, &up, camera->GetRotation().x / VIEW_PITCH_MAX);

				//�O�㎲
				m_Component_Rigidbody->SetMoveXZ(forward * velocity * _forward_value);
				//���E��
				m_Component_Rigidbody->SetMoveXZ(right   * velocity * _right_value);

				//�L�����N�^�[��]���`��ԕ����v�Z
				int   input_cnt = 0;//�L�[���͐�
				float degree = 0.0f;//�x��
				int   vector_pattern = 0;//�����̃p�^�[��

				if (_right_value > EFFECTIVE_VALUE)
				{//�E�������

					//�L�[���͐����Z
					input_cnt++;

					//180�x�ȏ��]���Ă��܂��ꍇ�t�ŕ␳
					if (rotationY <= cameraRotationY + DegToRad(-135.0f))
					{
						rotationY = cameraRotationY + DegToRad(180.0f);
					}

					//�i�s�̃p�^�[��
					vector_pattern = 1;

					//�J�������猩�ĉE�����w��
					degree += DegToRad(90.0f);

				}
				else if (_right_value < -EFFECTIVE_VALUE)
				{//���������

					//�L�[���͐����Z
					input_cnt++;

					//180�x�ȏ��]���Ă��܂��ꍇ�t�ŕ␳
					if (rotationY >= cameraRotationY + DegToRad(135.0f))
					{
						rotationY = cameraRotationY + DegToRad(-180.0f);
					}

					//�i�s�̃p�^�[��
					vector_pattern = 2;

					//�J�������猩�č������w��
					degree += DegToRad(-90.0f);
				}
				if (_forward_value > EFFECTIVE_VALUE)
				{//�O�������

					//�L�[���͐����Z
					input_cnt++;

				}
				else if (_forward_value < -EFFECTIVE_VALUE)
				{//���������

					//�L�[���͐����Z
					input_cnt++;

					//���ƕ��̐�Βl�����������ŕ��
					if (vector_pattern == 0)
					{
						(fabsf(rotationY - cameraRotationY + DegToRad(180.0f)) < fabsf(rotationY - cameraRotationY + DegToRad(-180.0f))) ? degree += DegToRad(-180.0f) : degree += DegToRad(180.0f);
					}
					else if (vector_pattern == 1)
					{
						degree += DegToRad(180.0f);
					}
					else
					{
						degree += DegToRad(-180.0f);
					}
				
		

				}
				if (input_cnt > 0)
				{//�L�[���͂��������ꍇ

					//���͕������ŉ�]�p������
					cameraRotationY += degree / input_cnt;

					////�J�����ƃL�����N�^�[�̉�]�������Ԃ���]�p���
					rotationY = Math::lerpf(rotationY, cameraRotationY, LERP_ROTATION_SPEED);

					//��]�p�ݒ�
					m_Owner->SetRotation(Vector3{ m_Owner->GetRotation().x,Math::clamp(rotationY,DegToRad(-360.0f),DegToRad(360.0f)),m_Owner->GetRotation().z});

				}
			}
		}
	}
}

void CharacterState::Jump()//�W�����v
{
	if (m_Component_Rigidbody != nullptr)
	{
		m_Component_Rigidbody->Jump();
	}
}
