//=============================================================================
//
//  スプライトコンポーネント [Component_Sprite.h]
//  Date   : 2021/11/02
//
//=============================================================================
#pragma once
#include "Base/Component.h"
#include "../Manager/Manager_Shader.h"

class Component_Sprite : public Component
{

protected:

	//テクスチャキャッシャー
	ID3D11ShaderResourceView*  m_Texture;
	//使用するシェーダーの種類
	int   m_ShaderType;
	//反転フラグ
	bool  m_Reverse;
	//デフォルトテクスチャを使用するかのフラグ
	bool m_isUseDefaultTexture;
	//二次元座標
	Vector2 m_Position2D;
	//二次元サイズ
	Vector2 m_Scale2D;
	//色
	Color m_Color;
	//Z座標
	float m_PostionZ;

public:

	Component_Sprite(class Actor* owner, int updatePriority) : Component(owner, updatePriority) {}


	virtual void Init()override;	//初期化
	virtual void Uninit()override;  //終了
	virtual void Update()override{};//更新
	
	//描画
	virtual void Draw();

	//画像セット
	void SetTexture(const char* Key);
	void SetShaderType(Manager_Shader::SHADER_TYPE type) { m_ShaderType = type; }

	//フォント用に直にテクスチャをセットする関数
	inline void SetTexture(ID3D11ShaderResourceView* _rv) { m_Texture = _rv; }

	//二次元座標設定
	inline void SetPosition2D(Vector2 position3D) { m_Position2D = position3D; }
	//二次元サイズ設定
	inline void SetScale2D(Vector2 scale2D)       { m_Scale2D = scale2D; }
	//色設定
	inline void SetColor(Color color)             { m_Color = color; }
	//Z座標設定
	inline void SetPostionZ(float z)              { m_PostionZ = z; }

	bool SaveComponentData(const char* _fileName) override;
	bool LoadComponentData(const char* _fileName) override;
};
