//=============================================================================
//
//  シェーダーマネージャー [Manager_Shader.h]
//  Date   : 2021/11/05
//
//=============================================================================
#pragma once

#include <unordered_map>
#include"../System/Singleton.h"
#include "../System/Main.h"


//******************************************************************************
// クラス定義
//******************************************************************************
class Manager_Shader : public Singleton<Manager_Shader>
{
private:

	friend class Singleton<Manager_Shader>;//シングルトン

	struct s_Shader
	{
		ID3D11InputLayout*    VertexLayout;//IL
		ID3D11VertexShader*   VertexShader;//VS
		ID3D11PixelShader*    PixelShader; //PS
	};//シェーダー情報

	std::unordered_map < int, s_Shader > m_Shaders;//シェーダー

public:

	void Init(void);//初期化処理
	void Uninit(void);//終了処理

	void SetShader(int _key);//シェーダー設定

	enum SHADER_TYPE//シェーダータイプ
	{
		SHADER_UNLIT,
		SHADER_VERTEX_LIGHTING,
		SHADER_PIXEL_LIGHTING,
		SHADER_ATOMOSPHERE,
		SHADER_TOON,
		SHADER_NORMALMAP,
		SHADER_ENVMAP
	};
};
