#pragma once

//******************************************************************************
// INCLUDE
//******************************************************************************
#include "../System/Singleton.h"

//******************************************************************************
// �N���X��`
//******************************************************************************
class Manager_Grid2D : public Singleton<Manager_Grid2D>
{
private:

	friend Singleton<Manager_Grid2D>;

	Vector2	scale;//1�}�X�̃X�P�[��
	Color color;//�ʏ�J���[
	Color centerColor;//�^�񒆂̐��̃J���[
	Int2 lineNum;//���̐�
	class MainWindow* cacheMain;//���C���E�B���h�E�L���b�V��

public:

	void Init();//������
	void Draw();//�`��

	void SetLength(float _len);//���l�p�`�̃T�C�Y
	void SetScale(Vector2 _scale);//��̎l�p�`�̃T�C�Y
	void SetCutNum(Int2 _cut);//��ʂ����������邩
	inline void SetColor(Color _color) { color = _color; }//�J���[�Z�b�g
	inline void SetCenterColor(Color _color) { centerColor = _color; }//�J���[�Z�b�g

};