//=============================================================================
//
//  �����T�E���h�N���X [FootStep.h]
//  Date   : 2021/1/21
//
//=============================================================================
#pragma once

#include <unordered_map>
#include <vector>

#include "../Manager/Manager_Drawer/Manager_OBB.h"

class FootStep
{
private:

	std::vector<const char*> m_FootStepes[MAX_PM_TYPE];//

public:

	static void Load();//���\�[�X�ǂݍ���
	static void Unload();//���\�[�X�폜

	//�������E�I��
	void Init();
	void Uninit();

	//�v���C
	void Play(int _physicsMaterial);

};