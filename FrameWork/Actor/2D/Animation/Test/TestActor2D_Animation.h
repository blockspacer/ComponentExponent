//=============================================================================
//
//  �e�X�g�p�A�j���[�V����2D�A�N�^�[�N���X [TestActor2D_Animation.h]
//  Date   : 2021/11/3
//
//=============================================================================
#pragma once


#include"../../Base/Actor2D.h"


class TestActor2D_Animation : public Actor2D
{
private:

	class Component_SpriteAnimation*    m_Component_SpriteAnimation;   //�A�j���[�V�����X�v���C�g�R���|�[�l���g

public:

	 static void Load();//���\�[�X�ǂݍ���
	 static void Unload();//���\�[�X�폜

	 void Init()override;//������
	 void Uninit()override;//�I��
	 void Update()override;//�X�V

};