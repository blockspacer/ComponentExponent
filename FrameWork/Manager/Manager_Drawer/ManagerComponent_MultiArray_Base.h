#pragma once

#include <list>
#include <vector>

//template <class T, int Max>
//class ManagerComponent_MultiArrey_Base
//{
//protected:
//
//	std::vector<std::list<T*>>m_ComponentListArrey;
//
//public:
//
//	ManagerComponent_MultiArrey_Base()
//	{
//		//最初に配列のサイズを確保しないとだめ。
//		m_ComponentListArrey.resize(Max);
//	}
//	virtual ~ManagerComponent_MultiArrey_Base() {}
//
//	virtual void Init() {};
//	virtual void Update() {}
//	virtual void Draw() = 0;
//	virtual void Uninit() = 0;
//
//	template <class C>
//	void AddComponent(C* _component, int _type)
//	{
//		if (_type > Max)return;
//		m_ComponentListArrey[_type].push_back(_component);
//	}
//
//	template <class C>
//	void RemoveComponent(C* _component)
//	{
//		for (int i = 0; i < Max; i++)
//		{
//			auto itr = m_ComponentListArrey[i].begin();
//			for (; itr != m_ComponentListArrey[i].end(); itr++)
//			{
//				if (*itr == _component)
//				{
//					m_ComponentListArrey[i].erase(itr);
//					//delete _component;
//					//_component = nullptr;
//					return;
//				}
//			}
//
//			//m_ComponentListArrey[i].remove(_component);
//		}
//	}
//
//};