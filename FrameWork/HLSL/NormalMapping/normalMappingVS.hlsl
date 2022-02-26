//============================================
//
//法線マッピング
//
//
//============================================
#include "../Common/Common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
	// 頂点変換
	matrix  wvp;               
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	Out.Position = mul(In.Position, wvp);

	// 光源計算
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.WorldPosition = mul(In.Position, World);
	Out.Normal = worldNormal;	
	Out.Diffuse = In.Diffuse;

	// テクスチャ座標代入	
	Out.TexCoord = In.TexCoord;            	
}