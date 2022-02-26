//=============================================================================
//
//  スプライトコンポーネント(Z使用) [Component_Sprite_Z.cpp]
//  Date   : 2021/12/14
//
//=============================================================================

#include "Component_Sprite_Z.h"
#include "../Actor/3D/Base/Actor3D.h"
#include "../Manager/Manager_Shader.h"
#include "../Manager/Manager_Game.h"
#include "../Manager/Manager_Texture.h"
#include "../Manager/Manager_Drawer/Owner/Manager_Drawer.h"
#include "../Manager/Manager_Drawer/Manager_Sprite.h"
#include "../Renderer/Renderer_DX11.h"
#include "../Renderer/Renderer_Polygon.h"
#include "../Level/Base/Level.h"


//描画
void Component_Sprite_Z::Draw()
{
	if (m_Active && m_Texture)//有効か
	{
		//シェーダー設定
		Manager_Shader::Instance()->SetShader(m_ShaderType);

		// テクスチャ設定
		Renderer_DX11::Instance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		//プレーンレンダラーから描画
		Renderer_Polygon::Instance()->Draw2D(m_Position2D, m_Scale2D, m_Color, m_Reverse,m_PostionZ);
	}
}
