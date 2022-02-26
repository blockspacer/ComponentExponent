//=============================================================================
//
//  テスト用固定2Dアクタークラス [TestActor2D_Static.h]
//  Date   : 2021/11/3
//
//=============================================================================
#pragma once


#include "../../Base/Actor2D.h"

class TestActor2D_Static : public Actor2D
{
private:

	class Component_Sprite*    m_Component_Sprite;   //スプライトコンポーネント

public:

	 static void Load();//リソース読み込み
	 static void Unload();//リソース削除

	 void Init()override;//初期化
	 void Uninit()override;//終了
	 void Update()override;//更新

};