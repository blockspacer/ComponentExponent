//=============================================================================
//
//  モデル関連コンポーネント基底 [Componet_ModelBase.h]
//  Date   : 2021/12/08
//
//=============================================================================
#pragma once

#include <unordered_map>
#include "../Base/Component.h"

class Component_ModelBase : public Component
{
protected:

	int m_ShaderType;
	float m_ScaleRate;//描画スケール調整

	Vector3 m_PositionOffset;//オフセット値
	Vector3	m_RotationOffset;

	ID3D11Buffer** m_VertexBuffer;
	ID3D11Buffer** m_IndexBuffer;

	Vector4 m_ModelColor;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

public:

	Component_ModelBase(class Actor* _owner, int _updatePriority) :
		Component(_owner, _updatePriority) {}

	virtual void Draw() = 0;

	//オフセット値設定
	inline void SetPositionOffset(Vector3 _set) { m_PositionOffset = _set; }
	inline void SetRotationOffset(Vector3 _set) { m_RotationOffset = _set; }

	//シェーダーセット
	inline void SetShaderType(int _type) { m_ShaderType = _type; }

	//サイズ比率設定
	inline void SetScaleRate(float _scaleRate) { m_ScaleRate = _scaleRate; }

	virtual inline void SetColor(Vector4 _color) { m_ModelColor; }
	inline Vector4 GetColor() { return m_ModelColor; }
};