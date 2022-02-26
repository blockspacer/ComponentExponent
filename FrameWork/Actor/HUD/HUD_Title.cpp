//=============================================================================
//
//  Titleレベル用HUDクラス [HUD_Title.cpp]
//  Date   : 2021/1/9
//
//=============================================================================


#include "HUD_Title.h"
#include "../../Component/Component_Sprite.h"
#include "../../Manager/Manager_Texture.h"
#include "../../Manager/Manager_Drawer/Manager_Font2D.h"
#include "../../Manager/Manager_Font3D.h"
#include "../../Renderer/Renderer_Polygon.h"
#include "../../System/Input.h"

constexpr float LERP_SPEED = 0.085f;//補間速度
constexpr float LERP_START = 0.01f;//補間開始
constexpr float LERP_END   = 0.99f;//補間終了

void HUD_Title::Init()//初期化
{
	//使用テクスチャ読み込み
	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//テクスチャーマネージャーキャッシュ
		manager_Texture->LoadTexture("ClickToStart", "ROM/2D/Title/T_ClickToStart.png");
		manager_Texture->LoadTexture("TitleLogo",    "ROM/2D/Title/T_Title.png");
	}

	//スプライトコンポーネント設定
	m_Component_Sprite = AddComponent<Component_Sprite>(0);//追加
	m_Component_Sprite->SetTexture("ClickToStart");	//テクスチャー設定
	m_Component_Sprite->SetPosition2D(Vector2{ 0.0f, 200.0f });
	m_Component_Sprite->SetScale2D   (Vector2{ 564.0f, 75.0f });

	m_Component_Sprite2 = AddComponent<Component_Sprite>(0);//追加
	m_Component_Sprite2->SetTexture("TitleLogo");	//テクスチャー設定
	m_Component_Sprite2->SetPosition2D(Vector2{ 0.0f, -175.0f });
	m_Component_Sprite2->SetScale2D(Vector2{ 830.0f, 160.0f });

	m_Opacity = 1.0f;

	m_LerpVector = false;
}

void HUD_Title::Update()//更新
{
	HUD::Update();


	//クリックスタート点滅
	if (m_LerpVector)
	{
		m_Opacity = Math::lerpf(m_Opacity, 1.0f, LERP_SPEED);
		if (m_Opacity >= LERP_END)
		{
			m_LerpVector = !m_LerpVector;
		}
	}
	else
	{
		m_Opacity = Math::lerpf(m_Opacity, 0.0f, LERP_SPEED);
		if (m_Opacity <= LERP_START)
		{
			m_LerpVector = !m_LerpVector;
		}
	}

	m_Component_Sprite->SetColor(Color{ 1.0f, 1.0f, 1.0f, m_Opacity });
}
	
void HUD_Title::Uninit()//終了
{
	HUD::Uninit();
}

void HUD_Title::Draw()//描画
{

	
}
