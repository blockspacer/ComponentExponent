//=============================================================================
//
//  テスト用ビルボードクラス [TestBillboard.h]
//  Date   : 2021/11/3
//
//=============================================================================
#pragma once


#include "../../Base/Actor3D.h"


class TestBillboard : public Actor3D
{
private:

	class Component_Billboard*    m_Component_Billboard;   //ビルボードコンポーネント

public:

	 static void Load();//リソース読み込み
	 static void Unload();//リソース削除

	 void Init()override;//初期化
	 void Uninit()override;//終了
	 void Update()override;//更新

};