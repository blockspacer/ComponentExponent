//=============================================================================
//
//  描画マネージャー [Manager_Drawer.h]
//  Date   : 2021/11/03
//
//=============================================================================
#pragma once

#include <unordered_map>

//******************************************************************************
// クラス定義
//******************************************************************************
class Manager_Drawer
{
private:

	class Manager_Landscape*       m_Manager_Landscape;    //ランドスケープマネージャー
	class Manager_Cube*            m_Manager_Cube;         //キューブマネージャー
	class Manager_Model*           m_Manager_Model;        //モデルマネージャー
	class Manager_Billboard*       m_Manager_Billboard;    //ビルボードマネージャー
	class Manager_Sprite*          m_Manager_Sprite;       //スプライトマネージャー
	class Manager_Fade*            m_Manager_Fade;         //フェードマネージャー
	class Manager_OBB*             m_Manager_OBB;          //OBBマネージャー
	class Manager_Rigidbody*       m_Manager_Rigidbody;    //Rigidbodyマネージャー
	class HUD3D*                   m_HUD3D;				   //使用HUD3D
	class HUD*                     m_HUD;                  //使用HUD
	class Manager_Font2D*		   m_Manager_Font2D;       //フォントマネージャー2Dキャッシュ用
	class Manager_Font3D*		   m_Manager_Font3D;       //フォントマネージャー3D

	bool m_DebugDraw;//デバッグ表示フラグ
	bool m_DebugDrawGrid;

public:

	void Init();  //初期化
	void UpdateBefore();//アクター更新前の更新
	void UpdateAfter(); //アクター更新後の更新
	void Draw();  //描画
	void Uninit();//終了

	void AddManager_Landscape();    //ランドスケープマネージャー追加
	void AddManager_Cube();         //キューブマネージャー追加
	void AddManager_Model();        //モデルマネージャー追加
	void AddManager_Billboard();    //ビルボードマネージャー追加
	void AddManager_Sprite();       //スプライトマネージャー追加
	void AddManager_OBB();          //OBBマネージャー追加
	void AddManager_Rigidbody();    //Rigidbodyマネージャー追加
	void AddManager_Font3D();	    //フォント3Dマネージャー追加


	inline class Manager_Landscape*     GetManager_Landscape()     const { return m_Manager_Landscape; }    //ランドスケープマネージャー取得
	inline class Manager_Cube*          GetManager_Cube()          const { return m_Manager_Cube; }         //キューブマネージャー取得
	inline class Manager_Model*         GetManager_Model()         const { return m_Manager_Model; }        //モデルマネージャー取得
	inline class Manager_Billboard*     GetManager_Billboard()     const { return m_Manager_Billboard; }    //ビルボードマネージャー取得
	inline class Manager_Sprite*        GetManager_Sprite()        const { return m_Manager_Sprite; }       //スプライトマネージャー取得
	inline class Manager_Fade*          GetManager_Fade()          const { return m_Manager_Fade; }         //フェードマネージャー取得
	inline class Manager_OBB*           GetManager_OBB()           const { return m_Manager_OBB; }          //OBBマネージャー取得
	inline class Manager_Rigidbody*     GetManager_Rigidbody()     const { return m_Manager_Rigidbody; }    //Rigidbodyマネージャー取得
	inline class Manager_Font3D*        GetManager_Font3D()        const { return m_Manager_Font3D; }       //Font3Dマネージャー取得


	template <typename T>
	void SetHUD()//HUD設定関数
	{
		//現在のHUDがある場合終了
		if (m_HUD != nullptr)
		{
			m_HUD->Uninit();//終了
			delete m_HUD;//削除
			m_HUD = nullptr;//nullptr代入
		}

		//HUD読み込み	
		if (m_HUD == nullptr)
		{
			m_HUD = new T();//HUD作成;
			m_HUD->Init();//初期化
		}
	}

};
