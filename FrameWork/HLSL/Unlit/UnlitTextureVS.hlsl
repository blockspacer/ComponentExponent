//=======================================================
//
// 光源なし
//
//=======================================================
#include "../Common/Common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
	// 頂点変換
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	// ステータスを代入
	Out.Position = mul(In.Position, wvp);
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse;
}

