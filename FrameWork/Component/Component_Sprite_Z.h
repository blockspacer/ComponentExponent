//=============================================================================
//
//  スプライトコンポーネント [Component_Sprite.h]
//  Date   : 2021/11/02
//
//=============================================================================
#pragma once
#include "Component_Sprite.h"
#include "../Manager/Manager_Shader.h"

class Component_Sprite_Z : public Component_Sprite
{
private:

	//Z座標
	float m_PostionZ;

public:

	Component_Sprite_Z(class Actor* owner, int updatePriority) : Component_Sprite(owner, updatePriority) {}

	//描画
	virtual void Draw()override;

	//Z座標設定
	inline void SetPostionZ(float z) { m_PostionZ = z; }
};
