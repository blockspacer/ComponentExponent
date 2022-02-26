//=============================================================================
//
//  Title���x���pHUD�N���X [HUD_Title.cpp]
//  Date   : 2021/1/9
//
//=============================================================================
#pragma once


#include "Base/HUD.h"


class HUD_Title : public HUD
{
private:

	class Component_Sprite*    m_Component_Sprite;    //�X�v���C�g�R���|�[�l���g
	class Component_Sprite*    m_Component_Sprite2;   //�X�v���C�g�R���|�[�l���g2

	bool m_LerpVector;

	float m_Opacity;

public:

	void Init()  override;//������
	void Uninit()override;//�I��
	void Update()override;//�X�V

	//�`��
	void Draw()override;

};