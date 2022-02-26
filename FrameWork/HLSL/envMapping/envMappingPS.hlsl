//=======================================================
//
// ���}�b�s���O
//
//=======================================================
#include "../Common/Common.hlsl"

// DirectX�̃e�N�X�`���ݒ���󂯌p��
Texture2D g_Texture : register(t0);  // �e�N�X�`���O��
Texture2D g_EnvTexture:register(t1); // �e�N�X�`���P��

// DirectX�̃T���v���[�X�e�[�g�̐ݒ���󂯌p��
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	// �@�����擾
	float4 normal = normalize(In.Normal);

	// ���̃s�N�Z���Ɏg����e�N�X�`���̐F���擾
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.a *= In.Diffuse.a;

	// ���C�g�̌v�Z(�n�[�t�����o�[�g)
	float light = 0.5f - 0.5f * dot(normal.xyz, Light.Direction.xyz);

	// ���˃x�N�g���̌v�Z
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev);                                 

	float3 refv = reflect(Light.Direction.xyz, normal.xyz); 
	refv = normalize(refv);

	// ���˃x�N�g������TexCoord���擾
	float2 envTexCoord;
    envTexCoord.x = -refv.x * 0.3 + 0.5;
    envTexCoord.y = -refv.y * 0.3 + 0.5;

	// ���}�b�v���擾
	float4 EnvTex = g_EnvTexture.SampleBias(g_SamplerState, envTexCoord, 0.0f);
	outDiffuse.rgb += EnvTex.rgb;

	// ���˃x�N�g�����甽�ˌ����v�Z
	float specular = -dot(eyev, refv);              
	specular = saturate(specular);                  
	specular = pow(specular, 30);                   

	// �X�y�L�����[�l�����Z
	outDiffuse.rgb += specular;
}