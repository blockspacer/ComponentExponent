//=============================================================================
//
//  メッシュフィールド描画 [MeshField.cpp]
//  Date   : 2021/11/20
//
//=============================================================================
//******************************************************************************
// インクルード
//******************************************************************************
#include "MeshField.h"
#include "../../../Manager/Manager_Game.h"
#include "../../../Manager/Manager_Texture.h"
#include "../../../Manager/Manager_Shader.h"
#include "../../../Manager/Manager_Drawer/Owner/Manager_Drawer.h"
#include "../../../Manager/Manager_Drawer/Manager_Landscape.h"
#include "../../../Level/Base/Level.h"
//============================================================================
// 頂点情報
//=============================================================================
float g_FieldHeight[21][21] =
{
    {0.0f,0.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,1.0f},
    {0.0f,5.0f,0.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
    {1.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,2.0f,0.0f},
    {0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
    {0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
    {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,2.0f},
    {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
    {0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
    {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
    {0.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f},
    {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f},
    {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,9.0f,2.0f},
    {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f},
    {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
    {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f},
    {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
    {1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f},
    {0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f},
    {0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
    {0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f},
    {0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f}
};

//=============================================================================
// リソース
//=============================================================================
void  MeshField::Load()//リソース読み込み
{

	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//テクスチャーマネージャーキャッシュ
		manager_Texture->LoadTexture("MeshField", "ROM/2D/Test/T_Sand.png");
	}

}
void  MeshField::Unload()//リソース削除
{

	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//テクスチャーマネージャーキャッシュ
		manager_Texture->UnloadTexture("MeshField");
	}

}

//=============================================================================
// 高さ取得関数
//=============================================================================
float MeshField::GetHeight(Vector3 _position)
{
	int x, z;

	x = static_cast<int>(_position.x /  5.0f + 10.0f);
	z = static_cast<int>(_position.z / -5.0f + 10.0f);

	Vector3 pos0, pos1, pos2, pos3;

	pos0 = m_Vertex[x][z].Position;
	pos1 = m_Vertex[x + 1][z].Position;
	pos2 = m_Vertex[x][z + 1].Position;
	pos3 = m_Vertex[x + 1][z + 1].Position;

	Vector3 v12, v1p, c;

	v12 = pos2     - pos1;
	v1p = _position - pos1;
	D3DXVec3Cross(&c, &v12, &v1p);

	float py;
	Vector3 n;
	if (c.y > 0.0f)
	{
		Vector3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);
	}
	else
	{
		Vector3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
	}
	py = -((_position.x - pos1.x) * n.x + (_position.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;

}
//=============================================================================
// 初期化関数
//=============================================================================
void MeshField::Init()
{
	// 頂点の設定
	for (int x = 0; x <= 20; x++)
	{
		for (int z = 0; z <= 20; z++)
		{
			m_Vertex[x][z].Position = Vector3((x - 10) *  5.0f,g_FieldHeight[x][z], (z - 10) * -5.0f);
			m_Vertex[x][z].Normal   = Vector3(0.0f, 1.0f, 0.0f);
			m_Vertex[x][z].Diffuse  = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			m_Vertex[x][z].TexCoord = Vector2(x * 0.5f, z * 0.5f);
		}
	}
	for (int x = 0; x <= 19 ; x++)
	{
		for (int z = 0; z <= 19 ; z++)
		{
			D3DXVECTOR3 vx, vz, vn;

			vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;
			vx = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;
			D3DXVec3Cross(&vn, &vz, &vx);
			D3DXVec3Normalize(&vn, &vn);
			m_Vertex[x][z].Normal = vn;
		}
	}
	D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 21 * 21;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_Vertex;

	Renderer_DX11::Instance()->GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	
	//インデックスバッファ生成
	unsigned int index[(22 * 2) * 20 - 2];

	int i = 0;
	for (int x = 0; x < 20; x++)
	{
		for (int z = 0; z < 21; z++)
		{
			index[i] = x * 21 + z;
			i++;
			index[i] = (x + 1) * 21 + z;
			i++;
		}
		if (x == 19)break;

		index[i] = (x + 1) * 21 + 20;
		i++;

		index[i] = (x + 1) * 21;
		i++;
	}
	// 頂点バッファ生成

	D3D11_BUFFER_DESC ibd{};
	ibd.ByteWidth = sizeof(unsigned int) * ((22 * 2) * 20 - 2);
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA isd{};
	isd.pSysMem = index;

	Renderer_DX11::Instance()->GetDevice()->CreateBuffer(&ibd, &isd, &m_IndexBuffer);

	m_ShaderType = Manager_Shader::SHADER_UNLIT;//デフォルトはアンリットシェーダー

	Actor3D::Init();

	if (ID3D11ShaderResourceView* texture = Manager_Texture::Instance()->GetTexture("MeshField"))
	{//指定のキーのテクスチャーがあるか確認
		m_Texture = texture;//設定
	}
	if (Manager_Landscape* manager_Landscape = Manager_Game::Instance()->GetLevel()->GetManager_Drawer()->GetManager_Landscape())
	{//マネージャー取得できたら登録
		manager_Landscape->AddComponent(this);
	}
}

//=============================================================================
// 終了処理関数
//=============================================================================
void MeshField::Uninit()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
}

//=============================================================================
// 更新処理関数
//=============================================================================
void MeshField::Update()
{

}

//=============================================================================
// 描画処理関数
//=============================================================================
void MeshField::Draw()
{
	//シェーダー設定
	Manager_Shader::Instance()->SetShader(m_ShaderType);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot*trans;
	Renderer_DX11::Instance()->SetWorldMatrix(&world);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer_DX11::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);


	Renderer_DX11::Instance()->GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer,DXGI_FORMAT_R32_UINT, 0);
	// テクスチャ設定
	Renderer_DX11::Instance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer_DX11::Instance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer_DX11::Instance()->GetDeviceContext()->DrawIndexed((22 * 2) * 20 - 2, 0,0);
}