//=============================================================================
//
//  アニメーションスプライトコンポーネント [Component_SpriteAnimation.cpp]
//  Date   : 2021/11/04
//
//=============================================================================

#include "Component_SpriteAnimation.h"
#include "../Actor/2D/Base/Actor2D.h"
#include "../Manager/Manager_Shader.h"
#include "../Manager/Manager_Game.h"
#include "../Manager/Manager_Texture.h"
#include "../Manager/Manager_Drawer/Manager_Sprite.h"
#include "../Renderer/Renderer_DX11.h"
#include "../Level/Base/Level.h"


//初期化
void Component_SpriteAnimation::Init()
{
	//初期化
	Component_Sprite::Init();

	m_AnimationInformation.AnimCount       = 0;//現在のフレーム数
	m_AnimationInformation.MaxAnimCount    = 0;//何フレームで更新するか

	m_AnimationInformation.OneFrameSize    = {0.0f,0.0f};//1コマ辺りのサイズ
	m_AnimationInformation.CutStartPostion = {0.0f,0.0f};//切り取り開始座標

	//ループさせるかフラグ
	m_AnimationInformation.Loop = true;
}

//更新
void Component_SpriteAnimation::Update()
{
	if (m_Active)//有効か
	{
		//現在分のフレーム数加算
		m_AnimationInformation.AnimCount++; 

		if (m_AnimationInformation.AnimCount == m_AnimationInformation.MaxAnimCount)
		{//更新フレーム到達時にアニメーション更新

			//現在の累積フレーム数リセット
			m_AnimationInformation.AnimCount = 0;

			if (m_AnimationInformation.CutStartPostion.x + m_AnimationInformation.OneFrameSize.x < MAX_CUT_START_POSTION)
			{//X方向にアニメーション更新可能か？
				m_AnimationInformation.CutStartPostion.x += m_AnimationInformation.OneFrameSize.x;//X切り取り座標を1コマ分ずらす
			}
			else
			{//X方向にアニメーション更新可不可時

				m_AnimationInformation.CutStartPostion.x = 0.0f;//X切り取り座標初期化

				if (m_AnimationInformation.CutStartPostion.y + m_AnimationInformation.OneFrameSize.y < MAX_CUT_START_POSTION)
				{//Y方向にアニメーション更新可能か？
					m_AnimationInformation.CutStartPostion.y += m_AnimationInformation.OneFrameSize.y;//Y切り取り座標を1コマ分ずらす
				}
				else
				{//XとY共に更新不可

					m_AnimationInformation.CutStartPostion.y = 0.0f;//Y切り取り座標初期化

					if (!m_AnimationInformation.Loop)//ループしないアニメーションの場合
					{//アクティブをfalseに
						m_Active = false;
					}
				}
			}
		}
	}
}

void Component_SpriteAnimation::Uninit()
{
	Component_Sprite::Uninit();
}

//描画
void Component_SpriteAnimation::Draw()
{
	if (m_Active && m_Texture)//有効か
	{
		//シェーダー設定
		Manager_Shader::Instance()->SetShader(m_ShaderType);

		// テクスチャ設定
		Renderer_DX11::Instance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		//プレーンレンダラーから描画
		Renderer_Polygon::Instance()->Draw2DAnimation(m_Position2D, m_Scale2D, m_Color,
			m_AnimationInformation.OneFrameSize, m_AnimationInformation.CutStartPostion, m_Reverse);
	}
}

bool Component_SpriteAnimation::SaveComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName,  "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Save<Component_SpriteAnimation>(_fileName, this, sizeof(Component_SpriteAnimation), tempName))
	{
		return false;
	}
	return true;
}

bool Component_SpriteAnimation::LoadComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName, "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Load<Component_SpriteAnimation>(_fileName, this, tempName))
	{
		return false;
	}
	return true;
}
