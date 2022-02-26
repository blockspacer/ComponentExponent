//=============================================================================
//
//  Rigidbodyマネージャー [Manager_Rigidbody.h]
//  Date   : 2021/11/22
//
//=============================================================================
#pragma once

#include <unordered_map>
#include "../../Project/Bullet/src/btBulletDynamicsCommon.h"
#include "../../Renderer/Renderer_Polygon.h"
#include "../../System/Main.h"

#if defined(_DEBUG)
#pragma comment(lib, "Bullet/bin/BulletCollision_Debug.lib")
#pragma comment(lib, "Bullet/bin/BulletDynamics_Debug.lib")
#pragma comment(lib, "Bullet/bin/LinearMath_Debug.lib")
#else
#pragma comment(lib, "Bullet/bin/BulletCollision.lib")
#pragma comment(lib, "Bullet/bin/BulletDynamics.lib")
#pragma comment(lib, "Bullet/bin/LinearMath.lib")
#endif

//******************************************************************************
// クラス定義
//******************************************************************************
class Manager_Rigidbody
{

public:

	//リジッドボディ構造体
	typedef struct 
	{

		btMotionState* state; //状態
		btRigidBody*   body;//実体

	} s_RigidBody;

	void Init();//初期化
	void Update();//更新
	void Draw();//描画
	void Uninit();//終了

	//スフィアリジッドボディ追加
	s_RigidBody* AddSphere(const Vector3& _scale, const Vector3& _position, float _mass);
	//ボックスリジッドボディ追加
	s_RigidBody* AddBox(const Vector3& _scale, const Vector3& _position, float _mass);
	//カプセルリジッドボディ追加
	s_RigidBody* AddCapsule(float _radius, float _height, const Vector3& _position, const Vector3& _rotation, float _mass);
	//プレーンリジッドボディ追加
	s_RigidBody* AddStaticPlane(const Vector3& _up, const Vector3& _position, float _mass);
	//シリンダー追加
	s_RigidBody* AddCylinder(const Vector3& _pos, const Vector3& _scale, const Vector3& _rot, float _mass);

	//Bulletワールド取得
	inline btDynamicsWorld* GebtDynamicsWorld() { return m_DynamicsWorld; }

	//リジッドボディを除去する
	void RemoveRigidBody(btRigidBody* _rigidBody);

private:

	btDynamicsWorld*   m_DynamicsWorld = 0;//Bulletワールド
	class MyDebugDraw* m_MyDebugDraw;      //デバッグ描画クラス

	//リジッドボディ追加
	s_RigidBody* AddRigidBody(btCollisionShape* _shape, btScalar _mass, const btTransform& _transform);

};

// デバッグ描画クラス
class MyDebugDraw : public btIDebugDraw
{
	int Debug_mode;//デバッグモード

	void drawLine(const btVector3& _from, const btVector3& _to,
		const btVector3& _color);

	void drawLine(const btVector3& _from, const btVector3& _to,
		const btVector3& _fromColor, const btVector3& _toColor);


	/* PointOnBに、衝突点を描画 */
	void drawContactPoint(const btVector3& _PointOnB, const btVector3& _normalOnB,
		btScalar _distance, int _lifeTime,
		const btVector3& _color){}


	/* 警告表示 */
	void reportErrorWarning(const char* _warningString){}

	/* 指定空間座標に文字列表示 */
	void draw3dText(const btVector3& _location, const char* _textString){}

public:

	/* デバッグモード指定 */
	inline void setDebugMode(int _debugMode){Debug_mode = _debugMode;}

	/* 現在のデバッグモードを返却 */
	inline int getDebugMode() const override{return Debug_mode;}


};