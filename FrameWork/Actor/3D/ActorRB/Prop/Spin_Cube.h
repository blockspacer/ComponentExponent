//=============================================================================
//
//  ��]�L���[�u�N���X [Spin_Cube.h]
//  Date   : 2021/12/4
//
//=============================================================================
#pragma once


#include "Cube.h"

class Spin_Cube : public Cube
{
private:

	float rot;

public:

	virtual void Init()override;//������
	virtual void Update()override;//�X�V
	void Uninit() override;


};