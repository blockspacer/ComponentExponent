//=============================================================================
//
//  Test���x���pHUD�N���X [HUD_Test.h]
//  Date   : 2021/12/3
//
//=============================================================================
#pragma once


#include "Base/HUD.h"


class HUD_Test : public HUD
{
private:

	class Component_Sprite*    m_Component_Sprite;    //�X�v���C�g�R���|�[�l���g
	class Component_Sprite*    m_Component_Sprite2;   //�X�v���C�g�R���|�[�l���g2

public:

	void Init()  override;//������
	void Uninit()override;//�I��
	void Update()override;//�X�V

	//�`��
	void Draw()override;

};