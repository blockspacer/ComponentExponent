//=============================================================================
//
//  [Common.hlsl]
//  Date   : 2021/12/08
//
//=============================================================================


cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}




struct MATERIAL
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
	float3 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}




struct LIGHT
{
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
	bool Enable;
	bool3 Dummy;
};

cbuffer LightBuffer : register(b4)
{
	LIGHT Light;
}


cbuffer CameraBuffer : register(b5)
{
	float4 CameraPosition;
	float CameraLength;
	float3 Dummy;
}


struct VS_IN
{
	float4 Position		: POSITION0;
	float4 Normal		: NORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
};


struct PS_IN
{
	float4 Position		 : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Normal        : NORMAL0;
	float4 Diffuse		 : COLOR0;
	float2 TexCoord		 : TEXCOORD0;
};

struct SkyRay
{
	float3 frustumRayTL;// スクリーンの左上に向かうカメラのレイ
	float3 frustumRayTR;// スクリーンの右上に向かうカメラのレイ
	float3 frustumRayBL;// スクリーンの左下に向かうカメラのレイ
	uint2 windowSize;   // ウィンドウサイズ
	//int a;
	bool Enable;
	//vector<bool, 3>	Dummy;
};

// 大気散乱用バッファ構造体
cbuffer CB : register(b6)
{
	float4 frustumRayTL;// スクリーンの左上に向かうカメラのレイ
	float4 frustumRayTR;// スクリーンの右上に向かうカメラのレイ
	float4 frustumRayBL;// スクリーンの左下に向かうカメラのレイ
	float2 windowSize;   // ウィンドウサイズ
	//int a;
	float2 Enable;
	//vector<bool, 3>	Dummy;
};

cbuffer SkyCB : register(b7)
{
	float3 v3CameraPos;		// The camera's current position
	float fCameraHeight;	// The camera's current height

	float3 v3LightPos;		// The direction vector to the light source
	float fCameraHeight2;	// fCameraHeight^2

	float3 v3InvWavelength;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
	float fScale;			// 1 / (fOuterRadius - fInnerRadius)

	float fOuterRadius;		// The outer (atmosphere) radius
	float fOuterRadius2;	// fOuterRadius^2
	float fInnerRadius;		// The inner (planetary) radius
	float fInnerRadius2;	// fInnerRadius^2

	float fKrESun;			// Kr * ESun
	float fKmESun;			// Km * ESun
	float fKr4PI;			// Kr * 4 * PI
	float fKm4PI;			// Km * 4 * PI

	float fScaleDepth;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
	float fScaleOverScaleDepth;	// fScale / fScaleDepth
	float g;
	float exposure;
};