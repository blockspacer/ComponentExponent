//=============================================================================
//
//  ビルボードコンポーネント [Component_Billboard.cpp]
//  Date   : 2021/11/05
//
//=============================================================================
#pragma once
#include "Base/Component.h"
#include "../Renderer/Renderer_Polygon.h"

class Component_Billboard : public Component
{
private:

	//テクスチャー
	ID3D11ShaderResourceView*  m_Texture;

	//スプライトアニメーション構造体
	s_SpriteAnimation m_AnimationInformation;

	//使用するシェーダーの種類
	int   m_ShaderType; 

	//色 
	Color m_Color;


public:

	Component_Billboard(class Actor* _owner, int _updatePriority) : Component(_owner, _updatePriority) {}


	virtual void Init()override;    //初期化
	virtual void Uninit()override;  //終了
	virtual void Update() override;	//更新

	virtual void Draw();//描画

	void SetTexture(const char* _Key);//画像セット

	inline void SetColor(Color _color) { m_Color = _color;}//色設定

	inline void CalculationOneFrameSize(int _W_frame, int _H_frame)
	{//1コマ辺りのサイズ割り出し
		m_AnimationInformation.OneFrameSize.x = static_cast<float>(1.0f / _W_frame);
		m_AnimationInformation.OneFrameSize.y = static_cast<float>(1.0f / _H_frame);
	}

	inline void SetLoop(bool _loop) { m_AnimationInformation.Loop = _loop; }//ループさせるか設定
	inline void SetMaxAnimCount(int _max_count) { m_AnimationInformation.MaxAnimCount = _max_count; }//何フレームで更新するか設定
};
