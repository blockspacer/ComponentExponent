//=============================================================================
//
// レベルデザイン用キューブクラス [Cube_LD.h]
//  Date   : 2021/1/19
//
//=============================================================================
#pragma once


#include "../Base/ActorRB.h"

class Cube_LD : public ActorRB
{
protected:

	class Component_Cube* m_Component_Cube;//キューブコンポーネント

public:

	static void Load();//リソース読み込み
	static void Unload();//リソース削除

	void Init()override;//初期化
	void Update()override;//更新
	void Uninit() override;

	void OffOBB();
	void AddOBB();


};