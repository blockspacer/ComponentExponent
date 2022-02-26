//=============================================================================
//
//  �L���[�u�R���|�[�l���g�}�l�[�W���[ [Manager_Cube.cpp]
//  Date   : 2022/1/19
//
//=============================================================================

//******************************************************************************
// �C���N���[�h
//******************************************************************************

#include <stdarg.h>
#include <codecvt>
#include "Manager_Cube.h"
#include "../../Component/Component_Cube.h"



//�`��֐�
void Manager_Cube::Draw()
{
	//���L�r���{�[�h�R���|�[�l���g��`��
	for (Component_Cube* component_Cube : m_ComponentList)
	{
		component_Cube->Draw();
	}

}

//�I���֐�
void Manager_Cube::Uninit()
{
	m_ComponentList.clear();//�v�f�폜
}
