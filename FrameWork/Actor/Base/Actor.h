//=============================================================================
//
//  �A�N�^�[�N���X [Actor.h]
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

	//�R���|�[�l���g�ǉ��֐�
	template <typename T>
	T* AddComponent(int _updatePriority)
	{
		T* component = new T(this, _updatePriority);
		component->Init();
		m_Components.push_back(component);
		return component;
	}

	//�R���|�[�l���g�P�̎擾�֐�
	template<typename T>
	T* GetComponentByClass()
	{
		for (Component* component : m_Components)
		{
			if (typeid(*component) == typeid(T))
			{//�^�𒲂ׂ�(RTTI���I�^���)
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


	virtual void Init() = 0;//������
	virtual void Uninit();//�I��
	virtual void Update();//�X�V

	inline void SetName(const char* _name) { name = _name; }//���O�ݒ�
	inline void SetTag(const char* _tag)   { tag  = _tag;  }//�^�O�ݒ�

	inline const char* GetName() const { return name.c_str(); }//���O�擾
	inline const char* GetTag()  const { return tag.c_str();  }//�^�O�擾

	//�j��t���O�ݒ�
	inline void SetDestroy() { m_Destroy = true; }
	inline bool GetIsDestroy()const { return m_Destroy; }

	bool SaveComponents(const char* _fileName);
	bool LoadComponents(const char* _fileName);

    //�j��֐�
	bool DestroyActor();

	inline void SetIsActive(bool _set) { m_IsActive = _set; }
	inline bool GetIsActive() const { return m_IsActive; }

private:

	//�j��t���O
	bool m_Destroy;
	bool m_IsActive;

	std::vector<class Component*> m_Components;
	std::string tag;
	std::string name;
};