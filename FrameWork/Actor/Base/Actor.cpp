//=============================================================================
//
//  �A�N�^�[�N���X [Actor.h]
//  Date   : 2021/11/1
//
//=============================================================================


#include "Actor.h"
#include "../../Component/Base/Component.h"
#include "../../System/DataSaver.h"


void Actor::Update()//�X�V
{
	for (class Component* component : m_Components)//���L�R���|�[�l���g�X�V
	{
		component->Update();
	}
}

void Actor::Uninit()//�I��
{
	for (class Component* component : m_Components)
	{
		component->Uninit();
		delete component;
		component = nullptr;
	}

	m_Components.clear(); //�R���|�[�l���g���X�g�̃N���A
}


bool Actor::SaveComponents(const char* _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;//���I�[�v��

	for (auto& i : m_Components)
	{
		if (!i->SaveComponentData(_fileName))return false;
	}

	return true;
}

bool Actor::LoadComponents(const char * _fileName)
{
	if (!DataSaver::Instance()->IsOpenFile(_fileName))return false;//���I�[�v��

	for (auto& i : m_Components)
	{
		std::string aaa = typeid(&i).name();
		std::string bbb = typeid(i).name();
		if (typeid(i) != typeid(Component*))continue;

		if (!i->LoadComponentData(_fileName))return false;
	}

	return true;
}

//���g��j��
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
