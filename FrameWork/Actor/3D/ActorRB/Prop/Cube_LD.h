//=============================================================================
//
// ���x���f�U�C���p�L���[�u�N���X [Cube_LD.h]
//  Date   : 2021/1/19
//
//=============================================================================
#pragma once


#include "../Base/ActorRB.h"

class Cube_LD : public ActorRB
{
protected:

	class Component_Cube* m_Component_Cube;//�L���[�u�R���|�[�l���g

public:

	static void Load();//���\�[�X�ǂݍ���
	static void Unload();//���\�[�X�폜

	void Init()override;//������
	void Update()override;//�X�V
	void Uninit() override;

	void OffOBB();
	void AddOBB();


};