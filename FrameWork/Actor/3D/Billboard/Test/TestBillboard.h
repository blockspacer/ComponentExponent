//=============================================================================
//
//  �e�X�g�p�r���{�[�h�N���X [TestBillboard.h]
//  Date   : 2021/11/3
//
//=============================================================================
#pragma once


#include "../../Base/Actor3D.h"


class TestBillboard : public Actor3D
{
private:

	class Component_Billboard*    m_Component_Billboard;   //�r���{�[�h�R���|�[�l���g

public:

	 static void Load();//���\�[�X�ǂݍ���
	 static void Unload();//���\�[�X�폜

	 void Init()override;//������
	 void Uninit()override;//�I��
	 void Update()override;//�X�V

};