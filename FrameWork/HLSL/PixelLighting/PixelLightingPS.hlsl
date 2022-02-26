//================================================
//
//�s�N�Z������
//
//================================================
#include "../Common/Common.hlsl"


// DirectX�̃e�N�X�`���ݒ���󂯌p��
Texture2D g_Texture : register(t0);  // �e�N�X�`���O��

// DirectX�̃T���v���[�X�e�[�g�̐ݒ���󂯌p��
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	// �@�����擾
	float4 normal = normalize(In.Normal);

	// ���C�g�̌v�Z(�n�[�t�����o�[�g)
	float light = 0.5f - 0.5f * dot(normal.xyz, Light.Direction.xyz);


	// ���̃s�N�Z���Ɏg����e�N�X�`���̐F���擾
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

	outDiffuse.rgb *= (light * In.Diffuse.rgb);
	outDiffuse.a *= In.Diffuse.a;

	// �X�y�L�����[(�t�H��)
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz; 
	eyev = normalize(eyev);                                  

	float3 refv = reflect(Light.Direction.xyz, normal.xyz);  
	refv = normalize(refv);

	// ���˃x�N�g�����甽�ˌ����擾
	float specular = -dot(eyev, refv);                       
	specular = saturate(specular);                           
	specular = pow(specular, 30);                            

	// �X�y�L�����[�l�����Z
	outDiffuse.rgb += specular;                              
}