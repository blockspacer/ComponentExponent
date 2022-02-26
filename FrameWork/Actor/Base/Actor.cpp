//=============================================================================
//
//  アクタークラス [Actor.h]
//  Date   : 2021/11/1
//
//=============================================================================


#include "Actor.h"
#include "../../Component/Base/Component.h"
#include "../../System/DataSaver.h"


void Actor::Update()//更新
{
	for (class Component* component : m_Components)//所有コンポーネント更新
	{
		component->Update();
	}
}

void Actor::Uninit()//終了
{
	for (class Component* component : m_Components)
	{
		component->Uninit();
		delete component;
		component = nullptr;
	}

	m_Components.clear(); //コンポーネントリストのクリア
}


bool Actor::SaveComponents(const char* _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;//未オープン

	for (auto& i : m_Components)
	{
		if (!i->SaveComponentData(_fileName))return false;
	}

	return true;
}

bool Actor::LoadComponents(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;//未オープン

	for (auto& i : m_Components)
	{
		std::string aaa = typeid(&i).name();
		std::string bbb = typeid(i).name();
		if (typeid(i) != typeid(Component*))continue;

		if (!i->LoadComponentData(_fileName))return false;
	}

	return true;
}

//自身を破壊
bool Actor::DestroyActor()
{
	if (m_Destroy)
	{
		Uninit();
		return true;
	}
	else
	{
		return false;
	}
}
