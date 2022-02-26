//=============================================================================
//
//  OBB�R���|�[�l���g�}�l�[�W���[ [Manager_OBB.cpp]
//  Date   : 2021/11/10
//
//=============================================================================

//******************************************************************************
// �C���N���[�h
//******************************************************************************

#include <stdarg.h>
#include <codecvt>
#include "Manager_OBB.h"
#include "Manager_ImGui.h"
#include "../Manager_Game.h"
#include "../../Actor/Base/Actor.h"
#include "../../Actor/3D/Camera.h"
#include "../../Actor/3D/Prop/Bullet.h"
#include "../../Actor/3D/ActorRB/Charcter/Base/Character.h"
#include "../../Component/Component_OBB.h"
#include "../../System/Math.h"
#include "../../System/Exchanger.h"
#include "../../Renderer/Renderer_Polygon.h"
#include "../../System/Time.h"
#include "../Manager_Audio.h"
#include "../../Level/Level_TP.h"

//�X�V�֐�
void Manager_OBB::Update()
{
	//�v���C���[�̔���
	for (Component_OBB* OBB_Player : m_ComponentList[COLLISION_PLAYER])
	{
		if (OBB_Player->GetActive())//�L����
		{
			//�v���C���[�ƃv���C���[�̔���
			for (Component_OBB* OBB_PlayerB : m_ComponentList[COLLISION_PLAYER])
			{
				if (OBB_PlayerB->GetActive() && !(OBB_Player == OBB_PlayerB))//�L�������g�ƈقȂ�v���C���[
				{
					//todo:
				}
			}
			//�v���C���[�ƃG�l�~�[�̔���
			for (Component_OBB* OBB_Enemy : m_ComponentList[COLLISION_ENEMY])
			{
				if (OBB_Enemy->GetActive())//�L����
				{
					if (OverlapJudgeOBB(OBB_Player, OBB_Enemy))
					{
						OBB_Player->SetColor(Color{ 1.0f,0.0f,1.0f,1.0f });
						OBB_Enemy->SetColor (Color{ 1.0f,0.0f,0.0f,1.0f });
					}
				}
			}
		}
	}

	//�e�̔���
	for (Component_OBB* OBB_Bullet : m_ComponentList[COLLISION_BULLET])
	{
		if (OBB_Bullet->GetActive())//�L����
		{
			//�e�ƃG�l�~�[�̔���
			for (Component_OBB* OBB_Enemy : m_ComponentList[COLLISION_ENEMY])
			{
				if (OBB_Enemy->GetActive())//�L����
				{
					if (OverlapJudgeOBB(OBB_Bullet, OBB_Enemy))
					{
						OBB_Bullet->SetColor(Color{ 0.0f,0.0f,1.0f,1.0f });
						OBB_Enemy->SetColor(Color{ 1.0f,0.0f,0.0f,1.0f });
					}
				}
			}
		}
	}

	//�G�l�~�[�̔���
	for (Component_OBB* OBB_Enemy : m_ComponentList[COLLISION_ENEMY])
	{
		if (OBB_Enemy->GetActive())//�L����
		{
			//�G�l�~�[�ƃG�l�~�[�̔���
			for (Component_OBB* OBB_EnemyB : m_ComponentList[COLLISION_ENEMY])
			{
				if (OBB_EnemyB->GetActive() && !(OBB_Enemy == OBB_EnemyB))//�L�������g�ƈقȂ�G�l�~�[
				{
					if (OverlapJudgeOBB(OBB_Enemy, OBB_EnemyB))
					{
						OBB_Enemy->SetColor(Color{ 0.0f,1.0f,0.0f,1.0f });
						OBB_Enemy->SetColor(Color{ 0.0f,1.0f,0.0f,1.0f });
					}
				}
			}
		}
	}

	//�A�C�e���̔���
	for (auto& i : m_ComponentList[COLLISION_ITEM])
	{
		if (i->GetActive())
		{
			//�L�����N�^�[�Ƃ̔���
			for (auto& k : m_ComponentList[COLLISION_PLAYER])
			{
				if (k->GetActive())
				{
					if (OverlapJudgeOBB(i, k))
					{
						Manager_Audio::Instance()->Play("coin", 0);
						if (Level_Test* level = dynamic_cast<Level_Test*>(Manager_Game::Instance()->GetLevel()))
						{
							level->AddScore();
						}
						i->GetOwner()->SetDestroy();
					}
				}
			}
		}
	}

	for (auto& i : debugDrawRayList)
	{
		i.time++;
	}

	debugDrawRayList.remove_if([](s_DrawData _data) {return _data.time >= _data.timeMax; });
}

//�`��֐�
void Manager_OBB::Draw()
{
	for (int i = 0; i < MAX_ACTOR_TYPE; i++)//�R���W�����R���|�[�l���g���X�g�̃N���A
	{
		for (Component_OBB* component_OBB : m_ComponentList[i])//�`��
		{
			component_OBB->Draw();
		}
	}

	for (auto& i : debugDrawRayList)
	{
		Renderer_Polygon::Instance()->DrawLine(i.startPos, i.endPos, i.color);
	}
}

//�I���֐�
void Manager_OBB::Uninit()
{
	for (int i = 0; i < MAX_ACTOR_TYPE; i++)//�R���W�����R���|�[�l���g���X�g�̃N���A
	{
		m_ComponentList[i].clear(); // CC���X�g�̃N���A
	}
}

void Manager_OBB::RemoveComponent(Component_OBB * _del)
{
	for (int k = 0; k < ACTOR_TYPE::MAX_ACTOR_TYPE; k++)
	{
		for (auto& i : m_ComponentList[k])
		{
			if (i == _del)
			{
				//delete _del;
				//_del = nullptr;

				Component_OBB* temp = i;

				m_ComponentList[k].remove(_del);

				return;
			}
		}
	}
}

//OBB��OBB�̃I�[�o�[���b�v����
bool Manager_OBB::OverlapJudgeOBB(Component_OBB* _component_OBBA , Component_OBB* _component_OBBB)
{
	//�{�b�N�X���m�̋���
	Vector3 distance = _component_OBBB->GetPosition() - _component_OBBA->GetPosition();

	//��r�p�̍\����
	s_OBBCompData A = {};
	s_OBBCompData B = {};

	A.lengthX = _component_OBBA->GetLengthX();
	A.lengthY = _component_OBBA->GetLengthY();
	A.lengthZ = _component_OBBA->GetLengthZ();
	B.lengthX = _component_OBBB->GetLengthX();
	B.lengthY = _component_OBBB->GetLengthY();
	B.lengthZ = _component_OBBB->GetLengthZ();

	//���[�J����ꎲ�x�N�g���ɂ��ꂼ��̉�]�𔽉f������
	D3DXMATRIX mtxRA;
	Vector3 tempCmpRotA = _component_OBBA->GetRotation();
	D3DXMatrixRotationYawPitchRoll(&mtxRA, tempCmpRotA.y, tempCmpRotA.x, tempCmpRotA.z);
	D3DXVec3TransformCoord(&A.axisX, &A.axisX, &mtxRA);
	D3DXVec3TransformCoord(&A.axisY, &A.axisY, &mtxRA);
	D3DXVec3TransformCoord(&A.axisZ, &A.axisZ, &mtxRA);

	D3DXMATRIX mtxRB;
	Vector3 tempCmpRotB = _component_OBBB->GetRotation();
	D3DXMatrixRotationYawPitchRoll(&mtxRB, tempCmpRotB.y, tempCmpRotB.x, tempCmpRotB.z);
	D3DXVec3TransformCoord(&B.axisX, &B.axisX, &mtxRB);
	D3DXVec3TransformCoord(&B.axisY, &B.axisY, &mtxRB);
	D3DXVec3TransformCoord(&B.axisZ, &B.axisZ, &mtxRB);

	//�{�b�N�XA��̃��[�J����ꎲ��̉e�Z�o
	if (!CompareLength(&A, &B, &A.axisX, &distance))return false;//X�𕪗�����
	if (!CompareLength(&A, &B, &A.axisY, &distance))return false;//Y�𕪗�����
	if (!CompareLength(&A, &B, &A.axisZ, &distance))return false;//Z�𕪗�����

	//�{�b�N�XB��̃��[�J����ꎲ��̉e�Z�o
	if (!CompareLength(&A, &B, &B.axisX, &distance))return false;//X�𕪗�����
	if (!CompareLength(&A, &B, &B.axisY, &distance))return false;//Y�𕪗�����
	if (!CompareLength(&A, &B, &B.axisZ, &distance))return false;//Z�𕪗�����

	Vector3 separate = {};
	//���݂��̊�ꎲ���m�̊O�ʃx�N�g����̉e�Z�o
	{
		//�{�b�N�XA��X
		{
			//�Ɓ@�{�b�N�XB��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisX, &B.axisX);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//�Ɓ@�{�b�N�XB��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisX, &B.axisY);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//�Ɓ@�{�b�N�XB��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisX, &B.axisZ);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
		}
		//�{�b�N�XAY
		{
			//�Ɓ@�{�b�N�XB��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisY, &B.axisX);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//�Ɓ@�{�b�N�XB��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisY, &B.axisY);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//�Ɓ@�{�b�N�XB��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisY, &B.axisZ);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
		}
		//�{�b�N�XAZ
		{
			//�Ɓ@�{�b�N�XB��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisX);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//�Ɓ@�{�b�N�XB��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisY);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//�Ɓ@�{�b�N�XB��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisZ);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
		}
	}

	return true;
}

bool Manager_OBB::BlockJudgeOBB(Component_OBB * _component_OBBA, Component_OBB * _component_OBBB)
{
	//�{�b�N�X���m�̋���
	Vector3 distance = _component_OBBB->GetPosition() - _component_OBBA->GetPosition();

	//��r�p�̍\����
	s_OBBCompData A = {};
	s_OBBCompData B = {};

	Component_OBB* Acomp = _component_OBBA;
	Component_OBB* Bcomp = _component_OBBB;

	A.lengthX = _component_OBBA->GetLengthX();
	A.lengthY = _component_OBBA->GetLengthY();
	A.lengthZ = _component_OBBA->GetLengthZ();
	B.lengthX = _component_OBBB->GetLengthX();
	B.lengthY = _component_OBBB->GetLengthY();
	B.lengthZ = _component_OBBB->GetLengthZ();

	//���[�J����ꎲ�x�N�g���ɂ��ꂼ��̉�]�𔽉f������
	D3DXMATRIX mtxRA;
	Vector3 tempCmpRotA = _component_OBBA->GetRotation();
	D3DXMatrixRotationYawPitchRoll(&mtxRA, tempCmpRotA.y, tempCmpRotA.x, tempCmpRotA.z);
	D3DXVec3TransformCoord(&A.axisX, &A.axisX, &mtxRA);
	D3DXVec3TransformCoord(&A.axisY, &A.axisY, &mtxRA);
	D3DXVec3TransformCoord(&A.axisZ, &A.axisZ, &mtxRA);

	D3DXMATRIX mtxRB;
	Vector3 tempCmpRotB = _component_OBBB->GetRotation();
	D3DXMatrixRotationYawPitchRoll(&mtxRB, tempCmpRotB.y, tempCmpRotB.x, tempCmpRotB.z);
	D3DXVec3TransformCoord(&B.axisX, &B.axisX, &mtxRB);
	D3DXVec3TransformCoord(&B.axisY, &B.axisY, &mtxRB);
	D3DXVec3TransformCoord(&B.axisZ, &B.axisZ, &mtxRB);

	//�{�b�N�XA��̃��[�J����ꎲ��̉e�Z�o
	if (!CompareLengthAndGetCaveinLength(&A, &B, &A.axisX, &distance, Acomp, Bcomp))return false;//X�𕪗�����
	if (!CompareLengthAndGetCaveinLength(&A, &B, &A.axisY, &distance, Acomp, Bcomp))return false;//Y�𕪗�����
	if (!CompareLengthAndGetCaveinLength(&A, &B, &A.axisZ, &distance, Acomp, Bcomp))return false;//Z�𕪗�����

	//�{�b�N�XB��̃��[�J����ꎲ��̉e�Z�o
	if (!CompareLengthAndGetCaveinLength(&A, &B, &B.axisX, &distance, Acomp, Bcomp))return false;//X�𕪗�����
	if (!CompareLengthAndGetCaveinLength(&A, &B, &B.axisY, &distance, Acomp, Bcomp))return false;//Y�𕪗�����
	if (!CompareLengthAndGetCaveinLength(&A, &B, &B.axisZ, &distance, Acomp, Bcomp))return false;//Z�𕪗�����

	Vector3 separate = {};
	//���݂��̊�ꎲ���m�̊O�ʃx�N�g����̉e�Z�o
	{
		//�{�b�N�XA��X
		{
			//�Ɓ@�{�b�N�XB��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisX, &B.axisX);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
			//�Ɓ@�{�b�N�XB��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisX, &B.axisY);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
			//�Ɓ@�{�b�N�XB��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisX, &B.axisZ);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
		}
		//�{�b�N�XAY
		{
			//�Ɓ@�{�b�N�XB��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisY, &B.axisX);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
			//�Ɓ@�{�b�N�XB��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisY, &B.axisY);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
			//�Ɓ@�{�b�N�XB��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisY, &B.axisZ);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
		}
		//�{�b�N�XAZ
		{
			//�Ɓ@�{�b�N�XB��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisX);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
			//�Ɓ@�{�b�N�XB��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisY);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
			//�Ɓ@�{�b�N�XB��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisZ);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
		}
	}

	return true;
}

bool Manager_OBB::OverlapJudgeOBBandPoint(Component_OBB * _component_OBB, Vector3 _point)
{
	//�{�b�N�X���m�̋���
	Vector3 distance = _component_OBB->GetPosition() - _point;

	//��r�p�̍\����
	s_OBBCompData A = {};
	s_OBBCompData B = {};

	A.lengthX = 0.0f;
	A.lengthY = 0.0f;
	A.lengthZ = 0.0f;
	B.lengthX = _component_OBB->GetLengthX();
	B.lengthY = _component_OBB->GetLengthY();
	B.lengthZ = _component_OBB->GetLengthZ();

	//���[�J����ꎲ�x�N�g���ɂ��ꂼ��̉�]�𔽉f������
	D3DXMATRIX mtxRA;
	Vector3 tempCmpRotA = Vector3(0, 0, 0);
	D3DXMatrixRotationYawPitchRoll(&mtxRA, tempCmpRotA.y, tempCmpRotA.x, tempCmpRotA.z);
	D3DXVec3TransformCoord(&A.axisX, &A.axisX, &mtxRA);
	D3DXVec3TransformCoord(&A.axisY, &A.axisY, &mtxRA);
	D3DXVec3TransformCoord(&A.axisZ, &A.axisZ, &mtxRA);

	D3DXMATRIX mtxRB;
	Vector3 tempCmpRotB = _component_OBB->GetRotation();
	D3DXMatrixRotationYawPitchRoll(&mtxRB, tempCmpRotB.y, tempCmpRotB.x, tempCmpRotB.z);
	D3DXVec3TransformCoord(&B.axisX, &B.axisX, &mtxRB);
	D3DXVec3TransformCoord(&B.axisY, &B.axisY, &mtxRB);
	D3DXVec3TransformCoord(&B.axisZ, &B.axisZ, &mtxRB);

	//�{�b�N�XA��̃��[�J����ꎲ��̉e�Z�o
	if (!CompareLength(&A, &B, &A.axisX, &distance))return false;//X�𕪗�����
	if (!CompareLength(&A, &B, &A.axisY, &distance))return false;//Y�𕪗�����
	if (!CompareLength(&A, &B, &A.axisZ, &distance))return false;//Z�𕪗�����

	//�{�b�N�XB��̃��[�J����ꎲ��̉e�Z�o
	if (!CompareLength(&A, &B, &B.axisX, &distance))return false;//X�𕪗�����
	if (!CompareLength(&A, &B, &B.axisY, &distance))return false;//Y�𕪗�����
	if (!CompareLength(&A, &B, &B.axisZ, &distance))return false;//Z�𕪗�����

	Vector3 separate = {};
	//���݂��̊�ꎲ���m�̊O�ʃx�N�g����̉e�Z�o
	{
		//�{�b�N�XA��X
		{
			//�Ɓ@�{�b�N�XB��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisX, &B.axisX);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//�Ɓ@�{�b�N�XB��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisX, &B.axisY);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//�Ɓ@�{�b�N�XB��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisX, &B.axisZ);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
		}
		//�{�b�N�XAY
		{
			//�Ɓ@�{�b�N�XB��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisY, &B.axisX);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//�Ɓ@�{�b�N�XB��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisY, &B.axisY);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//�Ɓ@�{�b�N�XB��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisY, &B.axisZ);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
		}
		//�{�b�N�XAZ
		{
			//�Ɓ@�{�b�N�XB��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisX);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//�Ɓ@�{�b�N�XB��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisY);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//�Ɓ@�{�b�N�XB��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisZ);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
		}
	}

	return true;
}

bool Manager_OBB::CompareLength(s_OBBCompData * _A, s_OBBCompData * _B, Vector3 * _separate, Vector3 * _distance)
{
	//��������ł̃{�b�N�XA�̒��S����{�b�N�XB�̒��S�܂ł̍��W
	float distance = fabsf(D3DXVec3Dot(_distance, _separate));
	//��������Ń{�b�N�XA�̒��S����ł������{�b�N�XA�̒��_�܂ł̋���
	float shadowA = 0.0f;
	//��������Ń{�b�N�XB�̒��S����ł������{�b�N�XB�̒��_�܂ł̋���
	float shadowB = 0.0f;
	//���ꂼ��̃{�b�N�X�̉e���Z�o
	shadowA = fabsf(D3DXVec3Dot(&_A->axisX, _separate) * _A->lengthX) +
		fabsf(D3DXVec3Dot(&_A->axisY, _separate) * _A->lengthY) + 
		fabsf(D3DXVec3Dot(&_A->axisZ, _separate) * _A->lengthZ);

	shadowB = fabsf(D3DXVec3Dot(&_B->axisX, _separate) * _B->lengthX) +
		fabsf(D3DXVec3Dot(&_B->axisY, _separate) * _B->lengthY) +
		fabsf(D3DXVec3Dot(&_B->axisZ, _separate) * _B->lengthZ);

	if (distance > shadowA + shadowB) return false;
	return true;
}

bool Manager_OBB::CompareLengthAndGetCaveinLength(
	s_OBBCompData * _A, s_OBBCompData * _B, Vector3 * _separate,
	Vector3 * _distance, Component_OBB* _Acmp, Component_OBB* _Bcmp)
{
	//��������ł̃{�b�N�XA�̒��S����{�b�N�XB�̒��S�܂ł̍��W
	float distance = fabsf(D3DXVec3Dot(_distance, _separate));
	//��������Ń{�b�N�XA�̒��S����ł������{�b�N�XA�̒��_�܂ł̋���
	float shadowA = 0.0f;
	//��������Ń{�b�N�XB�̒��S����ł������{�b�N�XB�̒��_�܂ł̋���
	float shadowB = 0.0f;
	//���ꂼ��̃{�b�N�X�̉e���Z�o
	shadowA = fabsf(D3DXVec3Dot(&_A->axisX, _separate) * _A->lengthX) +
		fabsf(D3DXVec3Dot(&_A->axisY, _separate) * _A->lengthY) +
		fabsf(D3DXVec3Dot(&_A->axisZ, _separate) * _A->lengthZ);

	shadowB = fabsf(D3DXVec3Dot(&_B->axisX, _separate) * _B->lengthX) +
		fabsf(D3DXVec3Dot(&_B->axisY, _separate) * _B->lengthY) +
		fabsf(D3DXVec3Dot(&_B->axisZ, _separate) * _B->lengthZ);

	Vector3 shadowA_V = { fabsf(D3DXVec3Dot(&_A->axisX, _separate) * _A->lengthX) ,fabsf(D3DXVec3Dot(&_A->axisY, _separate) * _A->lengthY) ,fabsf(D3DXVec3Dot(&_A->axisZ, _separate) * _A->lengthZ) };
	Vector3 shadowB_V = { fabsf(D3DXVec3Dot(&_B->axisX, _separate) * _B->lengthX) ,fabsf(D3DXVec3Dot(&_B->axisY, _separate) * _B->lengthY) ,fabsf(D3DXVec3Dot(&_B->axisZ, _separate) * _B->lengthZ) };

	//�������ĂȂ�
	if (distance > shadowA + shadowB)
	{
		return false;
	}

	float tempCaveinLength = (shadowA + shadowB) - distance;//�߂荞�ݒ���

	Vector3 nomDir = { 0,0,0 };
	D3DXVec3Normalize(&nomDir, _distance);
	float tempLen = D3DXVec3Length(&nomDir);

	Vector3 tempppp = _Bcmp->GetPosition() - _Acmp->GetPosition();
	float tempBLen = D3DXVec3Length(&tempppp);//�{�b�N�X�Ԓ���

	Vector3 tempDistanceRevX = *_distance;
	Vector3 tempLastPos = _Bcmp->GetPosition() + (_Acmp->GetScale() * 0.5f) + (tempDistanceRevX * tempCaveinLength);

	return true;
}

bool Manager_OBB::Raycast(Vector3 _startPos, Vector3 _endPos,
	Actor* _ignoreMySelf, bool _isDispRay, std::list<ACTOR_TYPE>_ignoreList,
	Color _drawRayColor, int _drawRayLife)
{
	const float length = D3DXVec3Length(&(_endPos - _startPos));//�J�n�_����I���_�܂ł̋���

	float tempLen = length;
	int tempRep = 0;
	//�J��Ԃ���
	while (tempLen < 1.0f)
	{
		tempLen *= 10.0f;
		tempRep++;
	}
	const int repeat = static_cast<int>(floor(length * (10.0f * max(tempRep, 1))));//�J��Ԃ���

	const float addLen = static_cast<float>(length / repeat);//��x�̍X�V�Ői�ދ���
	Vector3 currentPos = _startPos;//���݂̒T���ʒu
	Vector3 dir;//�v���C���[����J�����̕���
	D3DXVec3Normalize(&dir, &(_endPos - _startPos));

	//�`��p�f�[�^����
	if (_isDispRay)
	{
		s_DrawData temp = {};
		temp.color = _drawRayColor;
		temp.startPos = _startPos;
		temp.endPos = _endPos;
		temp.time = 0;
		temp.timeMax = _drawRayLife;
		debugDrawRayList.emplace_back(temp);
	}

	float currentLen = 0.0f;
	for (int i = 0; i < repeat; i++)
	{
		//OBB�̃��C���[���J��Ԃ�
		for (int k = 0; k < ACTOR_TYPE::MAX_ACTOR_TYPE; k++)
		{
			//�J�������m�͔��肵�Ȃ�
			if (k == ACTOR_TYPE::COLLISION_CAMERA)continue;

			bool isIgn = false;
			//�������X�g�ƌ��݂̃��C���[���r
			for (auto& ign : _ignoreList)
			{
				if (k == ign)
				{
					isIgn = true;
					break;
				}
			}
			if (isIgn)continue;

			//OBB�̌����J��Ԃ�
			std::list<Component_OBB*>::iterator itr = m_ComponentList[k].begin();
			//for (unsigned int m = 0; m < m_ComponentListArrey[k].size(); m++)
			for (; itr != m_ComponentList[k].end(); itr++)
			{
				//���g�𖳎�����ݒ�Ȃ疳������
				if (_ignoreMySelf && _ignoreMySelf == (*itr)->GetOwner())continue;

				//OBB�ɓ_���q�b�g
				if (OverlapJudgeOBBandPoint(*itr, currentPos))
				{
					//������ł�����������TRUE
					Manager_ImGui::Instance()->GetMainDebugWindow()->DebugLog(
						(*itr)->GetOwner()->GetName() + std::string(" : ") + Time::Instance()->GetCurrentSecondString(), "", Vector4(1, 0, 0, 1));
					return true;
				}
			}
		}
		currentLen += addLen;//�X�^�[�g�n�_����T����������
		currentPos += dir * currentLen;//���݂̒T���ʒu
	}

	//������Ȃ�����
	Manager_ImGui::Instance()->GetMainDebugWindow()->DebugLog("Ray No Hit", "rayHitObjName", Vector4(1, 1, 1, 1), 1);
	return false;
}

int Manager_OBB::RaycastPM(Vector3 _startPos, Vector3 _endPos,
	Actor* _ignoreMySelf, bool _isDispRay, std::list<ACTOR_TYPE>_ignoreList,
	Color _drawRayColor, int _drawRayLife)
{
	const float length = D3DXVec3Length(&(_endPos - _startPos));//�J�n�_����I���_�܂ł̋���

	float tempLen = length;
	int tempRep = 0;
	//�J��Ԃ���
	while (tempLen < 1.0f)
	{
		tempLen *= 10.0f;
		tempRep++;
	}
	const int repeat = static_cast<int>(floor(length * (10.0f * max(tempRep, 1))));//�J��Ԃ���

	const float addLen = static_cast<float>(length / repeat);//��x�̍X�V�Ői�ދ���
	Vector3 currentPos = _startPos;//���݂̒T���ʒu
	Vector3 dir;//�v���C���[����J�����̕���
	D3DXVec3Normalize(&dir, &(_endPos - _startPos));

	//�`��p�f�[�^����
	if (_isDispRay)
	{
		s_DrawData temp = {};
		temp.color = _drawRayColor;
		temp.startPos = _startPos;
		temp.endPos = _endPos;
		temp.time = 0;
		temp.timeMax = _drawRayLife;
		debugDrawRayList.emplace_back(temp);
	}

	float currentLen = 0.0f;
	for (int i = 0; i < repeat; i++)
	{
		//OBB�̃��C���[���J��Ԃ�
		for (int k = 0; k < ACTOR_TYPE::MAX_ACTOR_TYPE; k++)
		{
			//�J�������m�͔��肵�Ȃ�
			if (k == ACTOR_TYPE::COLLISION_CAMERA)continue;

			bool isIgn = false;
			//�������X�g�ƌ��݂̃��C���[���r
			for (auto& ign : _ignoreList)
			{
				if (k == ign)
				{
					isIgn = true;
					break;
				}
			}
			if (isIgn)continue;

			//OBB�̌����J��Ԃ�
			std::list<Component_OBB*>::iterator itr = m_ComponentList[k].begin();

			for (; itr != m_ComponentList[k].end(); itr++)
			{
				//���g�𖳎�����ݒ�Ȃ疳������
				if (_ignoreMySelf && _ignoreMySelf == (*itr)->GetOwner())continue;

				//OBB�ɓ_���q�b�g
				if (OverlapJudgeOBBandPoint(*itr, currentPos))
				{
					//�����}�e���A����Ԃ�
					return (*itr)->GetPhysicsMaterial();
				}
			}
		}
		currentLen += addLen;//�X�^�[�g�n�_����T����������
		currentPos += dir * currentLen;//���݂̒T���ʒu
	}

	//������Ȃ�����
	return INVALID_ID;
}

std::vector<Manager_OBB::s_RaycastHit> Manager_OBB::RaycastGetHit(Vector3 _startPos, Vector3 _endPos,
	Actor* _ignoreMySelf, bool _isDispRay, std::list<ACTOR_TYPE>_ignoreList, Color _drawRayColor, int _drawRayLife)
{
	std::vector<s_RaycastHit>ret;

	const float length = D3DXVec3Length(&(_endPos - _startPos));//�J�n�_����I���_�܂ł̋���

	float tempLen = length;
	int tempRep = 0;
	//�J��Ԃ���
	while (tempLen < 1.0f)
	{
		tempLen *= 10.0f;
		tempRep++;
	}
	const int repeat = static_cast<int>(floor(length * (10.0f * max(tempRep, 1))));//�J��Ԃ���

	const float addLen = static_cast<float>(length / repeat);//��x�̍X�V�Ői�ދ���
	Vector3 currentPos = _startPos;//���݂̒T���ʒu
	Vector3 dir;//�v���C���[����J�����̕���
	D3DXVec3Normalize(&dir, &(_endPos - _startPos));

	//�`��p�f�[�^����
	if (_isDispRay)
	{
		s_DrawData temp = {};
		temp.color = _drawRayColor;
		temp.startPos = _startPos;
		temp.endPos = _endPos;
		temp.time = 0;
		temp.timeMax = _drawRayLife;
		debugDrawRayList.emplace_back(temp);
	}

	float currentLen = 0.0f;
	for (int i = 0; i < repeat; i++)
	{
		//OBB�̃��C���[���J��Ԃ�
		for (int k = 0; k < ACTOR_TYPE::MAX_ACTOR_TYPE; k++)
		{
			//�J�������m�͔��肵�Ȃ�
			if (k == ACTOR_TYPE::COLLISION_CAMERA)continue;

			bool isIgn = false;
			//�������X�g�ƌ��݂̃��C���[���r
			for (auto& ign : _ignoreList)
			{
				if (k == ign)
				{
					isIgn = true;
					break;
				}
			}
			if (isIgn)continue;

			auto itr = m_ComponentList[k].begin();
			//OBB�̌����J��Ԃ�
			//for (unsigned int m = 0; m < m_ComponentListArrey[k].size(); m++)
			for (; itr != m_ComponentList[k].end(); itr++)
			{
				//���g�𖳎�����ݒ�Ȃ疳������
				if (_ignoreMySelf && _ignoreMySelf == (*itr)->GetOwner())continue;

				//OBB�ɓ_���q�b�g
				if (OverlapJudgeOBBandPoint(*itr, currentPos))
				{
					//�����A�N�^�[�͓o�^���Ȃ�
					bool isContinue = false;
					for (auto& hitObjList : ret)
					{
						if (hitObjList.hitObj == (*itr)->GetOwner())
						{
							isContinue = true;
							break;
						}
					}
					if (isContinue)continue;

					//�V�K�q�b�g�A�N�^�[�o�^
					s_RaycastHit temp = {};
					temp.hitObj = (*itr)->GetOwner();
					temp.hitPos = currentPos;
					temp.length = currentLen;
					ret.emplace_back(temp);
				}
			}
		}
		currentLen += addLen;
		currentPos += dir * addLen;
	}
	return ret;
}
