//=======================================================
//
// トゥーン
//
//=======================================================
#include "../Common/Common.hlsl"


// DirectXのテクスチャ設定を受け継ぐ
Texture2D g_Texture : register(t0);  // テクスチャ０番
Texture2D g_ToonTexture:register(t1);

// DirectXのサンプラーステートの設定を受け継ぐ
SamplerState g_SamplerState : register(s0);

void Toon(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	// 法線を取得
	float4 normal = normalize(In.Normal);

	// このピクセルに使われるテクスチャの色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.a *= In.Diffuse.a;

	// ライトの計算(ハーフランバート)
	float light = 0.5f - 0.5f * dot(normal.xyz, Light.Direction.xyz);

	// トゥーンの計算
	if (light > 0.6)
	{
		light = 1.0;
	}
	else if (light > 0.4)
	{
		light = 0.8;
	}
	else
	{
		light = 0.6;
	}

	// 頂点色を取得
	outDiffuse.rgb *= saturate(In.Diffuse.rgb * light);

	// 輪郭(エッジ)を付ける
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); 

	//法線と視線ベクトルの内積
	float d = dot(eyev, normal.xyz);
	if (d > -0.3)
	{
		outDiffuse.rgb *= 0.2; //適当な閾値で明度を落とす
	}
}