//=======================================================
//
// バーテックスライティング
//
//=======================================================
#include "../Common/Common.hlsl"

// DirectXのテクスチャ設定を受け継ぐ
Texture2D g_Texture : register(t0);

// DirectXのサンプラー設定を受け継ぐ
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	// テクスチャの色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

	outDiffuse *= In.Diffuse;
}

