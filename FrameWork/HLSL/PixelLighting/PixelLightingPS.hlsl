//================================================
//
//ピクセル光源
//
//================================================
#include "../Common/Common.hlsl"


// DirectXのテクスチャ設定を受け継ぐ
Texture2D g_Texture : register(t0);  // テクスチャ０番

// DirectXのサンプラーステートの設定を受け継ぐ
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	// 法線を取得
	float4 normal = normalize(In.Normal);

	// ライトの計算(ハーフランバート)
	float light = 0.5f - 0.5f * dot(normal.xyz, Light.Direction.xyz);


	// このピクセルに使われるテクスチャの色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

	outDiffuse.rgb *= (light * In.Diffuse.rgb);
	outDiffuse.a *= In.Diffuse.a;

	// スペキュラー(フォン)
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz; 
	eyev = normalize(eyev);                                  

	float3 refv = reflect(Light.Direction.xyz, normal.xyz);  
	refv = normalize(refv);

	// 反射ベクトルから反射光を取得
	float specular = -dot(eyev, refv);                       
	specular = saturate(specular);                           
	specular = pow(specular, 30);                            

	// スペキュラー値を加算
	outDiffuse.rgb += specular;                              
}