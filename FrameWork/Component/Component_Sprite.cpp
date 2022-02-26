//=============================================================================
//
//  スプライトコンポーネント [Component_Sprite.cpp]
//  Date   : 2021/11/02
//
//=============================================================================

#include "Component_Sprite.h"
#include "../Actor/2D/Base/Actor2D.h"
#include "../Manager/Manager_Shader.h"
#include "../Manager/Manager_Game.h"
#include "../Manager/Manager_Texture.h"
#include "../Manager/Manager_Drawer/Owner/Manager_Drawer.h"
#include "../Manager/Manager_Drawer/Manager_Sprite.h"
#include "../Renderer/Renderer_DX11.h"
#include "../Renderer/Renderer_Polygon.h"
#include "../Level/Base/Level.h"


//テクスチャー設定
void Component_Sprite::SetTexture(const char* Key)
{
	if (ID3D11ShaderResourceView* texture = Manager_Texture::Instance()->GetTexture(Key))
	{//指定のキーのテクスチャーがあるか確認
		m_Texture = texture;//設定
	}
}

bool Component_Sprite::SaveComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName,  "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Save<Component_Sprite>(_fileName, this, sizeof(Component_Sprite), tempName))
	{
		return false;
	}
	return true;
}

bool Component_Sprite::LoadComponentData(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;

	char tempName[256] = {};
	sprintf_s(tempName, "%s_%d", m_Owner->GetName(), typeid(this).hash_code());
	if (!DataSaver::Instance()->Load<Component_Sprite>(_fileName, this, tempName))
	{
		return false;
	}
	return true;
}


//初期化
void Component_Sprite::Init()
{
	//基底初期化
	Component::Init();

	//デフォルトはアンリットシェーダー
	m_ShaderType = Manager_Shader::SHADER_UNLIT;

	//反転フラグ
	m_Reverse = false;

	//デフォルトテクスチャ使用フラグ
	m_isUseDefaultTexture = false;

	//色初期値
	m_Color = { 1.0f,1.0f,1.0f,1.0f };

	//Z座標初期値
	m_PostionZ = 0.0f;

	if (Actor2D* actor2D = dynamic_cast<Actor2D*>(m_Owner))//Actor2Dにキャスト
	{
		//オーナーの座標キャッシュ
		m_Position2D = actor2D->GetPosition2D();
		//オーナーのスケールキャッシュ
		m_Scale2D = actor2D->GetScale2D();
	}

	if (Manager_Sprite* manager_Sprite = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Sprite())
	{//スプライトマネージャーが取得できたら登録完了
		manager_Sprite->AddComponent(this);
	}

}
//終了
void Component_Sprite::Uninit()
{
	if (m_Texture)
	{//キャッシュしたテクスチャをnullptr化
		m_Texture = nullptr;
	}

	if (Manager_Sprite* manager_Sprite = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Sprite())
	{//スプライトマネージャーが取得できたら登録完了
		//manager_Sprite->AddComponent<Component_Sprite>(this);
		manager_Sprite->RemoveComponent(this);
	}
}

//描画
void Component_Sprite::Draw()
{
	if (m_Active && m_Texture)//有効か
	{
		//シェーダー設定
		Manager_Shader::Instance()->SetShader(m_ShaderType);

		// テクスチャ設定
		Renderer_DX11::Instance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		//プレーンレンダラーから描画
		Renderer_Polygon::Instance()->Draw2D(m_Position2D, m_Scale2D, m_Color, m_Reverse, m_PostionZ);
	}
}
