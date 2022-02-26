//=============================================================================
//
//  3D�A�N�^�[�N���X [Actor3D.cpp]
//  Date   : 2021/11/5
//
//=============================================================================


#include "Actor3D.h"


void  Actor3D::Init()//������
{
	m_Position       = Vector3_Zero;//3�������W�����l
	m_Rotation       = Vector3_Zero;//3������]�����l
	m_Scale          = Vector3{ 1.0f,1.0f,1.0f };//3�����T�C�Y�����l
}

void Actor3D::Update()//�X�V
{
	//Yaw�����K��
	if (m_Rotation.y < DegToRad(-360.0f))
	{
		m_Rotation.y += DegToRad(360.0f);

	}
	else if (m_Rotation.y > DegToRad(360.0f))
	{
		m_Rotation.y -= DegToRad(360.0f);
	}

	Actor::Update();//�X�V

}

void Actor3D::Uninit()//�I��
{
	Actor::Uninit();//�I��
}
