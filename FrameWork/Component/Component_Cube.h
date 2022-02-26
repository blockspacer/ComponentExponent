//=============================================================================
//
//  キューブコンポーネント [Component_Cube.h]
//  Author : Ryosuke Kotera
//  Date   : 2021/1/19
//
//=============================================================================
#pragma once

#include "Base/Component.h"
#include "../Manager/Manager_Drawer/Manager_Cube.h"

class Component_Cube : public Component
{
public:

	Component_Cube(class Actor* _owner, int _updatePriority) : Component(_owner, _updatePriority) {}

	void Init()   override;//初期化
	void Uninit() override;//終了
	void Update() override {};//更新

	//描画
	void Draw();

	//テクスチャーセット
	void SetTexture(const char* _Key);

	//色設定
	inline void SetColor(Color _color) { m_Color = _color; }

	bool SaveComponentData(const char* _fileName) override;
	bool LoadComponentData(const char* _fileName) override;

private:

	//テクスチャー
	ID3D11ShaderResourceView*  m_Texture;

	//キューブマネージャーキャッシュ用
	class Manager_Cube* m_Manager_Cube;

	//色
	Color   m_Color;

	//使用するシェーダーの種類
	int  m_ShaderType;
	
};


