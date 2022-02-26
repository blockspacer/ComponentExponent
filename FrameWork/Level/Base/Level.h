//=============================================================================
//
//  ���x�� [Level.h]
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
// �N���X��`
//******************************************************************************
class Level
{
private:

	std::list<Actor*> m_Actors;//���L�A�N�^�[

	class Manager_Drawer* m_Manager_Drawer;//�`��}�l�[�W���[

	unsigned int actorUniqueCount;//�A�N�^�[�J�E���g
	char actorUniqueChar;


public:

	Level() :actorUniqueCount(0), actorUniqueChar('a') {}
	virtual ~Level() {}

	//������
	virtual void Init()
	{
		//�`��}�l�[�W���[����
		m_Manager_Drawer = new Manager_Drawer;
		m_Manager_Drawer->Init();
	}

	//�I��
	virtual void Uninit()
	{
		//���L�A�N�^�[�폜
		for (Actor* actor : m_Actors)
		{
			actor->Uninit();//�I��
			delete actor;//�폜
			actor = nullptr;//nullptr���
		}
		m_Actors.clear();//�N���A

		//�`��}�l�[�W���[�I��
		if (m_Manager_Drawer != nullptr)
		{
			m_Manager_Drawer->Uninit();//�I��
			delete m_Manager_Drawer;//�폜
			m_Manager_Drawer = nullptr;//nullptr���
		}

		Manager_ImGui::Instance()->Uninit();//���X�g��S�ăN���A
	}

	//�X�V
	virtual void Update()
	{
		//�`��}�l�[�W���[�X�V
		if (m_Manager_Drawer != nullptr)
		{
			//�A�N�^�[�X�V�O�X�V
			m_Manager_Drawer->UpdateBefore();

			//���L�A�N�^�[�X�V
			for (Actor* actor : m_Actors)
			{
				actor->Update();
			}

			//m_Actors.remove([](Actor* _p) {return _p->GetIsDestroy(); });

			//�A�N�^�[�X�V��X�V
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

	//�`��
	void Draw()
	{
		//�`��}�l�[�W���[�`��
		if (m_Manager_Drawer != nullptr)
		{
			m_Manager_Drawer->Draw();//�`��
		}

		if (Camera* camera = GetActor<Camera>())
		{//�J����������ꍇ
			camera->SetView();
		}
	}

	//******************************************************************************
	// �ǉ��֐�
	//******************************************************************************

		//�A�N�^�[�ǉ��֐�
	template<typename T>
	T* AddActor()
	{
		T* actor = new T();

		//���O��t���Ă��Ȃ��Ƃ��ׂ̈Ƀ��j�[�N�Ȗ��O�ŏ�����
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

	//�A�N�^�[�ǉ��֐�
	template<typename T>
	T* AddActor(const char* _name, const char* _tag = "Untagged")
	{
		T* actor = new T();

		actor->SetName(_name);//���O������
		actor->SetTag(_tag);//�^�O������
		m_Actors.push_back(actor);

		actor->Init();
		return actor;
	}


	//******************************************************************************
	// �擾�֐�
	//******************************************************************************

		//�A�N�^�[�P�̎擾
	template<typename T>
	T* GetActor()
	{
		for (Actor* actor : m_Actors)
		{
			if (typeid(*actor) == typeid(T))//�^�𒲂ׂ�(RTTI���I�^���)
			{
				return (T*)actor;
			}
		}
		return nullptr;
	}

	//�A�N�^�[�P�̎擾
	template<typename T>
	T* GetActorByString(const char* _name, const char* _tag)
	{
		std::string tempName(_name);
		std::string tempTag(_tag);

		for (Actor* actor : m_Actors)
		{
			if (typeid(*actor) == typeid(T))//�^�𒲂ׂ�(RTTI���I�^���)
			{
				if (actor->GetName() == tempName && actor->GetTag() == tempTag)
				{
					return (T*)actor;
				}
			}
		}
		return nullptr;
	}

	//�A�N�^�[�P�̎擾
	template<typename T>
	T* GetActorByName(const char* _name)
	{
		for (Actor* actor : m_Actors)
		{
			if (typeid(*actor) == typeid(T))//�^�𒲂ׂ�(RTTI���I�^���)
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

	//�A�N�^�[�P�̎擾
	template<typename T>
	std::vector<T*> GetActorByTag(const char* _tag)
	{
		std::vector<T*>ret;

		for (Actor* actor : m_Actors)
		{
			if (typeid(*actor) == typeid(T))//�^�𒲂ׂ�(RTTI���I�^���)
			{
				ret.push_back(actor);
			}
		}
		return ret;
	}


	//�A�N�^�[�����擾
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

	//�`��}�l�[�W���[�擾
	inline class Manager_Drawer* GetManager_Drawer() { return m_Manager_Drawer; }

	//******************************************************************************
	// �폜�֐�
	//******************************************************************************

		//�^�O�Ɩ��O��v�ō폜
	template <typename T>
	bool DeleteActorByString(T* _actor)
	{
		for (auto& i : m_Actors)
		{
			if (i == _actor)
			{
				i->SetDestroy();//�폜�t���O
				i->DestroyActor();//�R���|�[�l���g���폜
				m_Actors.remove(i);//���X�g����폜
				delete i;//�A�N�^�[���폜
				return true;
			}
		}
		return false;
	}

	//�^�O�Ɩ��O��v�ō폜
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
					i->SetDestroy();//�폜�t���O
					i->DestroyActor();//�R���|�[�l���g���폜
					Actor* temp = i;
					m_Actors.remove(i);//���X�g����폜
					delete temp;//�A�N�^�[�폜
					temp = nullptr;

					return true;
				}
			}
		}
		return false;
	}

	//******************************************************************************
	// �t�F�[�h�֐�
	//******************************************************************************
	void SetFade(float _time)
	{
		if (Manager_Fade* manager_Fade = GetManager_Drawer()->GetManager_Fade())
		{
			manager_Fade->SetFade(Manager_Fade::FADE_OUT, _time);
		}
	}

	//******************************************************************************
	// �Z�[�u/���[�h�֐�
	//******************************************************************************

	bool SaveAllActors(const char* _fileName)
	{
		//�t�@�C���I�[�v��
		//if (!DataSaver::Instance()->OpenFile(_fileName, DataSaver::SAVEDATA_TYPE::write_bin))return false;
		if (!DataSaver::Instance()->OpenFile(_fileName, DataSaver::SAVEDATA_TYPE::write_bin))return false;

		//�A�N�^�[��S���Z�[�u
		for (auto& i : m_Actors)
		{
			Actor3D* temp3d = dynamic_cast<Actor3D*>(i);
			if (!temp3d)continue;
			Vector3 tempPos = temp3d->GetPosition();
			char tempName[256] = {};
			sprintf_s(tempName, "%s_%s", i->GetName(), "pos");
			if (!DataSaver::Instance()->Save<Vector3>(_fileName, &tempPos, sizeof(Vector3), tempName))
			{
				//�t�@�C���N���[�Y
				DataSaver::Instance()->CloseFile(_fileName);
				//�Z�[�u�Ɏ��s
				return false;
			}
			Vector3 tempScale = temp3d->GetScale();
			sprintf_s(tempName, "%s_%s", i->GetName(), "scale");
			if (!DataSaver::Instance()->Save<Vector3>(_fileName, &tempScale, sizeof(Vector3), tempName))
			{
				//�t�@�C���N���[�Y
				DataSaver::Instance()->CloseFile(_fileName);
				//�Z�[�u�Ɏ��s
				return false;
			}
		}

		//�t�@�C���N���[�Y
		DataSaver::Instance()->CloseFile(_fileName);

		return true;
	}

	bool LoadAllActors(const char* _fileName)
	{
		//�t�@�C���I�[�v��
		if (!DataSaver::Instance()->OpenFile(_fileName, DataSaver::SAVEDATA_TYPE::read_bin))return false;

		//�A�N�^�[����S�����[�h
		for (auto& i : m_Actors)
		{
			Actor3D* temp3d = dynamic_cast<Actor3D*>(i);
			if (!temp3d)continue;

			char tempName[256] = {};
			sprintf_s(tempName, "%s_%s", i->GetName(), "pos");
			Vector3 tempPos = {};
			if (!DataSaver::Instance()->Load<Vector3>(_fileName, &tempPos, tempName))
			{
				//�t�@�C���N���[�Y
				DataSaver::Instance()->CloseFile(_fileName);
				//���[�h�Ɏ��s
				return false;
			}
			temp3d->SetPosition(tempPos);

			sprintf_s(tempName, "%s_%s", i->GetName(), "scale");
			Vector3 tempScale = {};
			if (!DataSaver::Instance()->Load<Vector3>(_fileName, &tempScale, tempName))
			{
				//�t�@�C���N���[�Y
				DataSaver::Instance()->CloseFile(_fileName);
				//���[�h�Ɏ��s
				return false;
			}
			temp3d->SetScale(tempScale);
		}

		//�t�@�C���N���[�Y
		DataSaver::Instance()->CloseFile(_fileName);

		return true;
	}

};


