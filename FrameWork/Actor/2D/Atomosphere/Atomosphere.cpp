//=============================================================================
//
//  ��C�N���X [Atomosphere.cpp]
//  Date   : 2021/12/10
//
//=============================================================================


#include "../../../Component/Component_Sprite.h"
#include "../../../Component/Component_StaticMesh.h"
#include "../../../Manager/Manager_Texture.h"
#include "../../../Manager/Manager_Shader.h"
#include "../../../Manager/Manager_Game.h"
#include "../../../Renderer/Renderer_DX11.h"
#include "../../../System/MainWindow.h"
#include "../../../Level/Base/Level.h"
#include "../../../System/MainWindow.h"
#include "../../3D/Camera.h"
#include "../../3D/Prop/SkySphere.h"
#include "Atomosphere.h"

#include "../../../Manager/Manager_Drawer/Manager_ImGui.h"
#include "../../../System/Exchanger.h"

constexpr float red   = 0.680f;//��
constexpr float green = 0.550f;//��
constexpr float blue  = 0.440f;//��

constexpr float Kr   = 0.0025f;//���C���[�U��
constexpr float Km   = 0.0010f;//�~�[�U��

constexpr float Esun = 3000.0f;// ���z�̖��邳

constexpr double Longitude = 135.0;//���Ɍ����Ύs�̌o�x
constexpr double Latitude  = 35.0; //���Ɍ����Ύs�̈ܓx

constexpr float rayleighScaleDepth = 0.1f;// ������ς���ƐF���ω�(0.05�`7.0���炢)

constexpr double ONE_FRAME_TIME = 0.005;  //1�t���[���ɐi�s���鎞��
constexpr double MAX_TIME       = 24.0;   //1���̎���
constexpr double SUNRISE_TIME   =  5.0;   //���̏o�̎���
constexpr double SUNSET_TIME    = 23.0;   //���v�̎���

void  Atomosphere::Load()//���\�[�X�ǂݍ���
{
	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//�e�N�X�`���[�}�l�[�W���[�L���b�V��
		manager_Texture->LoadTexture("Atomosphere", "ROM/2D/Test/FadeTexture.png");
	}
}
void  Atomosphere::Unload()//���\�[�X�폜
{
	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//�e�N�X�`���[�}�l�[�W���[�L���b�V��
		manager_Texture->UnloadTexture("Atomosphere");
	}
}

void  Atomosphere::Init()//������
{
	Actor2D::Init();//������

	Int2 windowSize = MainWindow::Instance()->GetWindowSize();
	m_Scale2D = Vector2{ static_cast<float>(windowSize.x),static_cast<float>(windowSize.y) };//2�����T�C�Y�����l

	// Renderer_DX11���L���b�V��
	m_Renderer_DX11 = Renderer_DX11::Instance();

	m_SkySphere = nullptr;
	m_SkyRayBuffer = nullptr;
	m_SkyBuffer = nullptr;

	// Renderer_DX11�ɒ萔�o�b�t�@�𐶐�
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(SKY_RAY);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	HRESULT hreslt = m_Renderer_DX11->GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_SkyRayBuffer);
	m_Renderer_DX11->GetDeviceContext()->VSSetConstantBuffers(6, 1, &m_SkyRayBuffer);
	m_Renderer_DX11->GetDeviceContext()->PSSetConstantBuffers(6, 1, &m_SkyRayBuffer);

	bufferDesc.ByteWidth = sizeof(SKY);

	m_Renderer_DX11->GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_SkyBuffer);
	m_Renderer_DX11->GetDeviceContext()->VSSetConstantBuffers(7, 1, &m_SkyBuffer);
	m_Renderer_DX11->GetDeviceContext()->PSSetConstantBuffers(7, 1, &m_SkyBuffer);

	// ��C�U���p�\���̏�����
	SKY sky{};
	sky.v3CameraPos = Vector3(0.0f, 2.0f, 0.0f);
	sky.fCameraHeight = 4.0f;
	sky.v3LightPos  = Vector3(4.0f, 10.0f, 0.0f);

	const auto invWav = [](float waveLength) { return 1.0f / pow(waveLength, 4.0f); };
	sky.v3InvWavelength = Vector3(invWav(red), invWav(green), invWav(blue));

	//�n���̑傫��
	sky.fInnerRadius = 1000.0f;
	sky.fInnerRadius2 = sky.fInnerRadius * sky.fInnerRadius;

	//��C�̑傫��
	sky.fOuterRadius = 1025.0f;
	sky.fOuterRadius2 = sky.fOuterRadius2 * sky.fOuterRadius2;

	sky.fScale = 1.0f / (sky.fOuterRadius - sky.fInnerRadius);

	sky.fKrESun = Kr * Esun;
	sky.fKmESun = Km * Esun;
	sky.fKr4PI = Kr * 4.0f * Pi;
	sky.fKm4PI = Km * 4.0f * Pi;

	const float rayleighScaleDepth = 0.25f;
	sky.fScaleDepth = rayleighScaleDepth;
	sky.fScaleOverScaleDepth = (1.0f / (sky.fOuterRadius - sky.fInnerRadius)) / rayleighScaleDepth;
	sky.g = -0.999f;
	sky.exposure = 10.0f;

	m_Time = 7.0;//�ŏ��͒�7��

	// �\���̃Z�b�g
	SetSky(sky);

	SKY_RAY ray{};

	ray.frustumRayTR = Vector4(0.2f, 0.3f, 0.5f, 0.0f);
	ray.frustumRayTL = Vector4(-0.3f, -0.5f, -0.2f, 0.0f);
	ray.frustumRayBL = Vector4(0.2f, -0.5f, -0.3f, 0.0f);
	ray.windowSize.x = m_Scale2D.x;
	ray.windowSize.y = m_Scale2D.y;
	ray.Enable       = Vector2(0.0f, 0.0f);
	SetSkyRay(ray);

	//�X�v���C�g�R���|�[�l���g�ݒ�
	m_Component_Sprite = AddComponent<Component_Sprite>(0);//�R���|�[�l���g�ǉ�
	m_Component_Sprite->SetTexture("Atomosphere");	//�e�N�X�`���[�ݒ�
	m_Component_Sprite->SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	m_Component_Sprite->SetShaderType(Manager_Shader::SHADER_ATOMOSPHERE);//�V�F�[�_�[�ݒ�
	m_Component_Sprite->SetPostionZ(1.0f);//Z�l�ݒ�

	if (Level* level = Manager_Game::Instance()->GetLevel())
	{
		m_SkySphere = level->AddActor<SkySphere>();
		if (Component_StaticMesh* component_StaticMesh = m_SkySphere->GetComponentByClass<Component_StaticMesh>())
		{//�R���|�[�l���g�k���`�F�b�N
			component_StaticMesh->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.0f});
		}
	}

}

void Atomosphere::Update()//�X�V
{
	Actor2D::Update();//�X�V

	// �J��������v���W�F�N�V�����ƃr���[���擾
	if (Camera* camera = Manager_Game::Instance()->GetLevel()->GetActor<Camera>())
	{
		D3DXMATRIX View, Projection;
		View = camera->GetViewMatrix();
		Projection = camera->GetProjectionMatrix();

		// �X�N���[�����W�����[���h���W�ɕϊ�
		D3DXMATRIX invV, invPrj, Vp, invVp;
		D3DXMatrixInverse(&invV, NULL, &View);
		D3DXMatrixInverse(&invPrj, NULL, &Projection);
		D3DXMatrixIdentity(&Vp);
		Vp._11 = m_Scale2D.x / 2.0f;
		Vp._22 = -m_Scale2D.y / 2.0f;
		Vp._41 = m_Scale2D.x / 2.0f;
		Vp._42 = m_Scale2D.y / 2.0f;
		D3DXMatrixInverse(&invVp, NULL, &Vp);

		D3DXMATRIX tmp = invVp * invPrj * invV;

		// TL(�X�N���[���̍���)
		Vector3 worldPosTL = Vector3_Zero;
		D3DXVec3TransformCoord(&worldPosTL, &Vector3(0.0f, 0.0f, 1.0f), &tmp);
		D3DXVec3Normalize(&worldPosTL, &worldPosTL);

		// TR(�X�N���[���̉E��)
		Vector3 worldPosTR = Vector3_Zero;
		D3DXVec3TransformCoord(&worldPosTR, &Vector3(m_Scale2D.x, 0.0f, 1.0f), &tmp);
		D3DXVec3Normalize(&worldPosTR, &worldPosTR);

		// BL(�X�N���[���̍���)
		Vector3 worldPosBL = Vector3_Zero;
		D3DXVec3TransformCoord(&worldPosBL, &Vector3(0.0f, m_Scale2D.y, 1.0f), &tmp);
		D3DXVec3Normalize(&worldPosBL, &worldPosBL);


		// �o�b�t�@�ɍ\���̂��Z�b�g
		SKY_RAY ray{};

		ray.frustumRayTR = Vector4(worldPosTR, 0.0f);
		ray.frustumRayTL = Vector4(worldPosTL, 0.0f);
		ray.frustumRayBL = Vector4(worldPosBL, 0.0f);
		ray.windowSize   = Vector2(m_Scale2D.x, m_Scale2D.y);
		ray.Enable       = Vector2_Zero;

		SetSkyRay(ray);
		Manager_ImGui::Instance()->GetMainDebugWindow()->
			DebugLogDontDestroy(
				Exchanger::DOUBLE_TO_STRING(m_Time),
				"Camera", Vector4(1, 1, 1, 1));

		//���ԉ��Z
		m_Time += ONE_FRAME_TIME;
		if (m_Time >= MAX_TIME)
		{//�O���œ��t�ύX
			m_Time = 0.0;
		}

		////���̃I�p�V�e�C
		if (m_Time >= SUNSET_TIME)
		{
			if (Component_StaticMesh* component_StaticMesh = m_SkySphere->GetComponentByClass<Component_StaticMesh>())
			{//�R���|�[�l���g�k���`�F�b�N
				component_StaticMesh->SetColor(Vector4{1.0f,1.0f,1.0f,1.0f });
			}
		}
		else if (m_Time >= SUNRISE_TIME)
		{
			if (Component_StaticMesh* component_StaticMesh = m_SkySphere->GetComponentByClass<Component_StaticMesh>())
			{//�R���|�[�l���g�k���`�F�b�N
				component_StaticMesh->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.0f});
			}
		}


		// ��C�U���̌v�Z
		SKY sky{};
		sky.v3LightPos = SunDirection(0, m_Time, 90.0, 0.0);
		D3DXVec3Normalize(&sky.v3LightPos, &sky.v3LightPos);
		sky.fCameraHeight2 = sky.fCameraHeight*sky.fCameraHeight;


		const auto invWav = [](float waveLength) { return 1.0f / pow(waveLength, 4.0f); };
		sky.v3InvWavelength = Vector3(invWav(red), invWav(green), invWav(blue));

		// �n���̑傫��
		sky.fInnerRadius = 1000.0f;
		sky.fInnerRadius2 = sky.fInnerRadius * sky.fInnerRadius;

		// ��C�̑傫��
		sky.fOuterRadius = 1500.0f;
		sky.fOuterRadius2 = sky.fOuterRadius * sky.fOuterRadius;

		sky.fScale = 1.0f / (sky.fOuterRadius - sky.fInnerRadius);

		sky.fKrESun = Kr * Esun;
		sky.fKmESun = Km * Esun;
		sky.fKr4PI = Kr * 4.0f * Pi;
		sky.fKm4PI = Km * 4.0f * Pi;

		//rayleighScaleDepth�ƘI�����ʂ𒲐����đ�C�F�𒲐�
		sky.fScaleDepth = rayleighScaleDepth;
		sky.fScaleOverScaleDepth = (1.0f / (sky.fOuterRadius - sky.fInnerRadius)) / rayleighScaleDepth;
		sky.g = -0.999f;
		//�I������0.01�`0.5���炢�Œ���
		sky.exposure = 0.03f;

		// �\���̃Z�b�g
		SetSky(sky);
	}
}

void Atomosphere::Uninit()//�I��
{
	//Actor2D::Uninit();//�I��

	// �o�b�t�@�̃����[�X
	m_SkyRayBuffer->Release();
	m_SkyRayBuffer = nullptr;
	m_SkyBuffer->Release();
	m_SkyBuffer = nullptr;

	m_SkySphere = nullptr;
}

// �����A�o�x�ܓx�Ō��̌������Z�o
Vector3 Atomosphere::SunDirection(int _day, double _time, double _eastLongitude, double _northLatitude)
{
	const double omega = Pi / 365.0;
	const double J = _day + 0.5;

	const double sigma = (0.33281 - 22.984*cos(omega*J) - 0.34990*cos(2 * omega*J) - 0.13980*cos(3 * omega*J) + 3.7872* sin(omega*J) + 0.03250*sin(2 * omega*J) + 0.07187*sin(3 * omega*J))*Pi / 180.0f;
	const double e = 0.0072*cos(omega*J) - 0.0528*cos(2 * omega*J) - 0.0012*cos(3 * omega*J) - 0.1229*sin(omega*J) - 0.1565*sin(2 * omega*J) - 0.0041*sin(3 * omega*J);

	const double T = _time + (_eastLongitude - 135.0) / 15.0 + e;
	const double t = (15.0*T - 180.0) * Pi / 180.0f;

	const double phi = _northLatitude * Pi / 180.0f;
	const double h = asin(sin(phi)*sin(sigma) + cos(phi)*cos(sigma)*cos(t));

	const double sinA = cos(sigma)*sin(t) / cos(h);
	const double cosA = (sin(h)*sin(phi) - sin(sigma)) / cos(h) / cos(phi);

	const double y = sin(h);
	const double xz_length = sqrt(1.0 - y * y);
	const double z = cosA * xz_length;
	const double x = sinA * xz_length;

	return Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
}

void Atomosphere::SetSkyRay(SKY_RAY _skyray)
{
	m_Renderer_DX11->GetDeviceContext()->UpdateSubresource(m_SkyRayBuffer, 0, NULL, &_skyray, 0, 0);
}

void Atomosphere::SetSky(SKY _sky)
{
	m_Renderer_DX11->GetDeviceContext()->UpdateSubresource(m_SkyBuffer, 0, NULL, &_sky, 0, 0);
}
