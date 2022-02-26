//=============================================================================
//
//  �X�v���C�g�R���|�[�l���g [Component_Sprite.h]
//  Date   : 2021/11/02
//
//=============================================================================
#pragma once
#include "Component_Sprite.h"
#include "../Manager/Manager_Shader.h"

class Component_Sprite_Z : public Component_Sprite
{
private:

	//Z���W
	float m_PostionZ;

public:

	Component_Sprite_Z(class Actor* owner, int updatePriority) : Component_Sprite(owner, updatePriority) {}

	//�`��
	virtual void Draw()override;

	//Z���W�ݒ�
	inline void SetPostionZ(float z) { m_PostionZ = z; }
};
