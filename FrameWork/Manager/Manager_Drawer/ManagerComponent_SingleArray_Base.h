#pragma once

#include <list>

//template <class T>
//class ManagerComponent_SingleArray_Base
//{
//protected:
//
//	std::list<T*>m_ComponentList;
//
//public:
//
//	virtual ~ManagerComponent_SingleArray_Base() {}
//
//	virtual void Init() {};
//	virtual void Update() {};
//	virtual void Draw() = 0;
//	virtual void Uninit() = 0;
//
//	template <class C>
//	void AddComponent(C* _component)
//	{
//		m_ComponentList.push_back(_component);
//	}
//
//	template <class C>
//	void RemoveComponent(C* _component)
//	{
//		//m_ComponentList.remove(_component);
//
//		for (auto& i : m_ComponentList)
//		{
//			if (i == _component)
//			{
//				m_ComponentList.remove(_component);
//				return;
//			}
//		}
//	}
//};
