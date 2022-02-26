//=============================================================================
//
//  ポリゴンレンダラー [Renderer_Polygon.cpp]
//  Date   : 2021/11/08
//
//=============================================================================

#include "../System/MainWindow.h"
#include "Renderer_Polygon.h"
#include "../System/Math.h"
#include "../Level/Base/Level.h"
#include "../Actor/3D/Camera.h"
#include "../Manager/Manager_Game.h"
#include "../Manager/Manager_Texture.h"
#include "../Manager/Manager_Shader.h"
#include "../System/Exchanger.h"

constexpr int  VERTEX_NUM_PLANE =  4;//平面の頂点
constexpr int  VERTEX_NUM_LINE  =  2;//線の頂点
constexpr int  VERTEX_NUM_BOX   = 36;//ボックスの頂点

//初期化
void Renderer_Polygon::Init()
{
	if (m_Renderer_DX11 == nullptr)//初期化済み
	{
		cacheMain = MainWindow::Instance();

		//DX11描画機能キャッシュ
		m_Renderer_DX11 = Renderer_DX11::Instance();

		//2Dポリゴン2個四角頂点の設定
		{
			VERTEX_3D vertex[VERTEX_NUM_PLANE];
			SetCubePolygonVertexToDefault(vertex);//デフォルト値代入

			// 頂点バッファ生成
			//頂点書き換え可能モードにする
			D3D11_BUFFER_DESC bd{};
			bd.Usage = D3D11_USAGE_DYNAMIC;//ダイナミック
			bd.ByteWidth = sizeof(VERTEX_3D) * VERTEX_NUM_PLANE;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//0からWRITEに

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = vertex;

			HRESULT hr = m_Renderer_DX11->GetDevice()->CreateBuffer(&bd, &sd, &m_CubePolygonVertexBuffer);
			if (FAILED(hr))
			{
				MessageBox(NULL, "ポリゴン初期化ミス", "ERROR", MB_OK);;
			}
		}

		//line頂点の設定
		{
			VERTEX_3D vertex[VERTEX_NUM_LINE];
			vertex[0].Position = Vector3_Zero;
			vertex[0].Diffuse = Vector4(1, 1, 1, 1);
			vertex[0].TexCoord = Vector2(0,0);
			vertex[0].Normal = Vector3(0, 0, 0);

			vertex[1].Position = Vector3_Zero;
			vertex[1].Diffuse = Vector4(1, 1, 1, 1);
			vertex[1].TexCoord = Vector2(0, 0);
			vertex[1].Normal = Vector3(0, 0, 0);

			// 頂点バッファ生成
			//頂点書き換え可能モードにする
			D3D11_BUFFER_DESC bd{};
			bd.Usage = D3D11_USAGE_DYNAMIC;//ダイナミック
			bd.ByteWidth = sizeof(VERTEX_3D) * VERTEX_NUM_LINE;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//0からWRITEに

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = vertex;

			m_Renderer_DX11->GetDevice()->CreateBuffer(&bd, &sd, &m_LineVertexBuffer);
		}

		//3DBox頂点の設定
		{
			VERTEX_3D vertex3D[VERTEX_NUM_BOX];

			Set3DBoxVertexToDefault(vertex3D);//デフォルト値代入

			// 頂点バッファ生成
			//頂点書き換え可能モードにする
			D3D11_BUFFER_DESC bd3D{};
			bd3D.Usage = D3D11_USAGE_DYNAMIC;//ダイナミック
			bd3D.ByteWidth = sizeof(VERTEX_3D) * VERTEX_NUM_BOX;
			bd3D.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd3D.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//0からWRITEに

			D3D11_SUBRESOURCE_DATA sd3D{};
			sd3D.pSysMem = vertex3D;

			m_Renderer_DX11->GetDevice()->CreateBuffer(&bd3D, &sd3D, &m_VertexBuffer3DBox);

			if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
			{//テクスチャマネージャー取得
				manager_Texture->LoadTexture("white", "ROM/2D/Test/white.png");//ロード
				m_TextureWhite = manager_Texture->GetTexture("white");//キャッシュ
			}
		}
	}
}
//終了
void Renderer_Polygon::Uninit()
{
	if (m_CubePolygonVertexBuffer)
	{
		m_CubePolygonVertexBuffer->Release();
		m_CubePolygonVertexBuffer = nullptr;
	}
	if (m_VertexBuffer3DBox)
	{
		m_VertexBuffer3DBox->Release();
		m_VertexBuffer3DBox = nullptr;
	}
	if (m_LineVertexBuffer)
	{
		m_LineVertexBuffer->Release();
		m_LineVertexBuffer = nullptr;
	}
}

void Renderer_Polygon::SetDefaultTexture()
{
	if (m_TextureWhite)
	{
		m_Renderer_DX11->GetDeviceContext()->PSSetShaderResources(0, 1, &m_TextureWhite);
	}
}

//描画
void Renderer_Polygon::DrawBox(Vector3 _position, Vector3 _rotation, 
	Vector3 _scale,Color _color, float _texMagni)
{
	if (m_Renderer_DX11 != nullptr)//初期化済み
	{
		if (Camera* camera = Manager_Game::Instance()->GetLevel()->GetActor<Camera>())
		{//カメラがある場合

			if (!camera->CheckView(_position, _scale))
			{//視錐台カリング
				//return;
			}

			//頂点データ書き換え
			{
				D3D11_MAPPED_SUBRESOURCE msr;
				m_Renderer_DX11->GetDeviceContext()->Map(m_VertexBuffer3DBox, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
				VERTEX_3D* vertex3D = (VERTEX_3D*)msr.pData;

				//todo:書き換え意味無し
				Set3DBoxVertexToDefault(vertex3D, _scale);//デフォルト値代入

				for (int i = 0; i < VERTEX_NUM_BOX; i++)
				{
					vertex3D[i].Diffuse = Vector4{ _color };
				}

				m_Renderer_DX11->GetDeviceContext()->Unmap(m_VertexBuffer3DBox, 0);
			}

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			m_Renderer_DX11->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer3DBox, &stride, &offset);
			
			//テクスチャ設定
			//m_Renderer_DX11->GetDeviceContext()->PSSetShaderResources(0, 1, &m_TextureWhite);

		    // マトリクス設定
			D3DXMATRIX world, Scale, rot, trans;

			D3DXMatrixScaling(&Scale, _scale.x, _scale.y, _scale.z);
			D3DXMatrixRotationYawPitchRoll(&rot,_rotation.y, _rotation.x, _rotation.z);
			D3DXMatrixTranslation(&trans, _position.x, _position.y, _position.z);

			world = Scale * rot * trans;

			m_Renderer_DX11->SetWorldMatrix(&world);

			// プリミティブトポロジ設定
			m_Renderer_DX11->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// ポリゴン描画
			m_Renderer_DX11->GetDeviceContext()->Draw(VERTEX_NUM_BOX, 0);
		}
	}
}

//テクスチャなし2D矩形描画
void Renderer_Polygon::DrawBox2D(Vector2 _position, Vector2 _scale,
	Color _color, bool _reverse, float _z)
{
	if (m_Renderer_DX11 != nullptr)
	{
		//テクスチャ設定
		m_Renderer_DX11->GetDeviceContext()->PSSetShaderResources(0, 1, &m_TextureWhite);

		//シェーダーをアンリットに
		Manager_Shader::Instance()->SetShader(Manager_Shader::SHADER_TYPE::SHADER_UNLIT);

		//矩形描画
		Draw2D(_position, _scale,
			_color,_reverse,_z);
	}
}

//2Dスプライト描画
void Renderer_Polygon::Draw2D(Vector2 _position, Vector2 _scale,
	Color _color, bool _reverse,float _z)
{
	if (m_Renderer_DX11 != nullptr)
	{
		//書き換え開始
		D3D11_MAPPED_SUBRESOURCE msr;
		m_Renderer_DX11->GetDeviceContext()->Map(m_CubePolygonVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		float x = _position.x + HalfSize(cacheMain->GetWindowSize().x); //X座標取得
		float y = _position.y + HalfSize(cacheMain->GetWindowSize().y);//Y座標取得

		float w = HalfSize(_scale.x);//横幅半径取得
		float h = HalfSize(_scale.y);//立幅半径取得

		if (_reverse)
		{//反転描画
			vertex[0].Position = Vector3(x + w, y + h, _z);
			vertex[1].Position = Vector3(x - w, y + h, _z);
			vertex[2].Position = Vector3(x + w, y - h, _z);
			vertex[3].Position = Vector3(x - w, y - h, _z);
		}
		else
		{//通常描画
			vertex[0].Position = Vector3(x - w, y - h, _z);
			vertex[1].Position = Vector3(x + w, y - h, _z);
			vertex[2].Position = Vector3(x - w, y + h, _z);
			vertex[3].Position = Vector3(x + w, y + h, _z);
		}

		vertex[0].Normal = Vector3(0.0f, 0.0f, 0.0f);
		vertex[1].Normal = Vector3(0.0f, 0.0f, 0.0f);
		vertex[2].Normal = Vector3(0.0f, 0.0f, 0.0f);
		vertex[3].Normal = Vector3(0.0f, 0.0f, 0.0f);

		vertex[0].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);
		vertex[1].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);
		vertex[2].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);
		vertex[3].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);

		vertex[0].TexCoord = Vector2(0.0f, 0.0f);
		vertex[1].TexCoord = Vector2(1.0f, 0.0f);
		vertex[2].TexCoord = Vector2(0.0f, 1.0f);
		vertex[3].TexCoord = Vector2(1.0f, 1.0f);

		//書き換え完了
		m_Renderer_DX11->GetDeviceContext()->Unmap(m_CubePolygonVertexBuffer, 0);

		// マトリクス設定
		m_Renderer_DX11->SetWorldViewProjection2D();//2D描画

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		m_Renderer_DX11->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_CubePolygonVertexBuffer, &stride, &offset);

		// マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
		m_Renderer_DX11->SetMaterial(material);

		// プリミティブトポロジ設定
		m_Renderer_DX11->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// ポリゴン描画
		m_Renderer_DX11->GetDeviceContext()->Draw(VERTEX_NUM_PLANE, 0);
	}
}

//2Dスプライトアニメーション描画
void Renderer_Polygon::Draw2DAnimation(Vector2 _position, Vector2 _scale, Color _color,
	Vector2 _OneFrameSize, Vector2 _CutStartPostion,bool _reverse)
{
	if (m_Renderer_DX11 != nullptr)
	{
		//書き換え開始
		D3D11_MAPPED_SUBRESOURCE msr;
		m_Renderer_DX11->GetDeviceContext()->Map(m_CubePolygonVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		float x = _position.x + HalfSize(cacheMain->GetWindowSize().x); //X座標取得
		float y = _position.y + HalfSize(cacheMain->GetWindowSize().y);//Y座標取得

		float w = HalfSize(_scale.x);//横幅半径取得
		float h = HalfSize(_scale.y);//立幅半径取得

		if (_reverse)
		{//反転描画
			vertex[0].Position = Vector3(x + w, y + h, 0.0f);
			vertex[1].Position = Vector3(x - w, y + h, 0.0f);
			vertex[2].Position = Vector3(x + w, y - h, 0.0f);
			vertex[3].Position = Vector3(x - w, y - h, 0.0f);
		}
		else
		{//通常描画
			vertex[0].Position = Vector3(x - w, y - h, 0.0f);
			vertex[1].Position = Vector3(x + w, y - h, 0.0f);
			vertex[2].Position = Vector3(x - w, y + h, 0.0f);
			vertex[3].Position = Vector3(x + w, y + h, 0.0f);
		}

		vertex[0].Normal = Vector3(0.0f, 0.0f, 0.0f);
		vertex[1].Normal = Vector3(0.0f, 0.0f, 0.0f);
		vertex[2].Normal = Vector3(0.0f, 0.0f, 0.0f);
		vertex[3].Normal = Vector3(0.0f, 0.0f, 0.0f);

		vertex[0].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);
		vertex[1].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);
		vertex[2].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);
		vertex[3].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);

		vertex[0].TexCoord = Vector2(_CutStartPostion.x,                   _CutStartPostion.y);
		vertex[1].TexCoord = Vector2(_CutStartPostion.x + _OneFrameSize.x, _CutStartPostion.y);
		vertex[2].TexCoord = Vector2(_CutStartPostion.x,                   _CutStartPostion.y + _OneFrameSize.y);
		vertex[3].TexCoord = Vector2(_CutStartPostion.x + _OneFrameSize.x, _CutStartPostion.y + _OneFrameSize.y);

		//書き換え完了
		m_Renderer_DX11->GetDeviceContext()->Unmap(m_CubePolygonVertexBuffer, 0);

		// マトリクス設定
		m_Renderer_DX11->SetWorldViewProjection2D();//2D描画

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		m_Renderer_DX11->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_CubePolygonVertexBuffer, &stride, &offset);

		// マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_Renderer_DX11->SetMaterial(material);


		// プリミティブトポロジ設定
		m_Renderer_DX11->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// ポリゴン描画
		m_Renderer_DX11->GetDeviceContext()->Draw(VERTEX_NUM_PLANE, 0);
	}
}

void Renderer_Polygon::DrawLine2D(Vector2 _startPos, Vector2 _endPos, Color _color)
{
	if (m_Renderer_DX11 != nullptr)
	{
		//頂点データ書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			m_Renderer_DX11->GetDeviceContext()->Map(m_LineVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			VERTEX_3D* vertex3D = (VERTEX_3D*)msr.pData;

			//始点
			vertex3D[0].Position = Vector3(_startPos.x, _startPos.y, 0.0f);
			vertex3D[0].Diffuse = Vector4{ _color.r,_color.g,_color.b,_color.a };
			vertex3D[0].TexCoord = Vector2(0, 0);
			vertex3D[0].Normal = Vector3(0, 0, 0);

			//終点
			vertex3D[1].Position = Vector3(_endPos.x, _endPos.y, 0.0f);
			vertex3D[1].Diffuse = Vector4{ _color.r,_color.g,_color.b,_color.a };
			vertex3D[1].TexCoord = Vector2(1, 0);
			vertex3D[1].Normal = Vector3(0, 0, 0);

			m_Renderer_DX11->GetDeviceContext()->Unmap(m_LineVertexBuffer, 0);
		}

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		m_Renderer_DX11->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_LineVertexBuffer, &stride, &offset);

		//テクスチャ設定
		m_Renderer_DX11->GetDeviceContext()->PSSetShaderResources(0, 1, &m_TextureWhite);

		//シェーダーをアンリットに
		Manager_Shader::Instance()->SetShader(Manager_Shader::SHADER_TYPE::SHADER_UNLIT);

		// マトリクス設定
		m_Renderer_DX11->SetWorldViewProjection2D();

		// マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
		m_Renderer_DX11->SetMaterial(material);

		// プリミティブトポロジ設定
		m_Renderer_DX11->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		// ポリゴン描画
		m_Renderer_DX11->GetDeviceContext()->Draw(VERTEX_NUM_LINE, 0);
	}
}

void Renderer_Polygon::DrawBillboard(Vector3 _position, Vector3 _scale, 
	Color _color, Vector2 _OneFrameSize, Vector2 _CutStartPostion, bool _isUseDefaultTex)//ビルボード描画
{
	if (m_Renderer_DX11 != nullptr)
	{
		if (Camera* camera = Manager_Game::Instance()->GetLevel()->GetActor<Camera>())
		{//カメラがある場合

			
			if (!camera->CheckView(_position,_scale))
			{//視錐台カリング
				//return;
			}

		   //頂点データ書き換え
			D3D11_MAPPED_SUBRESOURCE msr;
			m_Renderer_DX11->GetDeviceContext()->Map(m_CubePolygonVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;


			SetCubePolygonVertexToDefault(vertex);//デフォルト値代入

			vertex[0].TexCoord = Vector2(_CutStartPostion.x,  _CutStartPostion.y);
			vertex[1].TexCoord = Vector2(_CutStartPostion.x + _OneFrameSize.x,     _CutStartPostion.y);
			vertex[2].TexCoord = Vector2(_CutStartPostion.x,  _CutStartPostion.y + _OneFrameSize.y);
			vertex[3].TexCoord = Vector2(_CutStartPostion.x + _OneFrameSize.x,     _CutStartPostion.y + _OneFrameSize.y);

			m_Renderer_DX11->GetDeviceContext()->Unmap(m_CubePolygonVertexBuffer, 0);


			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			m_Renderer_DX11->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_CubePolygonVertexBuffer, &stride, &offset);

			//カメラのビューマトリクスを取得
			D3DXMATRIX View = camera->GetViewMatrix();

			//ビューの逆行列
			D3DXMATRIX InvView;
			D3DXMatrixInverse(&InvView, NULL, &View);
			InvView._41 = 0.0f;
			InvView._42 = 0.0f;
			InvView._43 = 0.0f;

			// テクスチャ設定
			if (_isUseDefaultTex)m_Renderer_DX11->GetDeviceContext()->PSSetShaderResources(0, 1, &m_TextureWhite);

			// マトリクス設定
			D3DXMATRIX world,mscale, trans;
			D3DXMatrixScaling(&mscale, _scale.x, _scale.y, _scale.z);
			D3DXMatrixTranslation(&trans, _position.x, _position.y, _position.z);

			world = mscale * InvView * trans;
			m_Renderer_DX11->SetWorldMatrix(&world);

			// プリミティブトポロジ設定
			m_Renderer_DX11->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// ポリゴン描画
			m_Renderer_DX11->GetDeviceContext()->Draw(VERTEX_NUM_PLANE, 0);
		}
	}
}

void Renderer_Polygon::DrawLine(Vector3 _start, Vector3 _end, Color _color)
{
	if (m_Renderer_DX11 != nullptr)
	{
		//頂点データ書き換え
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			m_Renderer_DX11->GetDeviceContext()->Map(m_LineVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			VERTEX_3D* vertex3D = (VERTEX_3D*)msr.pData;

			//始点
			vertex3D[0].Position = _start;
			vertex3D[0].Diffuse = Exchanger::COLOR_TO_VEC4(_color);
			vertex3D[0].TexCoord = Vector2(0, 0);
			vertex3D[0].Normal = Vector3(0, 0, 0);

			//終点
			vertex3D[1].Position = _end;
			vertex3D[1].Diffuse = Exchanger::COLOR_TO_VEC4(_color);
			vertex3D[1].TexCoord = Vector2(1, 0);
			vertex3D[1].Normal = Vector3(0, 0, 0);
	
			m_Renderer_DX11->GetDeviceContext()->Unmap(m_LineVertexBuffer, 0);
		}

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		m_Renderer_DX11->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_LineVertexBuffer, &stride, &offset);

		//テクスチャ設定
		m_Renderer_DX11->GetDeviceContext()->PSSetShaderResources(0, 1, &m_TextureWhite);

		Manager_Shader::Instance()->SetShader(Manager_Shader::SHADER_TYPE::SHADER_UNLIT);

		// マトリクス設定
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		m_Renderer_DX11->SetWorldMatrix(&world);

		// プリミティブトポロジ設定
		m_Renderer_DX11->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		// ポリゴン描画
		m_Renderer_DX11->GetDeviceContext()->Draw(VERTEX_NUM_LINE, 0);
	}
}

void Renderer_Polygon::Draw3D(Vector3 _pos, Vector2 _scale, Vector3 _rot, Color _color, bool _isReverse)
{
	if (m_Renderer_DX11 != nullptr)
	{
		if (Camera* camera = Manager_Game::Instance()->GetLevel()->GetActor<Camera>())
		{//カメラがある場合

			if (!camera->CheckView(_pos, { _scale .x,_scale .y,0.0f}))
			{//視錐台カリング
				//return;
			}

			//書き換え開始
			D3D11_MAPPED_SUBRESOURCE msr;
			m_Renderer_DX11->GetDeviceContext()->Map(m_CubePolygonVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

			vertex[0].Position = Vector3(-0.5f,  0.5f, 0.0f);
			vertex[1].Position = Vector3( 0.5f,  0.5f, 0.0f);
			vertex[2].Position = Vector3(-0.5f, -0.5f, 0.0f);
			vertex[3].Position = Vector3( 0.5f, -0.5f, 0.0f);

			vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
			vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
			vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
			vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);

			vertex[0].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);
			vertex[1].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);
			vertex[2].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);
			vertex[3].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);

			vertex[0].TexCoord = Vector2(0.0f, 0.0f);
			vertex[1].TexCoord = Vector2(1.0f, 0.0f);
			vertex[2].TexCoord = Vector2(0.0f, 1.0f);
			vertex[3].TexCoord = Vector2(1.0f, 1.0f);

			//書き換え完了
			m_Renderer_DX11->GetDeviceContext()->Unmap(m_CubePolygonVertexBuffer, 0);

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			m_Renderer_DX11->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_CubePolygonVertexBuffer, &stride, &offset);

			// マトリクス設定
			D3DXMATRIX world, mscale, trans, mtxR;
			D3DXMatrixScaling(&mscale, _scale.x, _scale.y, 0.0f);
			D3DXMatrixTranslation(&trans, _pos.x, _pos.y, _pos.z);
			world = mscale * trans;
			m_Renderer_DX11->SetWorldMatrix(&world);

			// マテリアル設定
			MATERIAL material;
			ZeroMemory(&material, sizeof(material));
			material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
			m_Renderer_DX11->SetMaterial(material);

			// プリミティブトポロジ設定
			m_Renderer_DX11->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// ポリゴン描画
			m_Renderer_DX11->GetDeviceContext()->Draw(VERTEX_NUM_PLANE, 0);
		}
	}
}

void Renderer_Polygon::Draw3DAnimation(Vector3 _pos, Vector2 _scale,
	Color _color, Vector2 _OneFrameSize, Vector2 _CutStartPostion,
	bool _isUseDefaultTex,bool _isReverse)
{
	if (m_Renderer_DX11 != nullptr)
	{
		if (Camera* camera = Manager_Game::Instance()->GetLevel()->GetActor<Camera>())
		{//カメラがある場合

			//視錐台カリング
			if (!camera->CheckView(_pos, { _scale.x,_scale.y,0.0f }))return;

			//頂点データ書き換え
			D3D11_MAPPED_SUBRESOURCE msr;
			m_Renderer_DX11->GetDeviceContext()->Map(m_CubePolygonVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

			SetCubePolygonVertexToDefault(vertex);//デフォルト値代入

			vertex[0].TexCoord = Vector2(_CutStartPostion.x, _CutStartPostion.y);
			vertex[1].TexCoord = Vector2(_CutStartPostion.x + _OneFrameSize.x, _CutStartPostion.y);
			vertex[2].TexCoord = Vector2(_CutStartPostion.x, _CutStartPostion.y + _OneFrameSize.y);
			vertex[3].TexCoord = Vector2(_CutStartPostion.x + _OneFrameSize.x, _CutStartPostion.y + _OneFrameSize.y);

			vertex[0].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);
			vertex[1].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);
			vertex[2].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);
			vertex[3].Diffuse = Vector4(_color.r, _color.g, _color.b, _color.a);

			vertex[0].TexCoord = Vector2(_CutStartPostion.x, _CutStartPostion.y);
			vertex[1].TexCoord = Vector2(_CutStartPostion.x + _OneFrameSize.x, _CutStartPostion.y);
			vertex[2].TexCoord = Vector2(_CutStartPostion.x, _CutStartPostion.y + _OneFrameSize.y);
			vertex[3].TexCoord = Vector2(_CutStartPostion.x + _OneFrameSize.x, _CutStartPostion.y + _OneFrameSize.y);

			m_Renderer_DX11->GetDeviceContext()->Unmap(m_CubePolygonVertexBuffer, 0);

			// テクスチャ設定
			if (_isUseDefaultTex)m_Renderer_DX11->GetDeviceContext()->PSSetShaderResources(0, 1, &m_TextureWhite);

			// 頂点バッファ設定
			UINT stride = sizeof(VERTEX_3D);
			UINT offset = 0;
			m_Renderer_DX11->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_CubePolygonVertexBuffer, &stride, &offset);

			// マトリクス設定
			D3DXMATRIX world, mscale, trans;
			D3DXMatrixScaling(&mscale, _scale.x, _scale.y, 0.0f);
			D3DXMatrixTranslation(&trans, _pos.x, _pos.y, _pos.z);

			world = mscale * trans;
			m_Renderer_DX11->SetWorldMatrix(&world);

			// プリミティブトポロジ設定
			m_Renderer_DX11->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// ポリゴン描画
			Renderer_DX11::Instance()->GetDeviceContext()->Draw(VERTEX_NUM_PLANE, 0);
		}
	}
}

void Renderer_Polygon::SetCubePolygonVertexToDefault(VERTEX_3D* _vertex)
{
	_vertex[0].Position = Vector3(-0.5f,  0.5f, 0);
	_vertex[1].Position = Vector3( 0.5f,  0.5f, 0);
	_vertex[2].Position = Vector3(-0.5f, -0.5f, 0);
	_vertex[3].Position = Vector3( 0.5f, -0.5f, 0);

	_vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
	_vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
	_vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
	_vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);

	_vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	_vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	_vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	_vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	_vertex[0].TexCoord = Vector2(0.0f, 0.0f);
	_vertex[1].TexCoord = Vector2(1.0f, 0.0f);
	_vertex[2].TexCoord = Vector2(0.0f, 1.0f);
	_vertex[3].TexCoord = Vector2(1.0f, 1.0f);
}

void Renderer_Polygon::Set3DBoxVertexToDefault(VERTEX_3D* _vertex,Vector3 _magni)
{
	//list
	{
		float tempTexX[6] = { 0,1,0,1,1,0 };
		float tempTexY[6] = { 0,0,1,0,1,1 };


		for (int i = 0; i < VERTEX_NUM_BOX; i++)
		{
			_vertex[i].Diffuse = Vector4(1, 1, 1, 1);

			//面によってXとYにする軸が違う
			Vector2 tempFace = i / 6 == 0 ? Vector2(_magni.x, _magni.y) : //手前
				i / 6 == 1 ? Vector2(_magni.z, _magni.y) : //右
				i / 6 == 2 ? Vector2(_magni.x, _magni.y) : //奥
				i / 6 == 3 ? Vector2(_magni.z, _magni.y) : //左
				i / 6 == 4 ? Vector2(_magni.x,_magni.z ) /*上*/ : Vector2(_magni.x, _magni.z);//下
			_vertex[i].TexCoord = Vector2(tempTexX[i % 6] * tempFace.x, tempTexY[i % 6] * tempFace.y);
		}

		//手前
		_vertex[0].Position = Vector3(-0.5f,  0.5f, -0.5f);
		_vertex[1].Position = Vector3( 0.5f,  0.5f, -0.5f);
		_vertex[2].Position = Vector3(-0.5f, -0.5f, -0.5f);

		_vertex[3].Position = Vector3( 0.5f,  0.5f, -0.5f);
		_vertex[4].Position = Vector3( 0.5f, -0.5f, -0.5f);
		_vertex[5].Position = Vector3(-0.5f, -0.5f, -0.5f);

		//右
		_vertex[6].Position = Vector3(0.5f,  0.5f, -0.5f);
		_vertex[7].Position = Vector3(0.5f,  0.5f,  0.5f);
		_vertex[8].Position = Vector3(0.5f, -0.5f, -0.5f);

		_vertex[ 9].Position = Vector3(0.5f,  0.5f,  0.5f);
		_vertex[10].Position = Vector3(0.5f, -0.5f,  0.5f);
		_vertex[11].Position = Vector3(0.5f, -0.5f, -0.5f);

		//奥
		_vertex[12].Position = Vector3( 0.5f,  0.5f, 0.5f);
		_vertex[13].Position = Vector3(-0.5f,  0.5f, 0.5f);
		_vertex[14].Position = Vector3( 0.5f, -0.5f, 0.5f);

		_vertex[15].Position = Vector3(-0.5f,  0.5f, 0.5f);
		_vertex[16].Position = Vector3(-0.5f, -0.5f, 0.5f);
		_vertex[17].Position = Vector3( 0.5f, -0.5f, 0.5f);

		//左
		_vertex[18].Position = Vector3(-0.5f,  0.5f,  0.5f);
		_vertex[19].Position = Vector3(-0.5f,  0.5f, -0.5f);
		_vertex[20].Position = Vector3(-0.5f, -0.5f,  0.5f);

		_vertex[21].Position = Vector3(-0.5f,  0.5f, -0.5f);
		_vertex[22].Position = Vector3(-0.5f, -0.5f, -0.5f);
		_vertex[23].Position = Vector3(-0.5f, -0.5f,  0.5f);

		//上
		_vertex[24].Position = Vector3(-0.5f,  0.5f,  0.5f);
		_vertex[25].Position = Vector3( 0.5f,  0.5f,  0.5f);
		_vertex[26].Position = Vector3(-0.5f,  0.5f, -0.5f);

		_vertex[27].Position = Vector3( 0.5f,  0.5f,  0.5f);
		_vertex[28].Position = Vector3( 0.5f,  0.5f, -0.5f);
		_vertex[29].Position = Vector3(-0.5f,  0.5f, -0.5f);

		//下
		_vertex[30].Position = Vector3(-0.5f, -0.5f, -0.5f);
		_vertex[31].Position = Vector3( 0.5f, -0.5f, -0.5f);
		_vertex[32].Position = Vector3(-0.5f, -0.5f,  0.5f);

		_vertex[33].Position = Vector3( 0.5f, -0.5f, -0.5f);
		_vertex[34].Position = Vector3( 0.5f, -0.5f,  0.5f);
		_vertex[35].Position = Vector3(-0.5f, -0.5f,  0.5f);
	}
}