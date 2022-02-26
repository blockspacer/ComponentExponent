//=============================================================================
//
//  �R���|�[�l���g[Component.h]
//  Date   : 2021/11/1
//
//=============================================================================
#pragma once

#include "../../System/Main.h"

class Component
{
public:
	Component(class Actor* _owner, int _updatePriority) : m_Owner(_owner), m_UpdatePriority(_updatePriority) {}

	virtual void Init()   = 0;//������
	virtual void Uninit() = 0;//�I��
	virtual void Update() = 0;//�X�V

	int GetPriority()const  { return m_UpdatePriority; }//�D�揇�擾

	inline void SetActive(bool _set) { m_Active = _set; }//�A�N�e�B�u�Z�b�g
	inline bool GetActive()const { return m_Active; }//�L�����擾

	inline class Actor* GetOwner() { return m_Owner; }//���̃R���|�[�l���g���������Ă���A�N�^�[�擾

	virtual bool SaveComponentData(const char* _fileName);//�R���|�[�l���g�̃f�[�^���Z�[�u
	virtual bool LoadComponentData(const char* _fileName);

protected:

	class Actor* m_Owner;//���̃R���|�[�l���g���������Ă���A�N�^�[
	int m_UpdatePriority;//�X�V�D�揇
	bool m_Active;//�L����
};
