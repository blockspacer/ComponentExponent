//=============================================================================
//
//  2D�A�N�^�[�N���X [Actor2D.h]
//  Date   : 2021/11/1
//
//=============================================================================
#pragma once


#include "../../Base/Actor.h"

class Actor2D : public Actor
{
protected:

	Vector2 m_Position2D;//2�������W
	Vector2 m_Scale2D;  //2�����X�P�[��

public:

	virtual void Init()override;//������
	virtual void Uninit()override;//�I��
	virtual void Update()override;//�X�V

	inline Vector2 GetPosition2D() const { return m_Position2D;}//2�������W�擾
	inline Vector2 GetScale2D()    const { return m_Scale2D; }  //2�����X�P�[���擾

	inline void SetPosition2D(Vector2 _position3D) { m_Position2D  = _position3D; }//2�������W�ݒ�
	inline void SetScale2D   (Vector2 _scale2D)     { m_Scale2D    = _scale2D; }   //2�����X�P�[���ݒ�

};