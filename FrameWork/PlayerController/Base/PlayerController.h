//=============================================================================
//
//  �v���C���[�R���g���[���[ [PlayerConroller.h]
//  Date   : 2021/10/13
//
//=============================================================================
#pragma once

#include "../../System/Input.h"

class PlayerController
{
protected :

	class Input* m_Input;          //�L���b�V��
	class MainWindow* m_MainWindow;//�L���b�V��

	float m_Mouse_Sensi   = 0.002f;//�}�E�X���x
	float m_Gamepad_Sensi = 0.04f;//�Q�[���p�b�h���x

	//�O��ړ�
	float MoveForward();

	//���E�ړ�
	float MoveRight();
	
	//�J�������E
	float Turn();
	
	//�J�����㉺
	float LookUp();

	//�J�����Y�[��
	float Zoom();

	//���C���A�N�V�������蓖��
	bool InputAction_Main();

	//�X�v�����g
	bool InputAction_Sprint();

	//�W�����v
	bool InputAction_Jump();

	//�|�[�Y
	bool InputAction_Pause();

public:

	virtual void Init();//������
	virtual void Update() = 0;//�X�V

};
