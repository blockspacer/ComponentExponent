//=======================================================
//
// �����Ȃ�
//
//=======================================================
#include "../Common/Common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
	// ���_�ϊ�
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	// �X�e�[�^�X����
	Out.Position = mul(In.Position, wvp);
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse;
}

