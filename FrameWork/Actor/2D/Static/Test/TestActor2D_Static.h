//=============================================================================
//
//  �e�X�g�p�Œ�2D�A�N�^�[�N���X [TestActor2D_Static.h]
//  Date   : 2021/11/3
//
//=============================================================================
#pragma once


#include "../../Base/Actor2D.h"

class TestActor2D_Static : public Actor2D
{
private:

	class Component_Sprite*    m_Component_Sprite;   //�X�v���C�g�R���|�[�l���g

public:

	 static void Load();//���\�[�X�ǂݍ���
	 static void Unload();//���\�[�X�폜

	 void Init()override;//������
	 void Uninit()override;//�I��
	 void Update()override;//�X�V

};