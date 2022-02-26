//=============================================================================
//
//  足音サウンドクラス [FootStep.h]
//  Date   : 2021/1/21
//
//=============================================================================
#pragma once

#include <unordered_map>
#include <vector>

#include "../Manager/Manager_Drawer/Manager_OBB.h"

class FootStep
{
private:

	std::vector<const char*> m_FootStepes[MAX_PM_TYPE];//

public:

	static void Load();//リソース読み込み
	static void Unload();//リソース削除

	//初期化・終了
	void Init();
	void Uninit();

	//プレイ
	void Play(int _physicsMaterial);

};