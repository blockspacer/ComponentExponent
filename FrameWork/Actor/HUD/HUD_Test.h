//=============================================================================
//
//  Testレベル用HUDクラス [HUD_Test.h]
//  Date   : 2021/12/3
//
//=============================================================================
#pragma once


#include "Base/HUD.h"


class HUD_Test : public HUD
{
private:

	class Component_Sprite*    m_Component_Sprite;    //スプライトコンポーネント
	class Component_Sprite*    m_Component_Sprite2;   //スプライトコンポーネント2

public:

	void Init()  override;//初期化
	void Uninit()override;//終了
	void Update()override;//更新

	//描画
	void Draw()override;

};