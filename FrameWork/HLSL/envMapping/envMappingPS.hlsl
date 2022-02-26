//=======================================================
//
// 環境マッピング
//
//=======================================================
#include "../Common/Common.hlsl"

// DirectXのテクスチャ設定を受け継ぐ
Texture2D g_Texture : register(t0);  // テクスチャ０番
Texture2D g_EnvTexture:register(t1); // テクスチャ１番

// DirectXのサンプラーステートの設定を受け継ぐ
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	// 法線を取得
	float4 normal = normalize(In.Normal);

	// このピクセルに使われるテクスチャの色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.a *= In.Diffuse.a;

	// ライトの計算(ハーフランバート)
	float light = 0.5f - 0.5f * dot(normal.xyz, Light.Direction.xyz);

	// 反射ベクトルの計算
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev);                                 

	float3 refv = reflect(Light.Direction.xyz, normal.xyz); 
	refv = normalize(refv);

	// 反射ベクトルからTexCoordを取得
	float2 envTexCoord;
    envTexCoord.x = -refv.x * 0.3 + 0.5;
    envTexCoord.y = -refv.y * 0.3 + 0.5;

	// 環境マップを取得
	float4 EnvTex = g_EnvTexture.SampleBias(g_SamplerState, envTexCoord, 0.0f);
	outDiffuse.rgb += EnvTex.rgb;

	// 反射ベクトルから反射光を計算
	float specular = -dot(eyev, refv);              
	specular = saturate(specular);                  
	specular = pow(specular, 30);                   

	// スペキュラー値を加算
	outDiffuse.rgb += specular;
}