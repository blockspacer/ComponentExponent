//=============================================================================
//
//  大気クラス [Atomosphere.h]
//  Date   : 2021/12/10
//
//=============================================================================
#pragma once

#include "../../2D/Base/Actor2D.h"
#include "../Atomosphere/Atomosphere.h"

// 大気散乱用バッファ構造体
struct SKY_RAY
{
	D3DXVECTOR4 frustumRayTL;
	D3DXVECTOR4 frustumRayTR;
	D3DXVECTOR4 frustumRayBL;
	D3DXVECTOR2 windowSize;
	D3DXVECTOR2	Enable;
};

struct SKY
{
	Vector3 v3CameraPos;    // The camera's current position
	float   fCameraHeight;	// The camera's current height

	Vector3 v3LightPos;		// The direction vector to the light source
	float fCameraHeight2;	// fCameraHeight^2

	Vector3 v3InvWavelength;// 1 / pow(wavelength, 4) for the red, green, and blue channels
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


class Atomosphere : public Actor2D
{
private:

	class Component_Sprite*    m_Component_Sprite;   //スプライトコンポーネント

	class Renderer_DX11* m_Renderer_DX11 = nullptr;//DX11描画クラスキャッシュ
	ID3D11Buffer* m_SkyRayBuffer; // 大気散乱用スクリーンレイ構造体
	ID3D11Buffer* m_SkyBuffer;    // 大気散乱用パラメータ構造体

	//時間
	double m_Time;

	//天球
	class SkySphere* m_SkySphere;

public:

	static void Load();  //リソース読み込み
	static void Unload();//リソース削除

	void Init()override;//初期化
	void Uninit()override;//終了
	void Update()override;//更新
	void SetSkyRay(SKY_RAY _skyray);
	void SetSky   (SKY _sky);

private:

	Vector3 SunDirection(int _day, double _time, double _eastLongitude, double _northLatitude);
};