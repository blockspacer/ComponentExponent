//=============================================================================
//
//  2D�A�N�^�[�N���X [Actor2D.cpp]
//  Date   : 2021/11/1
//
//=============================================================================


#include "Actor2D.h"


void  Actor2D::Init()//������
{
	m_Position2D       = Vector2_Zero;//2�������W�����l
	m_Scale2D          = Vector2_Zero;//2�����T�C�Y�����l
}

void Actor2D::Update()//�X�V
{
	Actor::Update();//�X�V
}

void Actor2D::Uninit()//�I��
{
	Actor::Uninit();//�I��
}
