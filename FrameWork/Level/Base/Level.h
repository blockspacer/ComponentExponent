//=============================================================================
//
//  レベル [Level.h]
//  Date   : 2021/11/03
//
//=============================================================================
#pragma once

#include <vector>
#include <typeinfo>
#include "../../Actor/Base/Actor.h"
#include "../../Actor/3D/Camera.h"
#include "../../Manager/Manager_Drawer/Owner/Manager_Drawer.h"
#include "../../Manager/Manager_Actor/Base/Manager_Actor.h"
#include "../../Manager/Manager_Drawer/Manager_Fade.h"
#include "../../System/DataSaver.h"
#include <type_traits>
#include "../../Manager/Manager_Drawer/Manager_ImGui.h"

//******************************************************************************
// クラス定義
//******************************************************************************
class Level
{
private:

	std::list<Actor*> m_Actors;//所有アクター

	class Manager_Drawer* m_Manager_Drawer;//描画マネージャー

	unsigned int actorUniqueCount;//アクターカウント
	char actorUniqueChar;


public:

	Level() :actorUniqueCount(0), actorUniqueChar('a') {}
	virtual ~Level() {}

	//初期化
	virtual void Init()
	{
		//描画マネージャー生成
		m_Manager_Drawer = new Manager_Drawer;
		m_Manager_Drawer->Init();
	}

	//終了
	virtual void Uninit()
	{
		//所有アクター削除
		for (Actor* actor : m_Actors)
		{
			actor->Uninit();//終了
			delete actor;//削除
			actor = nullptr;//nullptr代入
		}
		m_Actors.clear();//クリア

		//描画マネージャー終了
		if (m_Manager_Drawer != nullptr)
		{
			m_Manager_Drawer->Uninit();//終了
			delete m_Manager_Drawer;//削除
			m_Manager_Drawer = nullptr;//nullptr代入
		}

		Manager_ImGui::Instance()->Uninit();//リストを全てクリア
	}

	//更新
	virtual void Update()
	{
		//描画マネージャー更新
		if (m_Manager_Drawer != nullptr)
		{
			//アクター更新前更新
			m_Manager_Drawer->UpdateBefore();

			//所有アクター更新
			for (Actor* actor : m_Actors)
			{
				actor->Update();
			}

			//m_Actors.remove([](Actor* _p) {return _p->GetIsDestroy(); });

			//アクター更新後更新
			m_Manager_Drawer->UpdateAfter();
		}

		auto itr = m_Actors.begin();
		for (; itr != m_Actors.end(); itr++)
		{
			if ((*itr)->GetIsDestroy())
			{
				if ((*itr)->DestroyActor())
				{
					delete *itr;
					*itr = nullptr;
					itr = m_Actors.erase(itr);
				}
			}
		}
	}

	//描画
	void Draw()
	{
		//描画マネージャー描画
		if (m_Manager_Drawer != nullptr)
		{
			m_Manager_Drawer->Draw();//描画
		}

		if (Camera* camera = GetActor<Camera>())
		{//カメラがある場合
			camera->SetView();
		}
	}

	//******************************************************************************
	// 追加関数
	//******************************************************************************

		//アクター追加関数
	template<typename T>
	T* AddActor()
	{
		T* actor = new T();

		//名前を付けていないときの為にユニークな名前で初期化
		{
			char tempName[256] = {};
			sprintf_s(tempName, "actor_%c_%d", actorUniqueChar, actorUniqueCount);
			actor->SetName(tempName);
			actor->SetTag("Untagged");
			m_Actors.push_back(actor);
			actorUniqueCount++;
			if (actorUniqueCount >= INT_MAX)
			{
				actorUniqueCount = 0;
				actorUniqueChar++;
			}
		}

		actor->Init();
		return actor;
	}

	//アクター追加関数
	template<typename T>
	T* AddActor(const char* _name, const char* _tag = "Untagged")
	{
		T* actor = new T();

		actor->SetName(_name);//名前をつける
		actor->SetTag(_tag);//タグをつける
		m_Actors.push_back(actor);

		actor->Init();
		return actor;
	}


	//******************************************************************************
	// 取得関数
	//******************************************************************************

		//アクター単体取得
	template<typename T>
	T* GetActor()
	{
		for (Actor* actor : m_Actors)
		{
			if (typeid(*actor) == typeid(T))//型を調べる(RTTI動的型情報)
			{
				return (T*)actor;
			}
		}
		return nullptr;
	}

	//アクター単体取得
	template<typename T>
	T* GetActorByString(const char* _name, const char* _tag)
	{
		std::string tempName(_name);
		std::string tempTag(_tag);

		for (Actor* actor : m_Actors)
		{
			if (typeid(*actor) == typeid(T))//型を調べる(RTTI動的型情報)
			{
				if (actor->GetName() == tempName && actor->GetTag() == tempTag)
				{
					return (T*)actor;
				}
			}
		}
		return nullptr;
	}

	//アクター単体取得
	template<typename T>
	T* GetActorByName(const char* _name)
	{
		for (Actor* actor : m_Actors)
		{
			if (typeid(*actor) == typeid(T))//型を調べる(RTTI動的型情報)
			{
				//if (actor->GetName() == _name)
				if (strcmp(actor->GetName(), _name) == 0)
				{
					return (T*)actor;
				}
			}
		}
		return nullptr;
	}

	//アクター単体取得
	template<typename T>
	std::vector<T*> GetActorByTag(const char* _tag)
	{
		std::vector<T*>ret;

		for (Actor* actor : m_Actors)
		{
			if (typeid(*actor) == typeid(T))//型を調べる(RTTI動的型情報)
			{
				ret.push_back(actor);
			}
		}
		return ret;
	}


	//アクター複数取得
	template<typename T>
	std::vector<T*> GetActors()
	{
		std::vector<T*> Actors;
		for (Actor* actor : m_Actors)
		{
			if (typeid(*actor) == typeid(T))
			{
				Actors.push_back((T*)actor);
			}
		}
		return Actors;
	}

	//描画マネージャー取得
	inline class Manager_Drawer* GetManager_Drawer() { return m_Manager_Drawer; }

	//******************************************************************************
	// 削除関数
	//******************************************************************************

		//タグと名前一致で削除
	template <typename T>
	bool DeleteActorByString(T* _actor)
	{
		for (auto& i : m_Actors)
		{
			if (i == _actor)
			{
				i->SetDestroy();//削除フラグ
				i->DestroyActor();//コンポーネントを削除
				m_Actors.remove(i);//リストから削除
				delete i;//アクターを削除
				return true;
			}
		}
		return false;
	}

	//タグと名前一致で削除
	template <typename T>
	bool DeleteActorByString(const char* _name, const char* _tag)
	{
		std::string tempName(_name);
		std::string tempTag(_tag);

		for (auto& i : m_Actors)
		{
			if (typeid(*i) == typeid(T))
			{
				if (i->GetName() == tempName && i->GetTag() == tempTag)
				{
					i->SetDestroy();//削除フラグ
					i->DestroyActor();//コンポーネントを削除
					Actor* temp = i;
					m_Actors.remove(i);//リストから削除
					delete temp;//アクター削除
					temp = nullptr;

					return true;
				}
			}
		}
		return false;
	}

	//******************************************************************************
	// フェード関数
	//******************************************************************************
	void SetFade(float _time)
	{
		if (Manager_Fade* manager_Fade = GetManager_Drawer()->GetManager_Fade())
		{
			manager_Fade->SetFade(Manager_Fade::FADE_OUT, _time);
		}
	}

	//******************************************************************************
	// セーブ/ロード関数
	//******************************************************************************

	bool SaveAllActors(const char* _fileName)
	{
		//ファイルオープン
		//if (!DataSaver::Instance()->OpenFile(_fileName, DataSaver::SAVEDATA_TYPE::write_bin))return false;
		if (!DataSaver::Instance()->OpenFile(_fileName, DataSaver::SAVEDATA_TYPE::write_bin))return false;

		//アクターを全部セーブ
		for (auto& i : m_Actors)
		{
			Actor3D* temp3d = dynamic_cast<Actor3D*>(i);
			if (!temp3d)continue;
			Vector3 tempPos = temp3d->GetPosition();
			char tempName[256] = {};
			sprintf_s(tempName, "%s_%s", i->GetName(), "pos");
			if (!DataSaver::Instance()->Save<Vector3>(_fileName, &tempPos, sizeof(Vector3), tempName))
			{
				//ファイルクローズ
				DataSaver::Instance()->CloseFile(_fileName);
				//セーブに失敗
				return false;
			}
			Vector3 tempScale = temp3d->GetScale();
			sprintf_s(tempName, "%s_%s", i->GetName(), "scale");
			if (!DataSaver::Instance()->Save<Vector3>(_fileName, &tempScale, sizeof(Vector3), tempName))
			{
				//ファイルクローズ
				DataSaver::Instance()->CloseFile(_fileName);
				//セーブに失敗
				return false;
			}
		}

		//ファイルクローズ
		DataSaver::Instance()->CloseFile(_fileName);

		return true;
	}

	bool LoadAllActors(const char* _fileName)
	{
		//ファイルオープン
		if (!DataSaver::Instance()->OpenFile(_fileName, DataSaver::SAVEDATA_TYPE::read_bin))return false;

		//アクター情報を全部ロード
		for (auto& i : m_Actors)
		{
			Actor3D* temp3d = dynamic_cast<Actor3D*>(i);
			if (!temp3d)continue;

			char tempName[256] = {};
			sprintf_s(tempName, "%s_%s", i->GetName(), "pos");
			Vector3 tempPos = {};
			if (!DataSaver::Instance()->Load<Vector3>(_fileName, &tempPos, tempName))
			{
				//ファイルクローズ
				DataSaver::Instance()->CloseFile(_fileName);
				//ロードに失敗
				return false;
			}
			temp3d->SetPosition(tempPos);

			sprintf_s(tempName, "%s_%s", i->GetName(), "scale");
			Vector3 tempScale = {};
			if (!DataSaver::Instance()->Load<Vector3>(_fileName, &tempScale, tempName))
			{
				//ファイルクローズ
				DataSaver::Instance()->CloseFile(_fileName);
				//ロードに失敗
				return false;
			}
			temp3d->SetScale(tempScale);
		}

		//ファイルクローズ
		DataSaver::Instance()->CloseFile(_fileName);

		return true;
	}

};


