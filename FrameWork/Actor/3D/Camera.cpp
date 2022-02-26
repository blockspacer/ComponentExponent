//=============================================================================
//
//  �J�������� [Camera.cpp]
//  Date   : 2021/11/20
//
//=============================================================================
//******************************************************************************
// �C���N���[�h
//******************************************************************************
#include "Camera.h"
#include "../../System/MainWindow.h"
#include "../../Renderer/Renderer_DX11.h"
#include "../../Manager/Manager_Game.h"
#include "../../Manager/Manager_Drawer/Manager_ImGui.h"
#include "../../Component/Component_OBB.h"
#include "../../Component/Component_Rigidbody.h"
#include "../../Level/Base/Level.h"
#include "../../Component/Component_OBB.h"

#include "ActorRB/Charcter/Base/Character.h"
#include "../../System/Exchanger.h"

constexpr float CAMERA_NEAR   = 0.01f;
constexpr float CAMERA_FAR    = 500.0f;

//=============================================================================
// �������֐�
//=============================================================================
void Camera::Init()
{
	Actor3D::Init();

	//�����_�ΏۃA�N�^�[nullptr��
	m_Owner = nullptr;

	//�������W
	m_Position = Vector3(0.5f, 2.5f, 0.5f);
	//�����X�P�[��
	m_Scale    = Vector3(0.5f, 0.5f, 0.5f);
	//�����^�[�Q�b�g
	m_Target = m_Position + GetForward();

	//���W�I�t�Z�b�g
	m_Offset_Position = Vector3_Zero;

	m_Component_OBB = AddComponent<Component_OBB>(0);
	m_Component_OBB->SetVisibility(true);//����
	m_Component_OBB->SetScale(Vector3{ 1,1,1 });//�X�P�[���ݒ�

	//����
	m_FOV    = 1.0f;

	//�J��������
	m_TargetArmLength = 2.0f;

	//�i�s�������������ǂ���
	m_OrientRotationtoMovement = false;

	m_IsUseRayHit = true;

}

//=============================================================================
// �I�������֐�
//=============================================================================
void Camera::Uninit()
{
	Actor3D::Uninit();
}

//=============================================================================
// �X�V�����֐�
//=============================================================================
void Camera::Update()
{
	if (m_Owner != nullptr)
	{//�����_�Ώ�null�`�F�b�N

		//�����_�擾
		m_Target = m_Owner->GetPosition() + m_Offset_Position;

		//�����_�Ώۂ̑O���x�N�g�����L���b�V��
		Vector3 targetForward = m_Owner->GetForward();

		if (m_OrientRotationtoMovement)
		{//Y���Œ�J����
			m_Position.y = m_Target.y - GetForward().y  * m_TargetArmLength;
			m_Position.x = m_Target.x - targetForward.x * m_TargetArmLength;
			m_Position.z = m_Target.z - targetForward.z * m_TargetArmLength;
		}
		else
		{//�T�[�h�p�[�\���r���[
			m_Position = m_Target - GetForward() * m_TargetArmLength;
		}
	}
	else
	{
		m_Target = m_Position + GetForward();
	}

	Actor3D::Update();

	// �J�����o�b�t�@�̃Z�b�g
	CAMERA camera;
	camera.Position = Vector4(m_Position, 0.0f);
	camera.Length = D3DXVec3Length(&m_Position);

	if(m_IsUseRayHit)
	{
		//�v���C���[����J�����܂Ń��C�Ń`�F�b�N
		if (m_Owner)
		{
			std::vector < Manager_OBB::s_RaycastHit > hit =
				GetComponentByClass<Component_OBB>()->RaycastGetHit(m_Owner->GetPosition(), m_Position);

			//���C�n�Y��
			if (hit.empty())
			{
				Manager_ImGui::Instance()->GetMainDebugWindow()->DebugLog(
					"No Hit", "rayHitObjName", Vector4(1, 1, 1, 1), 1);
			}
			else//���C�����������ꍇ
			{
				Manager_OBB::s_RaycastHit* nearestObj = nullptr;
				for (auto& i : hit)
				{
					if (i.hitObj == nullptr)continue;
					if (!i.hitObj->GetComponentByClass<Component_OBB>()->GetIsReactRay())continue;//���C�ɔ������Ȃ�

					//�J�����̌����Ă���v���C���[�͔��肵�Ȃ�
					if (m_Owner != i.hitObj)
					{
						Manager_ImGui::Instance()->GetMainDebugWindow()->DebugLog(
							i.hitObj->GetName(), "rayHitObjName", Vector4(1, 0, 0, 1), 6);

						//null���v���C���[�ɋ߂��I�u�W�F�N�g
						if (!nearestObj || i.length < nearestObj->length)
						{
							nearestObj = &i;
						}
					}
				}

				//�v���C���[�ƃJ�����ȊO�̃I�u�W�F�N�g�ɓ���������v���C���[��
				//��ԋ߂��I�u�W�F�N�g�Ƀq�b�g�����ꏊ�ɃJ�������Z�b�g����
				if (nearestObj)
				{
					m_Position = nearestObj->hitPos;
					Vector3 tempDir;
					D3DXVec3Normalize(&tempDir, &(m_Position - m_Owner->GetPosition()));
					m_Position -= tempDir * m_Scale.z;
				}
			}
		}
	}

	Renderer_DX11::Instance()->SetCamera(camera);
}

//=============================================================================
// ���_�ݒ菈���֐�
//=============================================================================
void Camera::SetView()
{
	// �r���[�}�g���N�X�ݒ�
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target,
		&Vector3(0.0f, 1.0f, 0.0f));

	Renderer_DX11::Instance()->SetViewMatrix(&m_ViewMatrix);

	// ��p�ݒ�
	float windowX = static_cast<float>(MainWindow::Instance()->GetWindowSize().x);
	float windowY = static_cast<float>(MainWindow::Instance()->GetWindowSize().y);
	// �v���W�F�N�V�����}�g���N�X�ݒ�
	//D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, m_FOV,
	//	static_cast<float>(windowX) / static_cast<float>(windowY), CAMERA_NEAR, CAMERA_FAR);

	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, m_FOV,
		static_cast<float>(windowX) / static_cast<float>(windowY), 1.0f, 10000.0f);

	Renderer_DX11::Instance()->SetProjectionMatrix(&m_ProjectionMatrix);
}
//=============================================================================
// ������J�����O�֐�
//=============================================================================
bool Camera::CheckView(Vector3 _position, Vector3 _scale)
{
	//D3DXMATRIX vp, invvp;

	//vp = m_ViewMatrix * m_ProjectionMatrix;
	//D3DXMatrixInverse(&invvp, NULL, &vp);

	//Vector3 vpos[4];
	//Vector3 wpos[4];

	//vpos[0] = Vector3(-1.0f, 1.0f, 1.0f);
	//vpos[1] = Vector3(1.0f, 1.0f, 1.0f);
	//vpos[2] = Vector3(-1.0f, -1.0f, 1.0f);
	//vpos[3] = Vector3(1.0f, -1.0f, 1.0f);

	//D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
	//D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
	//D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
	//D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);

	//Vector3 v, v1, v2, n;

	//// �J�������猩���I�u�W�F�N�g�̃x�N�g��
	//v = _position - m_Position;

	///*=============== ���� ==================*/
	//v1 = wpos[0] - m_Position;
	//v2 = wpos[2] - m_Position;
	//D3DXVec3Cross(&n, &v1, &v2);

	//if (D3DXVec3Dot(&n, &v) < 0.0f)
	//{
	//	return false;
	//}

	///*=============== �E�� ==================*/
	//v1 = wpos[1] - m_Position;
	//v2 = wpos[3] - m_Position;
	//D3DXVec3Cross(&n, &v1, &v2);

	//if (D3DXVec3Dot(&n, &v) > 0.0f)
	//{
	//	return false;
	//}

	///*=============== ��� ==================*/
	//v1 = wpos[0] - m_Position;
	//v2 = wpos[1] - m_Position;
	//D3DXVec3Cross(&n, &v1, &v2);

	//if (D3DXVec3Dot(&n, &v) > 0.0f)
	//{
	//	return false;
	//}

	///*=============== ���� ==================*/
	//v1 = wpos[2] - m_Position;
	//v2 = wpos[3] - m_Position;
	//D3DXVec3Cross(&n, &v1, &v2);

	//if (D3DXVec3Dot(&n, &v) < 0.0f)
	//{
	//	return false;
	//}

	//return true;

	return false;
}


//�O��ړ�
void Camera::Move(float _forward, float _right)
{
	m_Position = m_Position + _forward * GetForward() + _right * GetRight();
}


//�J������]
void Camera::AddCameraYaw(float _value)  //Yaw���Z
{
	m_Rotation.y += _value;

	//Yaw�����K��
	if (m_Rotation.y < DegToRad(-180.0f))
	{
		m_Rotation.y += DegToRad(360.0f);
		if (m_Owner != nullptr)
		{//�I�[�i�[���L���b�V�����I�[�i�[�����K��
			Vector3 rotation = m_Owner->GetRotation();
			m_Owner->SetRotation(Vector3{ rotation .x,rotation.y + DegToRad(360.0f),rotation.z});
		}

	}
	else if (m_Rotation.y > DegToRad(180.0f))
	{
		m_Rotation.y -= DegToRad(360.0f);
		if (m_Owner != nullptr)
		{//�I�[�i�[���L���b�V�����I�[�i�[�����K��
			Vector3 rotation = m_Owner->GetRotation();
			m_Owner->SetRotation(Vector3{ rotation.x,rotation.y - DegToRad(360.0f),rotation.z });
		}
	}

}
void Camera::AddCameraPitch(float _value)//Pitch���Z
{
	m_Rotation.x = Math::clamp(m_Rotation.x + _value, -VIEW_PITCH_MAX, VIEW_PITCH_MAX);
}


//�^�[�Q�b�g�������Z
void Camera::AddTargetLength(float _length)
{
	//1�t���[���ŉ��Z�����J�����I�t�Z�b�gY���W
	float offset_y = (MAX_OFFSET_POSITION_Y - MIN_OFFSET_POSITION_Y) / ((MAX_TARGET_ARM_LENGTH - MIN_TARGET_ARM_LENGTH) / _length);

	m_TargetArmLength = Math::clamp(m_TargetArmLength + _length, MIN_TARGET_ARM_LENGTH, MAX_TARGET_ARM_LENGTH);

	m_Offset_Position.y = Math::clamp(m_Offset_Position.y - offset_y, MIN_OFFSET_POSITION_Y, MAX_OFFSET_POSITION_Y);
}
