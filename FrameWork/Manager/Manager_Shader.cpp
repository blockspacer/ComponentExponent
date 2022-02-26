//=============================================================================
//
//  �V�F�[�_�[�}�l�[�W���[ [Manager_Shader.h]
//  Date   : 2021/11/05
//
//=============================================================================

//******************************************************************************
// �C���N���[�h
//******************************************************************************

#include "Manager_Shader.h"
#include "../Renderer/Renderer_DX11.h"

void Manager_Shader::Init()//����������
{
	//SHADER_UNLIT������
	Renderer_DX11::Instance()->CreateVertexShader(&m_Shaders[SHADER_UNLIT].VertexShader, &m_Shaders[SHADER_UNLIT].VertexLayout, "ROM/cso/unlitTextureVS.cso");
	Renderer_DX11::Instance()->CreatePixelShader (&m_Shaders[SHADER_UNLIT].PixelShader, "ROM/cso/unlitTexturePS.cso");

	//SHADER_VERTEX_LIGHTING������
	Renderer_DX11::Instance()->CreateVertexShader(&m_Shaders[SHADER_VERTEX_LIGHTING].VertexShader, &m_Shaders[SHADER_VERTEX_LIGHTING].VertexLayout, "ROM/cso/VertexLightingVS.cso");
	Renderer_DX11::Instance()->CreatePixelShader (&m_Shaders[SHADER_VERTEX_LIGHTING].PixelShader, "ROM/cso/VertexLightingPS.cso");
	
	//SHADER_PIXEL_LIGHTING������
	Renderer_DX11::Instance()->CreateVertexShader(&m_Shaders[SHADER_PIXEL_LIGHTING].VertexShader, &m_Shaders[SHADER_PIXEL_LIGHTING].VertexLayout, "ROM/cso/PixelLightingVS.cso");
	Renderer_DX11::Instance()->CreatePixelShader (&m_Shaders[SHADER_PIXEL_LIGHTING].PixelShader, "ROM/cso/PixelLightingPS.cso");

	//SHADER_ATOMOSPHERE������
	Renderer_DX11::Instance()->CreateVertexShader(&m_Shaders[SHADER_ATOMOSPHERE].VertexShader, &m_Shaders[SHADER_ATOMOSPHERE].VertexLayout, "ROM/cso/Atomosphere_VS.cso");
	Renderer_DX11::Instance()->CreatePixelShader (&m_Shaders[SHADER_ATOMOSPHERE].PixelShader, "ROM/cso/Atomosphere_PS.cso");

	//SHADER_TOON������
	Renderer_DX11::Instance()->CreateVertexShader(&m_Shaders[SHADER_TOON].VertexShader, &m_Shaders[SHADER_TOON].VertexLayout, "ROM/cso/ToonVS.cso");
	Renderer_DX11::Instance()->CreatePixelShader (&m_Shaders[SHADER_TOON].PixelShader, "ROM/cso/ToonPS.cso");

	//SHADER_NORMALMAP������
	Renderer_DX11::Instance()->CreateVertexShader(&m_Shaders[SHADER_NORMALMAP].VertexShader, &m_Shaders[SHADER_NORMALMAP].VertexLayout, "ROM/cso/NormalMappingVS.cso");
	Renderer_DX11::Instance()->CreatePixelShader (&m_Shaders[SHADER_NORMALMAP].PixelShader, "ROM/cso/NormalMappingPS.cso");

	//SHADER_ENVMAP������
	Renderer_DX11::Instance()->CreateVertexShader(&m_Shaders[SHADER_ENVMAP].VertexShader, &m_Shaders[SHADER_ENVMAP].VertexLayout, "ROM/cso/EnvMappingVS.cso");
	Renderer_DX11::Instance()->CreatePixelShader (&m_Shaders[SHADER_ENVMAP].PixelShader, "ROM/cso/EnvMappingPS.cso");
}

void Manager_Shader::Uninit()//�I������
{
	//�C�e���[�^�[
	std::unordered_map < int, s_Shader >::iterator itr = m_Shaders.begin();

	for (itr; itr != m_Shaders.end(); itr++)
	{//�S�v�f�����[�v
		if (ID3D11PixelShader* pixelShader = itr->second.PixelShader)//PS���
		{
			pixelShader->Release();//PS���
		}
		if (ID3D11VertexShader* vertexShader = itr->second.VertexShader)
		{
			vertexShader->Release();//VS���
		}
		if (ID3D11InputLayout* vertexLayout = itr->second.VertexLayout)
		{
			vertexLayout->Release();//IL���
		}
	}

	m_Shaders.clear();
}

//�V�F�[�_�[�ݒ�
void Manager_Shader::SetShader(int _key)
{
	// ���̓��C�A�E�g�ݒ�
	Renderer_DX11::Instance()->GetDeviceContext()->IASetInputLayout(m_Shaders[_key].VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer_DX11::Instance()->GetDeviceContext()->VSSetShader(m_Shaders[_key].VertexShader, NULL, 0);
	Renderer_DX11::Instance()->GetDeviceContext()->PSSetShader(m_Shaders[_key].PixelShader , NULL, 0);
}



