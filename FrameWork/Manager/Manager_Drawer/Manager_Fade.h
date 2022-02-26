//=============================================================================
//
//  フェードマネージャー [Manager_Fade.h]
//  Date   : 2021/11/16
//
//=============================================================================

#pragma once
#include "../../System/Main.h"

class Manager_Fade
{
public:

	enum State
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_MAX
	};

	void Init();//初期化
	void Update();//更新
	void Draw();//描画

	void SetFade(State _fadeState, float _time);

	inline State GetState()    const { return m_FadeState; }
	inline float GetFadeTime() const { return m_FadeTime; }

private:

	int m_ShaderType;

	State m_FadeState = FADE_NONE;

	float m_FadeTime  = 0.0f;
	float m_StartTime = 0.0f;

	Color m_Color = Color(0.0f, 0.0f, 0.0f, 0.0f);

	ID3D11ShaderResourceView* m_Texture;//テクスチャ
};