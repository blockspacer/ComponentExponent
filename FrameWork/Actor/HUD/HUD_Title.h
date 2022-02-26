//=============================================================================
//
//  Titleレベル用HUDクラス [HUD_Title.cpp]
//  Date   : 2021/1/9
//
//=============================================================================
#pragma once


#include "Base/HUD.h"


class HUD_Title : public HUD
{
private:

	class Component_Sprite*    m_Component_Sprite;    //スプライトコンポーネント
	class Component_Sprite*    m_Component_Sprite2;   //スプライトコンポーネント2

	bool m_LerpVector;

	float m_Opacity;

public:

	void Init()  override;//初期化
	void Uninit()override;//終了
	void Update()override;//更新

	//描画
	void Draw()override;

};