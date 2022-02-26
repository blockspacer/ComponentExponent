//=======================================================
//
// バーテックスライティング
//
//=======================================================
#include "../Common/Common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
	// 頂点変換
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	
	// 法線の計算
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);

	// ライトの計算(ランバート)
	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);

	// 頂点色を計算
	Out.Diffuse = In.Diffuse * Material.Diffuse * light * Light.Diffuse;
	Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
	Out.Diffuse += Material.Emission;
	Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;

	// 頂点ポジションを取得
	Out.Position = mul( In.Position, wvp );

	// 入力TexCoordをそのまま代入
	Out.TexCoord = In.TexCoord;
}

