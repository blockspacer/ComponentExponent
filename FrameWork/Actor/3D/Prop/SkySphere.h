//=============================================================================
//
//  SkySphere�N���X [SkySphere.h]
//  Date   : 2021/1/16
//
//=============================================================================
#pragma once


#include "../Base/Actor3D.h"


class SkySphere : public Actor3D
{
private:

	class Component_StaticMesh* m_Component_StaticMesh;//�ÓI���b�V���R���|�[�l���g

public:

	virtual void Init()override;//������
	virtual void Update()override;//�X�V

};