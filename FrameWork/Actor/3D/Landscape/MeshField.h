//=============================================================================
//
//  地面描画 [MeshField.h]
//  Date   : 2021/11/20
//
//=============================================================================
#pragma once

#include "../Base/Actor3D.h"
#include "../../../Renderer/Renderer_DX11.h"
#include "../../../System/Main.h"
//******************************************************************************
// クラス定義
//******************************************************************************
class MeshField : public Actor3D
{
private:

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;

	ID3D11ShaderResourceView* m_Texture = NULL;


	VERTEX_3D  m_Vertex[21][21];

	int   m_ShaderType;//使用するシェーダーの種類


public:

	static void Load();//リソース読み込み
	static void Unload();//リソース削除

	void Init();//初期化
	void Uninit();//終了
	void Update();//更新
	void Draw();//描画

	float GetHeight(Vector3 _position);//高さ取得
};
