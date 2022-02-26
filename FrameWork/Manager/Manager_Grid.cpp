

//******************************************************************************
// �C���N���[�h
//******************************************************************************
#include "../System/Main.h"
#include "../System/MainWindow.h"
#include "../Renderer/Renderer_Polygon.h"
#include "Manager_Grid.h"

//******************************************************************************
// ������
//******************************************************************************
void Manager_Grid2D::Init()
{
	color = Color(1, 1, 1, 1);
	centerColor = Color(1, 0, 0, 1);

	cacheMain = MainWindow::Instance();
}

//******************************************************************************
// �`��
//******************************************************************************
void Manager_Grid2D::Draw()
{
	//�f�o�b�O���̂݋@�\
#ifndef _DEBUG
	return;
#endif

	//���̐�
	Color tempColor = color;
	for (int i = 0; i < lineNum.y; i++)
	{
		if (lineNum.y % 2 == 0)
		{
			//�����Ȃ�^�񒆂���{
			tempColor = (lineNum.y / 2 == (i + 1) || lineNum.y / 2 + 1 == (i + 1)) ? centerColor : color;
		}
		else
		{
			//��Ȃ�^�񒆂�����
			tempColor = lineNum.y / 2 == i ? centerColor : color;
		}

		Renderer_Polygon::Instance()->DrawLine2D({ 0.0f,scale.y * i + scale.y }, 
			{ static_cast<float>(cacheMain->GetWindowSize().x),scale.y * i + scale.y }, tempColor);
	}

	//�c�̐�
	for (int i = 0; i < lineNum.x; i++)
	{
		if (lineNum.x % 2 == 0)
		{
			//�����Ȃ�^�񒆂���{
			tempColor = (lineNum.x / 2 == (i + 1) || lineNum.x / 2 + 1 == (i + 1)) ? centerColor : color;
		}
		else
		{
			//��Ȃ�^�񒆂�����
			tempColor = lineNum.x / 2 == i ? centerColor : color;
		}

		Renderer_Polygon::Instance()->DrawLine2D({ scale.x * i + scale.x,0.0f }, 
			{ scale.x * i + scale.x,static_cast<float>(cacheMain->GetWindowSize().y) }, tempColor);
	}
}

//******************************************************************************
// ��������̖{���ƃX�P�[�����v�Z
//******************************************************************************
void Manager_Grid2D::SetLength(float _len)
{
	//��̖{�����v�Z
	lineNum.x = cacheMain->GetWindowSize().x / static_cast<int>(_len);
	lineNum.y = cacheMain->GetWindowSize().y / static_cast<int>(_len);

	//�X�P�[���Z�b�g
	scale.x = scale.y = _len;
}

//******************************************************************************
// ��������̐����v�Z
//******************************************************************************
void Manager_Grid2D::SetScale(Vector2 _scale)
{
	//��̖{�����v�Z
	lineNum.x = cacheMain->GetWindowSize().x / static_cast<int>(_scale.x);
	lineNum.y = cacheMain->GetWindowSize().y / static_cast<int>(_scale.y);

	//�X�P�[���Z�b�g
	scale = _scale;
}

//******************************************************************************
// �c�Ɖ��Ŏw�肳�ꂽ�������ɍ��킹���X�P�[���Ɛ��̖{���Ɍv�Z
//******************************************************************************
void Manager_Grid2D::SetCutNum(Int2 _cut)
{
	scale.x = static_cast<float>(cacheMain->GetWindowSize().x / (_cut.x <= 0 ? 1 : _cut.x));
	scale.y = static_cast<float>(cacheMain->GetWindowSize().y / (_cut.y <= 0 ? 1 : _cut.y));

	lineNum.x = cacheMain->GetWindowSize().x / static_cast<int>(scale.x) - 1;
	lineNum.y = cacheMain->GetWindowSize().y / static_cast<int>(scale.y) - 1;
}
