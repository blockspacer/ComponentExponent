//============================================
//
//�@���}�b�s���O
//
//
//============================================
#include "../Common/Common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
	// ���_�ϊ�
	matrix  wvp;               
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	Out.Position = mul(In.Position, wvp);

	// �����v�Z
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.WorldPosition = mul(In.Position, World);
	Out.Normal = worldNormal;	
	Out.Diffuse = In.Diffuse;

	// �e�N�X�`�����W���	
	Out.TexCoord = In.TexCoord;            	
}