//=======================================================
//
// �o�[�e�b�N�X���C�e�B���O
//
//=======================================================
#include "../Common/Common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
	// ���_�ϊ�
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	
	// �@���̌v�Z
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);

	// ���C�g�̌v�Z(�����o�[�g)
	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);

	// ���_�F���v�Z
	Out.Diffuse = In.Diffuse * Material.Diffuse * light * Light.Diffuse;
	Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
	Out.Diffuse += Material.Emission;
	Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;

	// ���_�|�W�V�������擾
	Out.Position = mul( In.Position, wvp );

	// ����TexCoord�����̂܂ܑ��
	Out.TexCoord = In.TexCoord;
}

