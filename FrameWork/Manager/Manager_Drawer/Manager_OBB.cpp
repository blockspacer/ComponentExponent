//=============================================================================
//
//  OBBコンポーネントマネージャー [Manager_OBB.cpp]
//  Date   : 2021/11/10
//
//=============================================================================

//******************************************************************************
// インクルード
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

//更新関数
void Manager_OBB::Update()
{
	//プレイヤーの判定
	for (Component_OBB* OBB_Player : m_ComponentList[COLLISION_PLAYER])
	{
		if (OBB_Player->GetActive())//有効か
		{
			//プレイヤーとプレイヤーの判定
			for (Component_OBB* OBB_PlayerB : m_ComponentList[COLLISION_PLAYER])
			{
				if (OBB_PlayerB->GetActive() && !(OBB_Player == OBB_PlayerB))//有効かつ自身と異なるプレイヤー
				{
					//todo:
				}
			}
			//プレイヤーとエネミーの判定
			for (Component_OBB* OBB_Enemy : m_ComponentList[COLLISION_ENEMY])
			{
				if (OBB_Enemy->GetActive())//有効か
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

	//弾の判定
	for (Component_OBB* OBB_Bullet : m_ComponentList[COLLISION_BULLET])
	{
		if (OBB_Bullet->GetActive())//有効か
		{
			//弾とエネミーの判定
			for (Component_OBB* OBB_Enemy : m_ComponentList[COLLISION_ENEMY])
			{
				if (OBB_Enemy->GetActive())//有効か
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

	//エネミーの判定
	for (Component_OBB* OBB_Enemy : m_ComponentList[COLLISION_ENEMY])
	{
		if (OBB_Enemy->GetActive())//有効か
		{
			//エネミーとエネミーの判定
			for (Component_OBB* OBB_EnemyB : m_ComponentList[COLLISION_ENEMY])
			{
				if (OBB_EnemyB->GetActive() && !(OBB_Enemy == OBB_EnemyB))//有効かつ自身と異なるエネミー
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

	//アイテムの判定
	for (auto& i : m_ComponentList[COLLISION_ITEM])
	{
		if (i->GetActive())
		{
			//キャラクターとの判定
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

//描画関数
void Manager_OBB::Draw()
{
	for (int i = 0; i < MAX_ACTOR_TYPE; i++)//コリジョンコンポーネントリストのクリア
	{
		for (Component_OBB* component_OBB : m_ComponentList[i])//描画
		{
			component_OBB->Draw();
		}
	}

	for (auto& i : debugDrawRayList)
	{
		Renderer_Polygon::Instance()->DrawLine(i.startPos, i.endPos, i.color);
	}
}

//終了関数
void Manager_OBB::Uninit()
{
	for (int i = 0; i < MAX_ACTOR_TYPE; i++)//コリジョンコンポーネントリストのクリア
	{
		m_ComponentList[i].clear(); // CCリストのクリア
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

//OBBとOBBのオーバーラップ判定
bool Manager_OBB::OverlapJudgeOBB(Component_OBB* _component_OBBA , Component_OBB* _component_OBBB)
{
	//ボックス同士の距離
	Vector3 distance = _component_OBBB->GetPosition() - _component_OBBA->GetPosition();

	//比較用の構造体
	s_OBBCompData A = {};
	s_OBBCompData B = {};

	A.lengthX = _component_OBBA->GetLengthX();
	A.lengthY = _component_OBBA->GetLengthY();
	A.lengthZ = _component_OBBA->GetLengthZ();
	B.lengthX = _component_OBBB->GetLengthX();
	B.lengthY = _component_OBBB->GetLengthY();
	B.lengthZ = _component_OBBB->GetLengthZ();

	//ローカル基底軸ベクトルにそれぞれの回転を反映させる
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

	//ボックスA基準のローカル基底軸基準の影算出
	if (!CompareLength(&A, &B, &A.axisX, &distance))return false;//Xを分離軸に
	if (!CompareLength(&A, &B, &A.axisY, &distance))return false;//Yを分離軸に
	if (!CompareLength(&A, &B, &A.axisZ, &distance))return false;//Zを分離軸に

	//ボックスB基準のローカル基底軸基準の影算出
	if (!CompareLength(&A, &B, &B.axisX, &distance))return false;//Xを分離軸に
	if (!CompareLength(&A, &B, &B.axisY, &distance))return false;//Yを分離軸に
	if (!CompareLength(&A, &B, &B.axisZ, &distance))return false;//Zを分離軸に

	Vector3 separate = {};
	//お互いの基底軸同士の外戚ベクトル基準の影算出
	{
		//ボックスAのX
		{
			//と　ボックスBのX軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisX, &B.axisX);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//と　ボックスBのY軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisX, &B.axisY);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//と　ボックスBのZ軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisX, &B.axisZ);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
		}
		//ボックスAY
		{
			//と　ボックスBのX軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisY, &B.axisX);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//と　ボックスBのY軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisY, &B.axisY);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//と　ボックスBのZ軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisY, &B.axisZ);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
		}
		//ボックスAZ
		{
			//と　ボックスBのX軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisX);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//と　ボックスBのY軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisY);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//と　ボックスBのZ軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisZ);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
		}
	}

	return true;
}

bool Manager_OBB::BlockJudgeOBB(Component_OBB * _component_OBBA, Component_OBB * _component_OBBB)
{
	//ボックス同士の距離
	Vector3 distance = _component_OBBB->GetPosition() - _component_OBBA->GetPosition();

	//比較用の構造体
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

	//ローカル基底軸ベクトルにそれぞれの回転を反映させる
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

	//ボックスA基準のローカル基底軸基準の影算出
	if (!CompareLengthAndGetCaveinLength(&A, &B, &A.axisX, &distance, Acomp, Bcomp))return false;//Xを分離軸に
	if (!CompareLengthAndGetCaveinLength(&A, &B, &A.axisY, &distance, Acomp, Bcomp))return false;//Yを分離軸に
	if (!CompareLengthAndGetCaveinLength(&A, &B, &A.axisZ, &distance, Acomp, Bcomp))return false;//Zを分離軸に

	//ボックスB基準のローカル基底軸基準の影算出
	if (!CompareLengthAndGetCaveinLength(&A, &B, &B.axisX, &distance, Acomp, Bcomp))return false;//Xを分離軸に
	if (!CompareLengthAndGetCaveinLength(&A, &B, &B.axisY, &distance, Acomp, Bcomp))return false;//Yを分離軸に
	if (!CompareLengthAndGetCaveinLength(&A, &B, &B.axisZ, &distance, Acomp, Bcomp))return false;//Zを分離軸に

	Vector3 separate = {};
	//お互いの基底軸同士の外戚ベクトル基準の影算出
	{
		//ボックスAのX
		{
			//と　ボックスBのX軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisX, &B.axisX);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
			//と　ボックスBのY軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisX, &B.axisY);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
			//と　ボックスBのZ軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisX, &B.axisZ);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
		}
		//ボックスAY
		{
			//と　ボックスBのX軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisY, &B.axisX);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
			//と　ボックスBのY軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisY, &B.axisY);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
			//と　ボックスBのZ軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisY, &B.axisZ);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
		}
		//ボックスAZ
		{
			//と　ボックスBのX軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisX);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
			//と　ボックスBのY軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisY);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
			//と　ボックスBのZ軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisZ);
			if (!CompareLengthAndGetCaveinLength(&A, &B, &separate, &distance, Acomp, Bcomp)) return false;
		}
	}

	return true;
}

bool Manager_OBB::OverlapJudgeOBBandPoint(Component_OBB * _component_OBB, Vector3 _point)
{
	//ボックス同士の距離
	Vector3 distance = _component_OBB->GetPosition() - _point;

	//比較用の構造体
	s_OBBCompData A = {};
	s_OBBCompData B = {};

	A.lengthX = 0.0f;
	A.lengthY = 0.0f;
	A.lengthZ = 0.0f;
	B.lengthX = _component_OBB->GetLengthX();
	B.lengthY = _component_OBB->GetLengthY();
	B.lengthZ = _component_OBB->GetLengthZ();

	//ローカル基底軸ベクトルにそれぞれの回転を反映させる
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

	//ボックスA基準のローカル基底軸基準の影算出
	if (!CompareLength(&A, &B, &A.axisX, &distance))return false;//Xを分離軸に
	if (!CompareLength(&A, &B, &A.axisY, &distance))return false;//Yを分離軸に
	if (!CompareLength(&A, &B, &A.axisZ, &distance))return false;//Zを分離軸に

	//ボックスB基準のローカル基底軸基準の影算出
	if (!CompareLength(&A, &B, &B.axisX, &distance))return false;//Xを分離軸に
	if (!CompareLength(&A, &B, &B.axisY, &distance))return false;//Yを分離軸に
	if (!CompareLength(&A, &B, &B.axisZ, &distance))return false;//Zを分離軸に

	Vector3 separate = {};
	//お互いの基底軸同士の外戚ベクトル基準の影算出
	{
		//ボックスAのX
		{
			//と　ボックスBのX軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisX, &B.axisX);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//と　ボックスBのY軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisX, &B.axisY);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//と　ボックスBのZ軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisX, &B.axisZ);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
		}
		//ボックスAY
		{
			//と　ボックスBのX軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisY, &B.axisX);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//と　ボックスBのY軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisY, &B.axisY);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//と　ボックスBのZ軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisY, &B.axisZ);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
		}
		//ボックスAZ
		{
			//と　ボックスBのX軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisX);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//と　ボックスBのY軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisY);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
			//と　ボックスBのZ軸との外積ベクトルを分離軸とした場合
			D3DXVec3Cross(&separate, &A.axisZ, &B.axisZ);
			if (!CompareLength(&A, &B, &separate, &distance)) return false;
		}
	}

	return true;
}

bool Manager_OBB::CompareLength(s_OBBCompData * _A, s_OBBCompData * _B, Vector3 * _separate, Vector3 * _distance)
{
	//分離軸上でのボックスAの中心からボックスBの中心までの座標
	float distance = fabsf(D3DXVec3Dot(_distance, _separate));
	//分離軸上でボックスAの中心から最も遠いボックスAの頂点までの距離
	float shadowA = 0.0f;
	//分離軸上でボックスBの中心から最も遠いボックスBの頂点までの距離
	float shadowB = 0.0f;
	//それぞれのボックスの影を算出
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
	//分離軸上でのボックスAの中心からボックスBの中心までの座標
	float distance = fabsf(D3DXVec3Dot(_distance, _separate));
	//分離軸上でボックスAの中心から最も遠いボックスAの頂点までの距離
	float shadowA = 0.0f;
	//分離軸上でボックスBの中心から最も遠いボックスBの頂点までの距離
	float shadowB = 0.0f;
	//それぞれのボックスの影を算出
	shadowA = fabsf(D3DXVec3Dot(&_A->axisX, _separate) * _A->lengthX) +
		fabsf(D3DXVec3Dot(&_A->axisY, _separate) * _A->lengthY) +
		fabsf(D3DXVec3Dot(&_A->axisZ, _separate) * _A->lengthZ);

	shadowB = fabsf(D3DXVec3Dot(&_B->axisX, _separate) * _B->lengthX) +
		fabsf(D3DXVec3Dot(&_B->axisY, _separate) * _B->lengthY) +
		fabsf(D3DXVec3Dot(&_B->axisZ, _separate) * _B->lengthZ);

	Vector3 shadowA_V = { fabsf(D3DXVec3Dot(&_A->axisX, _separate) * _A->lengthX) ,fabsf(D3DXVec3Dot(&_A->axisY, _separate) * _A->lengthY) ,fabsf(D3DXVec3Dot(&_A->axisZ, _separate) * _A->lengthZ) };
	Vector3 shadowB_V = { fabsf(D3DXVec3Dot(&_B->axisX, _separate) * _B->lengthX) ,fabsf(D3DXVec3Dot(&_B->axisY, _separate) * _B->lengthY) ,fabsf(D3DXVec3Dot(&_B->axisZ, _separate) * _B->lengthZ) };

	//当たってない
	if (distance > shadowA + shadowB)
	{
		return false;
	}

	float tempCaveinLength = (shadowA + shadowB) - distance;//めり込み長さ

	Vector3 nomDir = { 0,0,0 };
	D3DXVec3Normalize(&nomDir, _distance);
	float tempLen = D3DXVec3Length(&nomDir);

	Vector3 tempppp = _Bcmp->GetPosition() - _Acmp->GetPosition();
	float tempBLen = D3DXVec3Length(&tempppp);//ボックス間長さ

	Vector3 tempDistanceRevX = *_distance;
	Vector3 tempLastPos = _Bcmp->GetPosition() + (_Acmp->GetScale() * 0.5f) + (tempDistanceRevX * tempCaveinLength);

	return true;
}

bool Manager_OBB::Raycast(Vector3 _startPos, Vector3 _endPos,
	Actor* _ignoreMySelf, bool _isDispRay, std::list<ACTOR_TYPE>_ignoreList,
	Color _drawRayColor, int _drawRayLife)
{
	const float length = D3DXVec3Length(&(_endPos - _startPos));//開始点から終了点までの距離

	float tempLen = length;
	int tempRep = 0;
	//繰り返し回数
	while (tempLen < 1.0f)
	{
		tempLen *= 10.0f;
		tempRep++;
	}
	const int repeat = static_cast<int>(floor(length * (10.0f * max(tempRep, 1))));//繰り返し回数

	const float addLen = static_cast<float>(length / repeat);//一度の更新で進む距離
	Vector3 currentPos = _startPos;//現在の探索位置
	Vector3 dir;//プレイヤーからカメラの方向
	D3DXVec3Normalize(&dir, &(_endPos - _startPos));

	//描画用データ入力
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
		//OBBのレイヤー分繰り返す
		for (int k = 0; k < ACTOR_TYPE::MAX_ACTOR_TYPE; k++)
		{
			//カメラ同士は判定しない
			if (k == ACTOR_TYPE::COLLISION_CAMERA)continue;

			bool isIgn = false;
			//無視リストと現在のレイヤーを比較
			for (auto& ign : _ignoreList)
			{
				if (k == ign)
				{
					isIgn = true;
					break;
				}
			}
			if (isIgn)continue;

			//OBBの個数分繰り返す
			std::list<Component_OBB*>::iterator itr = m_ComponentList[k].begin();
			//for (unsigned int m = 0; m < m_ComponentListArrey[k].size(); m++)
			for (; itr != m_ComponentList[k].end(); itr++)
			{
				//自身を無視する設定なら無視する
				if (_ignoreMySelf && _ignoreMySelf == (*itr)->GetOwner())continue;

				//OBBに点がヒット
				if (OverlapJudgeOBBandPoint(*itr, currentPos))
				{
					//何か一つでもあたったらTRUE
					Manager_ImGui::Instance()->GetMainDebugWindow()->DebugLog(
						(*itr)->GetOwner()->GetName() + std::string(" : ") + Time::Instance()->GetCurrentSecondString(), "", Vector4(1, 0, 0, 1));
					return true;
				}
			}
		}
		currentLen += addLen;//スタート地点から探査した長さ
		currentPos += dir * currentLen;//現在の探索位置
	}

	//当たらなかった
	Manager_ImGui::Instance()->GetMainDebugWindow()->DebugLog("Ray No Hit", "rayHitObjName", Vector4(1, 1, 1, 1), 1);
	return false;
}

int Manager_OBB::RaycastPM(Vector3 _startPos, Vector3 _endPos,
	Actor* _ignoreMySelf, bool _isDispRay, std::list<ACTOR_TYPE>_ignoreList,
	Color _drawRayColor, int _drawRayLife)
{
	const float length = D3DXVec3Length(&(_endPos - _startPos));//開始点から終了点までの距離

	float tempLen = length;
	int tempRep = 0;
	//繰り返し回数
	while (tempLen < 1.0f)
	{
		tempLen *= 10.0f;
		tempRep++;
	}
	const int repeat = static_cast<int>(floor(length * (10.0f * max(tempRep, 1))));//繰り返し回数

	const float addLen = static_cast<float>(length / repeat);//一度の更新で進む距離
	Vector3 currentPos = _startPos;//現在の探索位置
	Vector3 dir;//プレイヤーからカメラの方向
	D3DXVec3Normalize(&dir, &(_endPos - _startPos));

	//描画用データ入力
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
		//OBBのレイヤー分繰り返す
		for (int k = 0; k < ACTOR_TYPE::MAX_ACTOR_TYPE; k++)
		{
			//カメラ同士は判定しない
			if (k == ACTOR_TYPE::COLLISION_CAMERA)continue;

			bool isIgn = false;
			//無視リストと現在のレイヤーを比較
			for (auto& ign : _ignoreList)
			{
				if (k == ign)
				{
					isIgn = true;
					break;
				}
			}
			if (isIgn)continue;

			//OBBの個数分繰り返す
			std::list<Component_OBB*>::iterator itr = m_ComponentList[k].begin();

			for (; itr != m_ComponentList[k].end(); itr++)
			{
				//自身を無視する設定なら無視する
				if (_ignoreMySelf && _ignoreMySelf == (*itr)->GetOwner())continue;

				//OBBに点がヒット
				if (OverlapJudgeOBBandPoint(*itr, currentPos))
				{
					//物理マテリアルを返す
					return (*itr)->GetPhysicsMaterial();
				}
			}
		}
		currentLen += addLen;//スタート地点から探査した長さ
		currentPos += dir * currentLen;//現在の探索位置
	}

	//当たらなかった
	return INVALID_ID;
}

std::vector<Manager_OBB::s_RaycastHit> Manager_OBB::RaycastGetHit(Vector3 _startPos, Vector3 _endPos,
	Actor* _ignoreMySelf, bool _isDispRay, std::list<ACTOR_TYPE>_ignoreList, Color _drawRayColor, int _drawRayLife)
{
	std::vector<s_RaycastHit>ret;

	const float length = D3DXVec3Length(&(_endPos - _startPos));//開始点から終了点までの距離

	float tempLen = length;
	int tempRep = 0;
	//繰り返し回数
	while (tempLen < 1.0f)
	{
		tempLen *= 10.0f;
		tempRep++;
	}
	const int repeat = static_cast<int>(floor(length * (10.0f * max(tempRep, 1))));//繰り返し回数

	const float addLen = static_cast<float>(length / repeat);//一度の更新で進む距離
	Vector3 currentPos = _startPos;//現在の探索位置
	Vector3 dir;//プレイヤーからカメラの方向
	D3DXVec3Normalize(&dir, &(_endPos - _startPos));

	//描画用データ入力
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
		//OBBのレイヤー分繰り返す
		for (int k = 0; k < ACTOR_TYPE::MAX_ACTOR_TYPE; k++)
		{
			//カメラ同士は判定しない
			if (k == ACTOR_TYPE::COLLISION_CAMERA)continue;

			bool isIgn = false;
			//無視リストと現在のレイヤーを比較
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
			//OBBの個数分繰り返す
			//for (unsigned int m = 0; m < m_ComponentListArrey[k].size(); m++)
			for (; itr != m_ComponentList[k].end(); itr++)
			{
				//自身を無視する設定なら無視する
				if (_ignoreMySelf && _ignoreMySelf == (*itr)->GetOwner())continue;

				//OBBに点がヒット
				if (OverlapJudgeOBBandPoint(*itr, currentPos))
				{
					//同じアクターは登録しない
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

					//新規ヒットアクター登録
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
