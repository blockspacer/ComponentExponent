//=======================================================
//
// �o�[�e�b�N�X���C�e�B���O
//
//=======================================================
#include "../Common/Common.hlsl"

// DirectX�̃e�N�X�`���ݒ���󂯌p��
Texture2D g_Texture : register(t0);

// DirectX�̃T���v���[�ݒ���󂯌p��
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	// �e�N�X�`���̐F���擾
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

	outDiffuse *= In.Diffuse;
}

