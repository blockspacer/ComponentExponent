//=======================================================
//
// �g�D�[��
//
//=======================================================
#include "../Common/Common.hlsl"


// DirectX�̃e�N�X�`���ݒ���󂯌p��
Texture2D g_Texture : register(t0);  // �e�N�X�`���O��
Texture2D g_ToonTexture:register(t1);

// DirectX�̃T���v���[�X�e�[�g�̐ݒ���󂯌p��
SamplerState g_SamplerState : register(s0);

void Toon(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	// �@�����擾
	float4 normal = normalize(In.Normal);

	// ���̃s�N�Z���Ɏg����e�N�X�`���̐F���擾
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.a *= In.Diffuse.a;

	// ���C�g�̌v�Z(�n�[�t�����o�[�g)
	float light = 0.5f - 0.5f * dot(normal.xyz, Light.Direction.xyz);

	// �g�D�[���̌v�Z
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

	// ���_�F���擾
	outDiffuse.rgb *= saturate(In.Diffuse.rgb * light);

	// �֊s(�G�b�W)��t����
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); 

	//�@���Ǝ����x�N�g���̓���
	float d = dot(eyev, normal.xyz);
	if (d > -0.3)
	{
		outDiffuse.rgb *= 0.2; //�K����臒l�Ŗ��x�𗎂Ƃ�
	}
}