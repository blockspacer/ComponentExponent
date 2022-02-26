//=============================================================================
//
//  ��C�N���X [Atomosphere.h]
//  Date   : 2021/12/10
//
//=============================================================================
#pragma once

#include "../../2D/Base/Actor2D.h"
#include "../Atomosphere/Atomosphere.h"

// ��C�U���p�o�b�t�@�\����
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

	class Component_Sprite*    m_Component_Sprite;   //�X�v���C�g�R���|�[�l���g

	class Renderer_DX11* m_Renderer_DX11 = nullptr;//DX11�`��N���X�L���b�V��
	ID3D11Buffer* m_SkyRayBuffer; // ��C�U���p�X�N���[�����C�\����
	ID3D11Buffer* m_SkyBuffer;    // ��C�U���p�p�����[�^�\����

	//����
	double m_Time;

	//�V��
	class SkySphere* m_SkySphere;

public:

	static void Load();  //���\�[�X�ǂݍ���
	static void Unload();//���\�[�X�폜

	void Init()override;//������
	void Uninit()override;//�I��
	void Update()override;//�X�V
	void SetSkyRay(SKY_RAY _skyray);
	void SetSky   (SKY _sky);

private:

	Vector3 SunDirection(int _day, double _time, double _eastLongitude, double _northLatitude);
};