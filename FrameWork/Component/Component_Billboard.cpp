//=============================================================================
//
//  ビルボードコンポーネント [Component_Billboard.cpp]
//  Date   : 2021/11/05
//
//=============================================================================

#include "Component_Billboard.h"
#include "../Actor/3D/Camera.h"
#include "../Manager/Manager_Shader.h"
#include "../Manager/Manager_Game.h"
#include "../Manager/Manager_Texture.h"
#include "../Manager/Manager_Drawer/Owner/Manager_Drawer.h"
#include "../Manager/Manager_Drawer/Manager_Billboard.h"
#include "../Renderer/Renderer_DX11.h"
#include "../Level/Base/Level.h"



//テクスチャー設定
void Component_Billboard::SetTexture(const char* _Key)
{
	if (ID3D11ShaderResourceView* texture = Manager_Texture::Instance()->GetTexture(_Key))
	{//指定のキーのテクスチャーがあるか確認
		m_Texture = texture;//設定
	}
}


//初期化
void Component_Billboard::Init()
{
	Component::Init();//基底初期化

	m_ShaderType = Manager_Shader::SHADER_UNLIT;//デフォルトはアンリットシェーダー

	m_AnimationInformation.AnimCount    = 0;       //現在のフレーム数
	m_AnimationInformation.MaxAnimCount = 0;       //何フレームで更新するか

	m_AnimationInformation.OneFrameSize    = { 0.0f,0.0f };//1コマ辺りのサイズ
	m_AnimationInformation.CutStartPostion = { 0.0f,0.0f };//切り取り開始座標

	m_AnimationInformation.Loop     = true;//ループさせるかフラグ

	m_Color = { 1.0f,1.0f,1.0f,1.0f };//色初期値

	if (Manager_Billboard* manager_Billboard = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Billboard())
	{//マネージャー取得できたら登録
		manager_Billboard->AddComponent(this);
	}


}
//終了
void Component_Billboard::Uninit()
{
	if (m_Texture)
	{
		m_Texture = nullptr;
	}

	if (Manager_Billboard* manager_Billboard = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Billboard())
	{//マネージャー取得できたら登録
		manager_Billboard->RemoveComponent(this);
	}
}

//更新
void Component_Billboard::Update()
{
	if (m_Active)//有効か
	{
		m_AnimationInformation.AnimCount++; //現在のフレーム数加算

		if (m_AnimationInformation.AnimCount == m_AnimationInformation.MaxAnimCount)
		{//更新フレーム到達時にアニメーション更新

			m_AnimationInformation.AnimCount = 0;//現在のフレーム数リセット

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
					{
						m_Active = false;
					}
				}
			}
		}
	}
}


//描画
void Component_Billboard::Draw()
{
	if (m_Active && m_Texture)//有効か
	{
		if (Actor3D* actor3D = dynamic_cast<Actor3D*>(m_Owner))//Actor3Dにキャスト
		{
			if (!actor3D->GetIsActive())return;//非アクティブなら描画しない

			//シェーダー設定
			Manager_Shader::Instance()->SetShader(m_ShaderType);

			// テクスチャ設定
			Renderer_DX11::Instance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

			//プレーンマネージャーで描画
			Renderer_Polygon::Instance()->DrawBillboard(actor3D->GetPosition(), actor3D->GetScale(),m_Color, m_AnimationInformation.OneFrameSize, m_AnimationInformation.CutStartPostion);
		}
	}
}
