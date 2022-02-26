//=============================================================================
//
//  アニメーションスプライトコンポーネント [Component_SpriteAnimation.h]
//  Date   : 2021/11/04
//
//=============================================================================
#pragma once

#include "Component_Sprite.h"
#include "../Renderer/Renderer_Polygon.h"

class Component_SpriteAnimation : public Component_Sprite
{
private:
	//スプライトアニメーション構造体
	s_SpriteAnimation m_AnimationInformation;

public:
	Component_SpriteAnimation(class Actor* _owner, int _updatePriority) : Component_Sprite(_owner, _updatePriority) {}

	void Init()override;  //初期化
	void Update()override;//更新
	void Uninit() override;

	void Draw()override;  //描画関数

	inline void CalculationOneFrameSize(int _W_frame, int _H_frame)
	{//1コマ辺りのサイズ割り出し
		m_AnimationInformation.OneFrameSize.x = static_cast<float>(1.0f / _W_frame);
		m_AnimationInformation.OneFrameSize.y = static_cast<float>(1.0f / _H_frame);
	}

	inline void SetLoop(bool _loop) { m_AnimationInformation.Loop = _loop; }//ループさせるか設定
	inline void SetMaxAnimCount(int _max_count) { m_AnimationInformation.MaxAnimCount = _max_count; }//何フレームで更新するか設定

	bool SaveComponentData(const char* _fileName) override;
	bool LoadComponentData(const char* _fileName) override;
};
