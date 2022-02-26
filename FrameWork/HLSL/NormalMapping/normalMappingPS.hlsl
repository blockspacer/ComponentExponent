//============================================
//
//法線マッピング
//
//
//============================================
//使う場合はテクスチャのt1に法線マップテクスチャをセットする
#include "../Common/Common.hlsl"


// テクスチャ変数の宣言
Texture2D g_Texture:register(t0);// モデルのテクスチャ　テクスチャ0番
Texture2D g_NormalTexture:register(t1);// 法線テクスチャ　テクスチャ1番

// サンプラーの宣言
SamplerState g_SamplerState : register(s0);

// ※法線マップのデータはoutDiffuseに入れない
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	// 法線のテクスチャから色情報を取得
	float4 normalMap = g_NormalTexture.Sample(g_SamplerState, In.TexCoord);
	//取得したRGBA値をベクトル情報XYZWとして展開する
	normalMap = (normalMap * 2.0f) - 1.0f;

	float4 normal;
	normal.x = -normalMap.x;
	normal.y = normalMap.z;
	normal.z = normalMap.y;
	normal.w = 0.0;
	normal = normalize(normal); //正規化

	float light = -dot(normal.xyz, Light.Direction.xyz);

	// このピクセルに使われるテクスチャの色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

	// ピクセルの色を計算
	outDiffuse.rgb *= (light * In.Diffuse.rgb);
	outDiffuse.a *= In.Diffuse.a;


	// 光源計算を行う
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev);                                 

	float3 refv = reflect(Light.Direction.xyz, normal.xyz);  // 光の反射ベクトル
	refv = normalize(refv);

	// 反射ベクトルから反射光を取得
	float specular = -dot(eyev, refv);                       
	specular = saturate(specular);                           
	specular = pow(specular, 30);

	// スペキュラー値を加算
	outDiffuse.rgb += specular;                              
}