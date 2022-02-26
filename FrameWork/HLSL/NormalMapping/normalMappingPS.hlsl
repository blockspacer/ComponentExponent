//============================================
//
//�@���}�b�s���O
//
//
//============================================
//�g���ꍇ�̓e�N�X�`����t1�ɖ@���}�b�v�e�N�X�`�����Z�b�g����
#include "../Common/Common.hlsl"


// �e�N�X�`���ϐ��̐錾
Texture2D g_Texture:register(t0);// ���f���̃e�N�X�`���@�e�N�X�`��0��
Texture2D g_NormalTexture:register(t1);// �@���e�N�X�`���@�e�N�X�`��1��

// �T���v���[�̐錾
SamplerState g_SamplerState : register(s0);

// ���@���}�b�v�̃f�[�^��outDiffuse�ɓ���Ȃ�
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	// �@���̃e�N�X�`������F�����擾
	float4 normalMap = g_NormalTexture.Sample(g_SamplerState, In.TexCoord);
	//�擾����RGBA�l���x�N�g�����XYZW�Ƃ��ēW�J����
	normalMap = (normalMap * 2.0f) - 1.0f;

	float4 normal;
	normal.x = -normalMap.x;
	normal.y = normalMap.z;
	normal.z = normalMap.y;
	normal.w = 0.0;
	normal = normalize(normal); //���K��

	float light = -dot(normal.xyz, Light.Direction.xyz);

	// ���̃s�N�Z���Ɏg����e�N�X�`���̐F���擾
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

	// �s�N�Z���̐F���v�Z
	outDiffuse.rgb *= (light * In.Diffuse.rgb);
	outDiffuse.a *= In.Diffuse.a;


	// �����v�Z���s��
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev);                                 

	float3 refv = reflect(Light.Direction.xyz, normal.xyz);  // ���̔��˃x�N�g��
	refv = normalize(refv);

	// ���˃x�N�g�����甽�ˌ����擾
	float specular = -dot(eyev, refv);                       
	specular = saturate(specular);                           
	specular = pow(specular, 30);

	// �X�y�L�����[�l�����Z
	outDiffuse.rgb += specular;                              
}