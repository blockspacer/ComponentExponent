//=============================================================================
//
//  カメラ制御 [Camera.h]
//  Date   : 2021/11/20
//
//=============================================================================
#pragma once

#include "Base/Actor3D.h"

constexpr float VIEW_PITCH_MAX = DegToRad(89.9f);//最大ピッチ

constexpr float MAX_TARGET_ARM_LENGTH = 2.0f;//最大カメラ距離
constexpr float MIN_TARGET_ARM_LENGTH = 0.5f;//最小カメラ距離

constexpr float MAX_OFFSET_POSITION_Y = 0.3f;//最大カメラオフセットY座標
constexpr float MIN_OFFSET_POSITION_Y = 0.0f;//最小カメラオフセットY座標


//******************************************************************************
// クラス定義
//******************************************************************************
class Camera : public Actor3D
{
private:

	//注視点対象3Dアクター
	class Actor3D* m_Owner;

	//OBBコンポーネント
	class Component_OBB* m_Component_OBB;

	//座標オフセット
	Vector3 m_Offset_Position;

	//注視点
	Vector3     m_Target;

	//ビューマトリクス
	D3DXMATRIX  m_ViewMatrix;
	//プロジェクションマトリクス
	D3DXMATRIX  m_ProjectionMatrix;

	//視野
	float       m_FOV;

	//カメラ距離
	float       m_TargetArmLength;

	//進行方向を向くかどうか
	bool        m_OrientRotationtoMovement;

	//プレイヤーとの当たり判定を取るか
	bool m_IsUseRayHit;

public:

	//初期化・更新・終了
	void Init()override;
	void Uninit()override;
	void Update()override;

	//注視点対象3Dアクター設定
	inline void SetOwner(Actor3D* _owner) { m_Owner = _owner; }

	inline D3DXMATRIX GetViewMatrix() const { return m_ViewMatrix; }
	inline D3DXMATRIX GetProjectionMatrix() const { return m_ProjectionMatrix; }

	//視点設定関数
	void SetView();

	//ターゲットまでの距離
	inline void  SetTargetLength(float _length) { m_TargetArmLength = _length;}//代入
	inline float GetTargetLength()const        { return m_TargetArmLength;  }//取得
	//ターゲット距離加算関数
	void AddTargetLength(float _length);

	// 視錐台カリング関数
	bool CheckView(Vector3 _position, Vector3 _scale);

	//進行方向を向くかどうか取得
	inline bool  GetOrientRotationtoMovement()const { return m_OrientRotationtoMovement;}

	//進行方向を向くかどうか変更
	inline void  SetOrientRotationtoMovement(bool _flag) { m_OrientRotationtoMovement = _flag;}

	//移動
	void Move(float _forward, float _right);
	//カメラ回転
	void AddCameraYaw  (float _value); //Yaw加算
	void AddCameraPitch(float _value);//Pitch加算

	//レイヒット切り替え
	inline void SetIsUseRayHit(bool _set) { m_IsUseRayHit = _set; }
	inline bool GetIsUseRayhit()const { return m_IsUseRayHit; }

};