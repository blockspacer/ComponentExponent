//=============================================================================
//
//  DX11”ÅRendrer [Renderer.h]
//  Date   : 2021/11/02
//
//=============================================================================

#pragma once

#include "../System/Singleton.h"


struct VERTEX_3D
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXVECTOR4 Diffuse;
    D3DXVECTOR2 TexCoord;
};



struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];
};



struct LIGHT
{
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;
	BOOL		Enable;
	BOOL		Dummy[3];
};


struct CAMERA
{
	D3DXVECTOR4 Position;
	float Length;
	D3DXVECTOR3 dummy;
};


class Renderer_DX11 : public Singleton<Renderer_DX11>
{
private:
	friend class Singleton<Renderer_DX11>;//ƒVƒ“ƒOƒ‹ƒgƒ“

	D3D_FEATURE_LEVEL       m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	ID3D11Device*           m_Device;
	ID3D11DeviceContext*    m_DeviceContext;
	IDXGISwapChain*         m_SwapChain;
	ID3D11RenderTargetView* m_RenderTargetView;
	ID3D11DepthStencilView* m_DepthStencilView;

	ID3D11Buffer*			m_WorldBuffer;
	ID3D11Buffer*			m_ViewBuffer;
	ID3D11Buffer*			m_ProjectionBuffer;
	ID3D11Buffer*			m_MaterialBuffer;
	ID3D11Buffer*			m_LightBuffer;
	ID3D11Buffer*           m_CameraBuffer;

	class MainWindow* cacheMain;
	ID3D11DepthStencilState* m_DepthStateEnable;
	ID3D11DepthStencilState* m_DepthStateDisable;

	Vector4 m_ClearColor;

public:
	void Init();
	void Uninit();
	void Begin();
	void End();

	void SetDepthEnable(bool Enable);
	void SetWorldViewProjection2D();
	void SetWorldMatrix(const D3DXMATRIX* WorldMatrix);
	void SetViewMatrix(D3DXMATRIX* ViewMatrix);
	void SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix);
	void SetMaterial(MATERIAL Material);
	void SetLight(LIGHT Light);
	void SetCamera(CAMERA Camera);

	ID3D11Device* GetDevice( void ){ return m_Device; }
	ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }
	IDXGISwapChain* GetSwapChain(void) { return m_SwapChain; }

	void SetRasterizerState(D3D11_FILL_MODE _set);

	void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

	inline ID3D11Buffer** GetConstantWorldBuf() { return &m_WorldBuffer; }

	inline void SetClearColor(Vector4 _color) { m_ClearColor = _color; }
};
