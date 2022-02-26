//=============================================================================
//
//  3Dアクタークラス [Actor3D.h]
//  Date   : 2021/11/5
//
//=============================================================================
#pragma once


#include "../../Base/Actor.h"

class Actor3D : public Actor
{
protected:

	Vector3     m_Position;//座標
	Vector3     m_Rotation;//回転
	Vector3     m_Scale;   //スケール

	D3DXMATRIX  m_Matrix;  //マトリクス

public:

	virtual ~Actor3D() {}

	virtual void Init()override;//初期化
	virtual void Uninit()override;//終了
	virtual void Update()override;//更新

	inline Vector3    GetPosition() const { return m_Position;}//3次元座標取得
	inline Vector3    GetRotation() const { return m_Rotation;}//3次元回転取得
	inline Vector3    GetScale()    const { return m_Scale;}  //3次元スケール取得
	inline D3DXMATRIX GetMatrix()   const { return m_Matrix;}//マトリクス取得

	inline void SetPosition(Vector3 _position) { m_Position = _position; }//3次元座標設定
	inline void SetRotation(Vector3 _rotation) { m_Rotation = _rotation; }//3次元回転設定
	inline void SetScale  (Vector3 _scale)     { m_Scale    = _scale; }   //3次元スケール設定
	inline void SetMatrix (D3DXMATRIX& _matrix){ m_Matrix   = _matrix; }  //マトリクス設定

	inline Vector3 GetForward()//前方向ベクトル取得
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Rotation.y, m_Rotation.x, m_Rotation.z);

		Vector3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}
	inline Vector3 GetRight()//横方向ベクトル取得
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Rotation.y, m_Rotation.x, m_Rotation.z);

		Vector3 Right;
		Right.x = rot._11;
		Right.y = rot._12;
		Right.z = rot._13;

		return Right;
	}
	inline Vector3 GetUp()//上方向ベクトル取得
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Rotation.y, m_Rotation.x, m_Rotation.z);

		Vector3 Up;
		Up.x = rot._21;
		Up.y = rot._22;
		Up.z = rot._23;

		return Up;
	}

};