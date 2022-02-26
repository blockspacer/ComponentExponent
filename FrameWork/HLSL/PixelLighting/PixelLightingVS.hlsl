//================================================
//
//ピクセル光源
//
//================================================
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
	Out.Normal = worldNormal;

	// 受け取った頂点のディフューズをそのまま出力
	Out.Diffuse = In.Diffuse;

	// テクスチャ座標を取得
	Out.TexCoord = In.TexCoord;            

	// ワールド変換した頂点座標を出力
	Out.WorldPosition = mul(In.Position, World);
}