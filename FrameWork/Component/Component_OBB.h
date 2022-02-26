//=============================================================================
//
//  OBBコンポーネント [Component_OBB.h]
//  Author : Ryosuke Kotera
//  Date   : 2021/10/18
//
//=============================================================================
#pragma once

#include "Base/Component.h"
#include "../Manager/Manager_Drawer/Manager_OBB.h"

class Component_OBB : public Component
{
public:

	Component_OBB(class Actor* _owner, int _updatePriority) : Component(_owner, _updatePriority) {}

	void Init()   override;//初期化
	void Uninit() override;//終了
	void Update() override;//更新
	
	//描画
	void Draw();

	//視認性設定
	inline void SetVisibility(bool _visibility) { m_Visibility = _visibility; }

	//スケール設定
	inline void SetScale(Vector3 _scale) { m_Scale = _scale; }

	//スケール取得
	inline Vector3 GetScale() const { return m_Scale; };

	// 座標取得
	inline Vector3 GetPosition() const { return m_Position; };

	//回転取得
	inline Vector3 GetRotation() const { return m_Rotation; }

	//視認性取得
	inline bool GetVisibility()  const { return  m_Visibility; }

	//色設定
	inline void SetColor(Color _color) { m_Color = _color; }

	//回転セット
	inline void SetRotation(Vector3 _rot) { m_Rotation = _rot; }

	//レイに反応するかを取得・設定
	inline void SetIsReactRay(bool _set) { m_isReactRay = _set; }
	inline bool GetIsReactRay()const { return m_isReactRay; }

	//物理マテリアル取得・設定
	inline int GetPhysicsMaterial() const{ return m_PhysicsMaterial; }
	inline void SetPhysicsMaterial(int _physicsMaterial)  { m_PhysicsMaterial = _physicsMaterial; }

	//OBB
	inline Vector3 GetNormaDirectionX() const { return m_NormaDirectionX; };// 指定軸番号のX方向ベクトルを取得
	inline Vector3 GetNormaDirectionY() const { return m_NormaDirectionY; };// 指定軸番号のY方向ベクトルを取得
	inline Vector3 GetNormaDirectionZ() const { return m_NormaDirectionZ; };// 指定軸番号のZ方向ベクトルを取得

	inline float   GetLengthX()        const { return m_LengthX; }// X軸方向の長さを取得
	inline float   GetLengthY()        const { return m_LengthY; }// Y軸方向の長さを取得
	inline float   GetLengthZ()        const { return m_LengthZ; }// Z軸方向の長さを取得

	//オーナー座標設定
	void SetOwnerPosition(Vector3 _position);

	//レイキャスト
	//_ignoreMySelfは無視したい自分自身のActor*。_ignoreListは無視したいアクタータイプ。list。
	bool Raycast(Vector3 _startPos, Vector3 _endPos,
		Actor* _ignoreMySelf = nullptr, bool _isDispRay = false, std::list<ACTOR_TYPE>_ignoreList = std::list<ACTOR_TYPE>(),
		Color _drawRayColor = Color(1, 0, 0, 1), int _drawRayLife = 60 * 3);

	//物理マテリアルを返すレイキャスト
	int RaycastPM(Vector3 _startPos, Vector3 _endPos,
		Actor* _ignoreMySelf = nullptr, bool _isDispRay = false, std::list<ACTOR_TYPE>_ignoreList = std::list<ACTOR_TYPE>(),
		Color _drawRayColor = Color(1, 0, 0, 1), int _drawRayLife = 60 * 3);

	//_ignoreMySelfは無視したい自分自身のActor*。_ignoreListは無視したいアクタータイプ。list。
	std::vector<Manager_OBB::s_RaycastHit> RaycastGetHit(Vector3 _startPos, Vector3 _endPos,
		Actor* _ignoreMySelf = nullptr, bool _isDispRay = false, std::list<ACTOR_TYPE>_ignoreList = std::list<ACTOR_TYPE>(),
		Color _drawRayColor = Color(1, 0, 0, 1), int _drawRayLife = 60 * 3);

	bool SaveComponentData(const char* _fileName) override;
	bool LoadComponentData(const char* _fileName) override;

private:

	class Manager_OBB* m_Manager_OBB;//OBBマネージャーキャッシュ用

	Vector3 m_NormaDirectionX;//X方向ベクトル
	Vector3 m_NormaDirectionY;//Y方向ベクトル
	Vector3 m_NormaDirectionZ;//Z方向ベクトル

	float   m_LengthX;// X軸方向の長さ
	float   m_LengthY;// Y軸方向の長さ
	float   m_LengthZ;// Z軸方向の長さ

	Vector3 m_Position;//座標
	Vector3 m_Rotation;//回転
	Vector3 m_Scale;   //スケール

	Color   m_Color;//色

	//使用するシェーダーの種類
	int  m_ShaderType;

	//視認性
	bool m_Visibility;

	//レイに反応するか
	bool m_isReactRay;

	//物理マテリアル
	int  m_PhysicsMaterial;

};


