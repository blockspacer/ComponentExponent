//=============================================================================
//
//  テスト用アニメーション2Dアクタークラス [TestActor2D_Animation.h]
//  Date   : 2021/11/3
//
//=============================================================================
#pragma once


#include"../../Base/Actor2D.h"


class TestActor2D_Animation : public Actor2D
{
private:

	class Component_SpriteAnimation*    m_Component_SpriteAnimation;   //アニメーションスプライトコンポーネント

public:

	 static void Load();//リソース読み込み
	 static void Unload();//リソース削除

	 void Init()override;//初期化
	 void Uninit()override;//終了
	 void Update()override;//更新

};