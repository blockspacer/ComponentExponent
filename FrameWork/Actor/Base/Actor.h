//=============================================================================
//
//  アクタークラス [Actor.h]
//  Date   : 2021/11/1
//
//=============================================================================
#pragma once


#include <vector>
#include "../../System/Main.h"
#include "../../System/Math.h"


class Actor
{
public:

	Actor() :m_Destroy(false), m_IsActive(true) {};
	virtual ~Actor() {}

	//コンポーネント追加関数
	template <typename T>
	T* AddComponent(int _updatePriority)
	{
		T* component = new T(this, _updatePriority);
		component->Init();
		m_Components.push_back(component);
		return component;
	}

	//コンポーネント単体取得関数
	template<typename T>
	T* GetComponentByClass()
	{
		for (Component* component : m_Components)
		{
			if (typeid(*component) == typeid(T))
			{//型を調べる(RTTI動的型情報)
				return (T*)component;
			}
		}
		return nullptr;
	}

	template<typename T>
	void DeleteComponent(T* _delComponent)
	{
		int cnt = 0;
		for (auto& i : m_Components)
		{
			if (i == _delComponent)
			{
				Component* temp = i;
				i->Uninit();
				m_Components.erase(m_Components.begin() + cnt);
				delete temp;
				temp = nullptr;
				return;
			}
			cnt++;
		}
	}

	class Component* GetComponentByIndex(int _index)
	{
		int cnt = 0;
		for (auto& i : m_Components)
		{
			if (cnt == _index)
			{
				return i;
			}
			cnt++;
		}
	}


	virtual void Init() = 0;//初期化
	virtual void Uninit();//終了
	virtual void Update();//更新

	inline void SetName(const char* _name) { name = _name; }//名前設定
	inline void SetTag(const char* _tag)   { tag  = _tag;  }//タグ設定

	inline const char* GetName() const { return name.c_str(); }//名前取得
	inline const char* GetTag()  const { return tag.c_str();  }//タグ取得

	//破壊フラグ設定
	inline void SetDestroy() { m_Destroy = true; }
	inline bool GetIsDestroy()const { return m_Destroy; }

	bool SaveComponents(const char* _fileName);
	bool LoadComponents(const char* _fileName);

    //破壊関数
	bool DestroyActor();

	inline void SetIsActive(bool _set) { m_IsActive = _set; }
	inline bool GetIsActive() const { return m_IsActive; }

private:

	//破壊フラグ
	bool m_Destroy;
	bool m_IsActive;

	std::vector<class Component*> m_Components;
	std::string tag;
	std::string name;
};