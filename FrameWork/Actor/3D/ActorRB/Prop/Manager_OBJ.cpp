
#include ""
#include "Manager_OBJ.h"

void Manager_OBJ::Draw()
{
	//���L�X�v���C�g�R���|�[�l���g��`��
	for (Component_OBJ* component_Obj : m_Component_OBJ)
	{
		component_Obj->Draw();
	}
}

void Manager_OBJ::Uninit()
{
	m_Component_OBJ.clear();//�v�f�폜
}
