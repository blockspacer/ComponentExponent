//=============================================================================
//
//  OBBコンポーネントマネージャー [Manager_OBB.h]
//  Date   : 2021/11/10
//
//=============================================================================
#pragma once

#include <unordered_map>
#include "../../System/Main.h"
#include "ManagerComponent_MultiArray_Base.h"

//コリジョンタイプ
enum ACTOR_TYPE
{
	COLLISION_PLAYER,//プレイヤー
	COLLISION_ENEMY,//エネミー
	COLLISION_BULLET,//弾
	COLLISION_BLOCK,//ブロック
	COLLISION_ITEM,//アイテム
	COLLISION_CAMERA,//カメラ
	MAX_ACTOR_TYPE,//アクタータイプ数
};

//物理マテリアル
enum PHYSICS_MATERIAL
{
	PM_CONCRETE,//コンクリート
	PM_METAL,   //金属
	PM_SAND,    //砂
	MAX_PM_TYPE,//物理マテリアル最大数
};

//******************************************************************************
// クラス定義
//******************************************************************************
class Manager_OBB
{
public:

	struct s_ConpareOBBInfo
	{
		float caveinLength = 0.0f;
		Vector3 caveinDir = { 0,0,0 };
		bool isHit = false;
	};

	struct s_RaycastHit
	{
		float length = 0.0f;
		class Actor* hitObj = nullptr;
		Vector3 hitPos = Vector3_Zero;
	};

private:

	struct s_OBBCompData
	{
		Vector3 axisX = { 1,0,0 };
		Vector3 axisY = { 0,1,0 };
		Vector3 axisZ = { 0,0,1 };
		float lengthX = 0.0f;
		float lengthY = 0.0f;
		float lengthZ = 0.0f;
	};

	std::list<class Component_OBB*>m_ComponentList[ACTOR_TYPE::MAX_ACTOR_TYPE];

	bool CompareLength(s_OBBCompData* _A, s_OBBCompData* _B, Vector3* _separate, Vector3* _distance);

	bool CompareLengthAndGetCaveinLength(s_OBBCompData * _A,
		s_OBBCompData * _B, Vector3 * _separate, Vector3 * _distance, 
		class Component_OBB* _Acmp, class Component_OBB* _Bcmp);

	struct s_DrawData
	{
		Vector3 startPos;
		Vector3 endPos;
		Color color;
		int time;
		int timeMax;
	};

	std::list<s_DrawData>debugDrawRayList;

public:

	void Update();//更新
	void Draw();  //描画
	void Uninit();//終了

	void AddComponent(class Component_OBB* _add,int _layer)
	{
		m_ComponentList[_layer].push_back(_add);
	}

	void RemoveComponent(class Component_OBB* _del);

	//OBBのオーバーラップ判定
	bool OverlapJudgeOBB(Component_OBB* _component_OBBA, Component_OBB* _component_OBBB);
	//OBBブロック判定
	bool BlockJudgeOBB(Component_OBB* _component_OBBA, Component_OBB* _component_OBBB);
	//OBBと点の判定
	bool OverlapJudgeOBBandPoint(Component_OBB* _component_OBB, Vector3 _point);

	//レイキャスト
	//_ignoreMySelfは無視したい自分自身のActor*。_ignoreListは無視したいアクタータイプ。list。
	bool Raycast(Vector3 _startPos, Vector3 _endPos,
		Actor* _ignoreMySelf = nullptr, bool _isDispRay = false, std::list<ACTOR_TYPE>_ignoreList = std::list<ACTOR_TYPE>(),
		 Color _drawRayColor = Color(1, 0, 0, 1), int _drawRayLife = 60 * 3);

	//物理マテリアルを返すレイキャスト
    //_ignoreMySelfは無視したい自分自身のActor*。_ignoreListは無視したいアクタータイプ。list。
	int RaycastPM(Vector3 _startPos, Vector3 _endPos,
		Actor* _ignoreMySelf = nullptr, bool _isDispRay = false, std::list<ACTOR_TYPE>_ignoreList = std::list<ACTOR_TYPE>(),
		Color _drawRayColor = Color(1, 0, 0, 1), int _drawRayLife = 60 * 3);

	//_ignoreMySelfは無視したい自分自身のActor*。_ignoreListは無視したいアクタータイプ。list。
	std::vector<s_RaycastHit> RaycastGetHit(Vector3 _startPos, Vector3 _endPos, 
		Actor* _ignoreMySelf = nullptr, bool _isDispRay = false, std::list<ACTOR_TYPE>_ignoreList = std::list<ACTOR_TYPE>(),
		 Color _drawRayColor = Color(1, 0, 0, 1), int _drawRayLife = 60 * 3);
};

